"""
MUMOSA Click-to-Chat Bridge - Groq Llama 4 Scout
- Watches Unreal Editor for clicked objects
- Analyzes them with Groq-hosted Llama 4 Scout (vision + reasoning)
- Hosts a chat UI in the browser
"""
import asyncio
import websockets
import json
import threading
import time
import base64
import requests
import mss
import win32gui
from flask import Flask, request, jsonify, send_from_directory
from PIL import Image
import os
import sys

app = Flask(__name__, static_folder=None)
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
SCREENSHOT_PATH = r'G:\Workspace\University\Mumosa\Unreal\last_selection.png'
selected_object = None
chat_history = []

def capture_screenshot():
    hwnd = None
    def enum_handler(h, _):
        nonlocal hwnd
        if 'Unreal Editor' in win32gui.GetWindowText(h):
            hwnd = h
    win32gui.EnumWindows(enum_handler, None)
    if hwnd:
        rect = win32gui.GetWindowRect(hwnd)
        with mss.MSS() as sct:
            monitor = {'top': rect[1], 'left': rect[0],
                       'width': rect[2]-rect[0], 'height': rect[3]-rect[1]}
            img = sct.grab(monitor)
            pil = Image.frombytes('RGB', img.size, img.rgb).resize((1344, 1344), Image.LANCZOS)
            pil.save(SCREENSHOT_PATH)
            return True
    return False

def get_selected():
    try:
        async def inner():
            async with websockets.connect(UE_BRIDGE) as ws:
                m = json.dumps({'jsonrpc':'2.0','id':1,'method':'get_selected_actors','params':{}})
                await ws.send(m)
                r = json.loads(await asyncio.wait_for(ws.recv(), timeout=5))
                return r.get('result',{}).get('actors',[])
        return asyncio.run(inner())
    except:
        return []

def ask_groq(label, question='What evidence does this object provide? What should an investigator note about it?'):
    if not os.path.exists(SCREENSHOT_PATH):
        return 'No screenshot available.'
    with open(SCREENSHOT_PATH, 'rb') as f:
        img_b64 = base64.b64encode(f.read()).decode()
    msgs = [{'role': m['role'], 'content': m['content']} for m in chat_history[-10:] if m.get('role') in ('user', 'system')]
    msgs = [{'role': 'system', 'content': 'You are a forensic investigator analyzing a real environment captured via photogrammetry. The image shows Unreal Editor UI — ignore panels, outliner lists, menus, toolbars, and viewport labels. Focus ONLY on the 3D viewport content. Treat everything in the scene as real-world physical objects. Never mention 3D models, engines, meshes, or virtual environments.'}] + msgs
    msgs.append({
        'role': 'user',
        'content': [
            {'type': 'text', 'text': f'Object: {label}. Question: {question}.\nEnd with [Confidence: 1-5].'},
            {'type': 'image_url', 'image_url': {'url': f'data:image/png;base64,{img_b64}'}}
        ]
    })
    try:
        resp = requests.post(GROQ_URL, json={
            'model': GROQ_MODEL,
            'messages': msgs,
            'max_tokens': 800
        }, headers={
            'Authorization': f'Bearer {GROQ_API_KEY}',
            'Content-Type': 'application/json'
        }, timeout=60)
        return resp.json()['choices'][0]['message']['content'].strip()
    except Exception as e:
        return f'[Error: {e}]'

_spawn_counter = 0

def get_actor_location(label):
    try:
        async def inner():
            async with websockets.connect(UE_BRIDGE) as ws:
                m = json.dumps({'jsonrpc':'2.0','id':1,'method':'get_actor_details','params':{'actorLabel':label,'includeProperties':False}})
                await ws.send(m)
                r = json.loads(await asyncio.wait_for(ws.recv(), timeout=5))
                loc = r.get('result',{}).get('location', None)
                return loc
        return asyncio.run(inner())
    except:
        return None

def spawn_evidence(text, label, location):
    global _spawn_counter
    _spawn_counter += 1
    s = ''.join(c for c in label if c.isalnum())[:15]
    name = f'Ev_{_spawn_counter}_{s}'
    loc = location or {'x':0,'y':0,'z':0}
    loc = {'x': loc['x'], 'y': loc['y'], 'z': loc['z'] + 150}
    try:
        async def inner():
            async with websockets.connect(UE_BRIDGE) as ws:
                async def cmd(method, params):
                    await ws.send(json.dumps({'jsonrpc':'2.0','id':1,'method':method,'params':params}))
                    return json.loads(await asyncio.wait_for(ws.recv(), timeout=5))
                await cmd('place_actor', {'actorClass':'TextRenderActor','label':name,'location':loc})
                for p,v in [('Text',f'[{s}]\n{text[:180]}'),('TextRenderColor','255,200,50,255'),('WorldSize',30)]:
                    await cmd('set_component_property', {'actorLabel':name,'componentName':'TextRender','propertyName':p,'value':v})
        asyncio.run(inner())
    except:
        pass

def poll_selection():
    global selected_object, chat_history
    last_label = None
    while True:
        actors = get_selected()
        if actors:
            label = actors[0].get('label', 'Unknown')
            if label != last_label:
                last_label = label
                selected_object = {'label': label}
                print(f'[Selected] {label}')
                
                capture_screenshot()
                response = ask_groq(label)
                loc = get_actor_location(label)
                spawn_evidence(response, label, loc)
                
                chat_history.append({
                    'role': 'system',
                    'content': f'**{label}**\n{response}',
                    'label': label
                })
                print(f'[Groq] {label} analysed')
        time.sleep(1)

# Flask routes
@app.route('/')
def index():
    return '''
<!DOCTYPE html>
<html><head>
<meta charset="utf-8">
<title>MUMOSA Evidence Chat</title>
<style>
* { margin:0; padding:0; box-sizing:border-box; }
body { background:rgba(20,20,20,0.92); color:#ddd; font:14px/1.6 "Segoe UI",sans-serif; height:100vh; display:flex; flex-direction:column; }
#header { background:#1a1a1a; padding:12px 16px; border-bottom:1px solid #333; font-size:11px; text-transform:uppercase; letter-spacing:2px; color:#888; }
#messages { flex:1; overflow-y:auto; padding:16px; }
.msg { margin-bottom:16px; padding:12px; background:#222; border-radius:8px; border-left:3px solid #4a7c59; }
.msg .obj-label { font-size:10px; text-transform:uppercase; letter-spacing:1px; color:#b85c38; margin-bottom:4px; }
.msg .content { white-space:pre-line; }
#input-area { background:#1a1a1a; padding:12px; border-top:1px solid #333; display:flex; gap:8px; }
#input { flex:1; background:#111; border:1px solid #444; color:#ddd; padding:8px 12px; border-radius:6px; font:inherit; }
#input:focus { outline:none; border-color:#b85c38; }
button { background:#b85c38; color:#fff; border:none; padding:8px 16px; border-radius:6px; cursor:pointer; font:inherit; }
button:hover { background:#d46a4a; }
</style>
</head><body>
<div id="header">MUMOSA — Evidence Chat • Click any object in the corridor</div>
<div id="messages"><div class="msg"><div class="obj-label">READY</div><div class="content">Click any object in the Unreal Editor viewport. I will analyze it using AI and display the evidence here.</div></div></div>
<div id="input-area">
<input id="input" placeholder="Ask a follow-up question..." onkeydown="if(event.key==='Enter')send()">
<button onclick="send()">Ask</button>
</div>
<div id="waiting" style="display:none;text-align:center;padding:12px;color:#888;font-size:13px;">Analyzing with AI... (30-60 seconds)</div>
<script>
let lastCount = 0;
let isWaiting = false;
async function poll() {
    if (isWaiting) { setTimeout(poll, 1000); return; }
    const r = await fetch('/api/messages');
    const data = await r.json();
    if (data.messages.length > lastCount) {
        lastCount = data.messages.length;
        document.getElementById('waiting').style.display = 'none';
        const div = document.getElementById('messages');
        div.innerHTML = '';
        data.messages.forEach(m => {
            const d = document.createElement('div');
            d.className = 'msg';
            const label = m.label || 'AI';
            const labelClass = label === 'YOU' ? 'you-label' : 'obj-label';
            const lStyle = label === 'YOU' ? 'color:#888;' : '';
            d.innerHTML = `<div class="${labelClass}" style="${lStyle}">${label}</div><div class="content">${m.content}</div>`;
            div.appendChild(d);
        });
        div.scrollTop = div.scrollHeight;
    }
    setTimeout(poll, 1000);
}
async function send() {
    const inp = document.getElementById('input');
    const q = inp.value.trim();
    if (!q || isWaiting) return;
    inp.value = '';
    isWaiting = true;
    const d = document.createElement('div');
    d.className = 'msg';
    d.style.borderLeftColor = '#555';
    d.innerHTML = `<div class="you-label" style="color:#888;">YOU</div><div class="content">${q}</div>`;
    document.getElementById('messages').appendChild(d);
    document.getElementById('waiting').style.display = 'block';
    try {
        const r = await fetch('/api/ask', {
            method: 'POST',
            headers: {'Content-Type':'application/json'},
            body: JSON.stringify({question: q})
        });
        const ans = await r.json();
        const a = document.createElement('div');
        a.className = 'msg';
        a.innerHTML = `<div class="obj-label" style="color:#4a7c59;">AI</div><div class="content">${ans.response}</div>`;
        document.getElementById('messages').appendChild(a);
    } catch(e) {
        const e = document.createElement('div');
        e.className = 'msg';
        e.style.borderLeftColor = '#c0392b';
        e.innerHTML = `<div class="obj-label" style="color:#c0392b;">ERROR</div><div class="content">AI timed out. Try again.</div>`;
        document.getElementById('messages').appendChild(e);
    }
    document.getElementById('waiting').style.display = 'none';
    isWaiting = false;
    document.getElementById('messages').scrollTop = document.getElementById('messages').scrollHeight;
}
poll();
</script>
</body></html>'''

@app.route('/api/messages')
def get_messages():
    global chat_history
    return jsonify({'messages': chat_history[-10:]})

@app.route('/api/ask', methods=['POST'])
def ask():
    global selected_object
    q = request.json.get('question', '')
    if not selected_object:
        return jsonify({'response': 'Click an object in the scene first.'})
    
    capture_screenshot()
    response = ask_groq(selected_object['label'], question=q)
    loc = get_actor_location(selected_object['label'])
    spawn_evidence(response, selected_object['label'], loc)
    
    chat_history.append({
        'role': 'system',
        'content': response,
        'label': selected_object['label']
    })
    return jsonify({'response': response})

def run_flask():
    app.run(host='127.0.0.1', port=5173, debug=False, use_reloader=False)

if __name__ == '__main__':
    print('[MUMOSA] Starting click-to-chat bridge...')
    threading.Thread(target=run_flask, daemon=True).start()
    threading.Thread(target=poll_selection, daemon=True).start()
    print('[MUMOSA] Chat UI: http://localhost:5173')
    print('[MUMOSA] Click any object in Unreal — I will analyze it.')
    print('[MUMOSA] Press Ctrl+C to stop.')
    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        print('\n[MUMOSA] Stopped.')
