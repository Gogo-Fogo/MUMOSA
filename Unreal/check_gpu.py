import subprocess, psutil
r = subprocess.run(['nvidia-smi', '--query-gpu=memory.used,memory.total,utilization.gpu', '--format=csv,noheader,nounits'],
                   capture_output=True, text=True, timeout=5)
print('GPU:', r.stdout.strip())
for p in psutil.process_iter(['name', 'memory_info']):
    if 'UnrealEditor' in p.info['name']:
        mem = p.info['memory_info'].rss / 1e9
        print(f'Unreal Editor RAM: {mem:.1f} GB')
