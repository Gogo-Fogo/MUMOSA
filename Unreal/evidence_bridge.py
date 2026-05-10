"""
MUMOSA Bridge v4 - Groq Llama 4 Scout
- Viewport-only screenshots, resized to 672x672
- Groq-hosted Llama 4 Scout vision + reasoning in one call
- Scene inventory from bridge API
- Performance logging
"""
import asyncio, websockets, json, threading, time, base64, requests, os, sys
from datetime import datetime
from PIL import Image
from flask import Flask, request, jsonify

UE_BRIDGE = 'ws://127.0.0.1:9877'
# Load .env file (same dir) if GROQ_API_KEY not set
if not os.environ.get('GROQ_API_KEY'):
    _env_path = os.path.join(os.path.dirname(__file__), '.env')
    if os.path.exists(_env_path):
        for _line in open(_env_path):
            if _line.startswith('GROQ_API_KEY='):
                os.environ['GROQ_API_KEY'] = _line.strip().split('=', 1)[1]
GROQ_API_KEY = os.environ.get('GROQ_API_KEY')
if not GROQ_API_KEY:
    print('[FATAL] Set GROQ_API_KEY in .env file or as environment variable')
    sys.exit(1)
GROQ_URL = 'https://api.groq.com/openai/v1/chat/completions'
GROQ_MODEL = 'meta-llama/llama-4-scout-17b-16e-instruct'
SCREENSHOT = r'G:\Workspace\University\Mumosa\Unreal\viewport_capture.png'
SELECTED = None
INVENTORY = 'Not loaded'
CHAT = [{'role': 'system', 'content': 'You are a forensic investigator analyzing a real environment captured via photogrammetry. The image shows Unreal Editor UI — ignore panels, outliner lists, menus, toolbars, and viewport labels. Focus ONLY on the 3D viewport content. Treat everything in the scene as real-world physical objects. Never mention 3D models, engines, meshes, or virtual environments. Answer as if at a real investigation site.', 'label': 'MUMOSA'}]
PERF = []
app = Flask(__name__)

def log(msg):
    t = datetime.now().strftime('%H:%M:%S')
    line = f'[{t}] {msg}'
    print(line); sys.stdout.flush(); PERF.append(line)

async def bridge(method, params):
    try:
        async with websockets.connect(UE_BRIDGE, ping_interval=None) as ws:
            await ws.send(json.dumps({'jsonrpc':'2.0','id':1,'method':method,'params':params}))
            r = json.loads(await asyncio.wait_for(ws.recv(), timeout=15))
            return r.get('result')
    except Exception as e:
        log(f'Bridge: {e}')
        return None

def capture_viewport():
    try:
        import win32gui, mss
        found = [None]
        def cb(h, _):
            if win32gui.GetClassName(h) == 'UnrealWindow' and win32gui.IsWindowVisible(h):
                found[0] = win32gui.GetWindowRect(h)
        win32gui.EnumWindows(cb, None)
        r = found[0]
        if not r or (r[2]-r[0]) < 100 or (r[3]-r[1]) < 100:
            return False
        with mss.MSS() as sct:
            shot = sct.grab({'top':r[1], 'left':r[0], 'width':r[2]-r[0], 'height':r[3]-r[1]})
            pil = Image.frombytes('RGB', shot.size, shot.rgb).resize((1344, 1344), Image.LANCZOS)
            pil.save(SCREENSHOT)
            log(f'Viewport: {pil.size[0]}x{pil.size[1]}')
        return True
    except Exception as e:
        log(f'Capture: {e}')
        return False

def load_inventory():
    global INVENTORY
    t0 = time.time()
    try:
        r = asyncio.run(bridge('get_actors_by_class', {'className': 'StaticMeshActor'}))
        if r and r.get('actors'):
            names = [a.get('label','') for a in r['actors'] if a.get('label')]
            cats = {}
            for n in names:
                p = n.split('_')
                cats.setdefault(p[2] if len(p)>=3 else 'Misc', []).append(n)
            INVENTORY = f'{len(names)} objects: '+', '.join(f'{c}({len(v)})' for c,v in sorted(cats.items())[:12])
            log(f'Inventory: {len(names)} ({time.time()-t0:.1f}s)')
    except Exception as e:
        log(f'Inventory: {e}')

def ask_ai(obj, question):
    t0 = time.time()
    if not capture_viewport():
        return '[Error: no viewport]'
    if not os.path.exists(SCREENSHOT):
        return '[Error: no screenshot]'
    with open(SCREENSHOT, 'rb') as f:
        img_b64 = base64.b64encode(f.read()).decode()
    log(f'Image: {os.path.getsize(SCREENSHOT)//1024}KB, {len(img_b64)//1024}KB b64')

    # Build messages with chat history for context, image at end of latest user msg
    msgs = []
    for m in CHAT[-10:]:
        if m.get('role') in ('user', 'system'):
            msgs.append({'role': m['role'], 'content': m['content']})
    msgs.append({
        'role': 'user',
        'content': [
            {'type': 'text', 'text': f'Focus on the 3D viewport content only. Ignore the Unreal Editor UI (panels, outliner, menus). This is a real photogrammetry scene.\nInspecting object labeled: {obj}\nScene inventory: {INVENTORY}\nQuestion: {question}\nEnd with [Confidence: 1-5].'},
            {'type': 'image_url', 'image_url': {'url': f'data:image/png;base64,{img_b64}'}}
        ]
    })
    try:
        r = requests.post(GROQ_URL, json={
            'model': GROQ_MODEL,
            'messages': msgs,
            'max_tokens': 800
        }, headers={
            'Authorization': f'Bearer {GROQ_API_KEY}',
            'Content-Type': 'application/json'
        }, timeout=120)
        resp = r.json()['choices'][0]['message']['content'].strip()
        log(f'Groq Scout: {len(resp)} chars, {time.time()-t0:.1f}s')
        return resp
    except Exception as e:
        log(f'Groq: {e}')
        return f'[Error: {e}]'

def get_actor_location(label):
    try:
        r = asyncio.run(bridge('get_actor_details', {'actorLabel': label, 'includeProperties': False}))
        return r.get('location') if r else None
    except:
        return None

def spawn_text(text, label, location=None):
    global COUNTER
    COUNTER += 1
    s = ''.join(c for c in label if c.isalnum())[:15]
    name = f'Ev_{COUNTER}_{s}'
    if location:
        loc = {'x': location['x'], 'y': location['y'], 'z': location['z'] + 150}
    else:
        loc = {'x': 9500+COUNTER*30, 'y': -2000-COUNTER*40, 'z': 300+COUNTER*50}
    try:
        asyncio.run(bridge('place_actor', {
            'actorClass': 'TextRenderActor', 'label': name, 'location': loc
        }))
        for p, v in [('Text',f'[{s}]\\n{text[:180]}'),
                      ('TextRenderColor','255,200,50,255'),
                      ('WorldSize',30)]:
            asyncio.run(bridge('set_component_property', {
                'actorLabel':name,'componentName':'TextRender',
                'propertyName':p,'value':v}))
    except:
        pass

def poll():
    global SELECTED
    last_logged = None
    while True:
        try:
            r = asyncio.run(bridge('get_selected_actors', {}))
            if r and r.get('actors'):
                lbl = r['actors'][0].get('label','')
                if lbl and lbl not in ('WorldSettings', last_logged):
                    SELECTED = lbl
                    last_logged = lbl
                    log(f'Selected: {lbl}')
        except: pass
        time.sleep(0.3)

@app.route('/')
def index():
    return open(r'G:\Workspace\University\Mumosa\Unreal\chat_ui.html',encoding='utf-8').read()

@app.route('/api/status')
def status():
    return jsonify({'selected':SELECTED,'chat':CHAT[-15:]})

@app.route('/api/ask', methods=['POST'])
def api_ask():
    global SELECTED
    q = request.json.get('question','')
    obj = SELECTED
    CHAT.append({'role':'user','content':q,'label':'YOU'})
    if not obj:
        msg = 'Click an object in the Unreal viewport first.'
        CHAT.append({'role':'system','content':msg,'label':'MUMOSA'})
        return jsonify({'response':msg,'object':None})
    resp = ask_ai(obj, q)
    CHAT.append({'role':'system','content':resp,'label':obj})
    loc = get_actor_location(obj)
    spawn_text(resp, obj, loc)
    return jsonify({'response':resp,'object':obj})

@app.route('/api/perf')
def api_perf():
    return jsonify({'log':PERF[-30:]})

if __name__ == '__main__':
    log('=== MUMOSA Bridge v4 (Groq Scout) ===')
    load_inventory()
    threading.Thread(target=poll, daemon=True).start()
    threading.Thread(target=lambda: app.run(host='127.0.0.1',port=5173,debug=False,use_reloader=False), daemon=True).start()
    log('Ready: http://localhost:5173')
    try:
        while True: time.sleep(1)
    except KeyboardInterrupt:
        log('Stopped')
