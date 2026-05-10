import base64, requests, json, os, sys

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

with open(r'G:\Workspace\UnrealEngine\Projects\DerelictCorridorMegascans\Saved\Screenshots\mumosa_scene.png', 'rb') as f:
    img_b64 = base64.b64encode(f.read()).decode()

resp = requests.post('https://api.groq.com/openai/v1/chat/completions', json={
    'model': 'meta-llama/llama-4-scout-17b-16e-instruct',
    'messages': [{
        'role': 'user',
        'content': [
            {'type': 'text', 'text': 'List every object, hazard, damaged item, and evidence-worthy thing visible in this scene. Be specific.'},
            {'type': 'image_url', 'image_url': {'url': f'data:image/png;base64,{img_b64}'}}
        ]
    }],
    'max_tokens': 500
}, headers={
    'Authorization': f'Bearer {GROQ_API_KEY}',
    'Content-Type': 'application/json'
}, timeout=60)

data = resp.json()
print(data['choices'][0]['message']['content'].strip())
