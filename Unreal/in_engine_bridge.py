"""
MUMOSA In-Engine Evidence Chat Bridge - Groq Llama 4 Scout
- Polls Unreal for selected objects
- Analyzes with Groq-hosted Llama 4 Scout (vision + reasoning)
- Updates the EUW_EvidenceChat widget in the editor
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
import os
import sys
from PIL import Image

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
WIDGET_PATH = '/Game/MUMOSA/UI/EUW_EvidenceChat.EUW_EvidenceChat'
CHAT_HISTORY = []

async def bridge_call(method, params):
    try:
        async with websockets.connect(UE_BRIDGE, ping_interval=None) as ws:
            msg = json.dumps({'jsonrpc': '2.0', 'id': 1, 'method': method, 'params': params})
            await ws.send(msg)
            resp = await asyncio.wait_for(ws.recv(), timeout=15)
            r = json.loads(resp)
            if 'result' in r and r['result'].get('success'):
                return r['result']
            return None
    except Exception as e:
        print(f'[ERROR] Bridge call {method}: {e}')
        return None

def capture_screenshot():
    hwnd = None
    def enum_handler(h, _):
        nonlocal hwnd
        t = win32gui.GetWindowText(h)
        if 'Unreal Editor' in t:
            hwnd = h
    win32gui.EnumWindows(enum_handler, None)
    if hwnd:
        try:
            rect = win32gui.GetWindowRect(hwnd)
            with mss.MSS() as sct:
                mon = {'top': rect[1], 'left': rect[0], 'width': rect[2]-rect[0], 'height': rect[3]-rect[1]}
                img = sct.grab(mon)
                pil = Image.frombytes('RGB', img.size, img.rgb).resize((1344, 1344), Image.LANCZOS)
                pil.save(SCREENSHOT_PATH)
                return True
        except:
            pass
    return False

def get_selected():
    try:
        r = asyncio.run(bridge_call('get_selected_actors', {}))
        if r and r.get('actors'):
            return r['actors']
    except:
        pass
    return []

def update_widget_text(text):
    try:
        asyncio.run(bridge_call('set_widget_property', {
            'assetPath': WIDGET_PATH,
            'widgetName': 'ChatDisplay',
            'propertyName': 'Text',
            'value': text
        }))
    except:
        pass

def ask_ai(object_label, question="What is this object and what evidence does it provide?"):
    if not os.path.exists(SCREENSHOT_PATH):
        return 'No screenshot available.'
    with open(SCREENSHOT_PATH, 'rb') as f:
        img_b64 = base64.b64encode(f.read()).decode()

    msgs = [{'role': m['role'], 'content': m['content']} for m in CHAT_HISTORY[-10:] if m.get('role') in ('user', 'system')]
    msgs = [{'role': 'system', 'content': 'You are a forensic investigator analyzing a real environment captured via photogrammetry. The image shows Unreal Editor UI — ignore panels, outliner lists, menus, toolbars, and viewport labels. Focus ONLY on the 3D viewport content. Treat everything in the scene as real-world physical objects. Never mention 3D models, engines, meshes, or virtual environments.'}] + msgs
    msgs.append({
        'role': 'user',
        'content': [
            {'type': 'text', 'text': f'Object: {object_label}. Question: {question}.\nEnd with [Confidence: 1-5].'},
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
        }, timeout=60)
        return r.json()['choices'][0]['message']['content'].strip()
    except Exception as e:
        return f'[AI Error: {e}]'

def main_loop():
    global CHAT_HISTORY
    last_label = None
    while True:
        actors = get_selected()
        if actors:
            label = actors[0].get('label', '')
            if label and label != last_label:
                last_label = label
                print(f'[SELECTED] {label}')
                update_widget_text(f'Analyzing {label}...')
                capture_screenshot()
                response = ask_ai(label)
                CHAT_HISTORY.append({'role': 'system', 'content': response})
                update_widget_text(f'--- {label} ---\n{response}')
                print(f'[AI RESPONSE] {response[:100]}...')
        time.sleep(1.5)

if __name__ == '__main__':
    print('[MUMOSA] In-Engine Bridge started')
    print('[MUMOSA] Click objects in Unreal viewport')
    update_widget_text('MUMOSA Evidence AI Ready\n\nClick any object in the scene to analyze.')
    main_loop()
