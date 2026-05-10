import asyncio, websockets, json

async def test():
    async with websockets.connect('ws://127.0.0.1:9877') as ws:
        msg = json.dumps({'jsonrpc':'2.0','id':1,'method':'get_actors_by_class','params':{'className':'StaticMeshActor'}})
        await ws.send(msg)
        r = json.loads(await asyncio.wait_for(ws.recv(), timeout=10))
        result = r.get('result', {})
        if result.get('success'):
            actors = result.get('actors', [])
            print(f'Total: {len(actors)}')
            for a in actors[:10]:
                print(a.get('label', '?'))
        else:
            # Try with other param name
            msg2 = json.dumps({'jsonrpc':'2.0','id':2,'method':'get_actors_by_class','params':{'class':'StaticMeshActor'}})
            await ws.send(msg2)
            r2 = json.loads(await asyncio.wait_for(ws.recv(), timeout=10))
            print(json.dumps(r2)[:300])

asyncio.run(test())
