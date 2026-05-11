# Fix placeholder text in WBP_EvidenceOverlay.uasset
with open('G:/Workspace/UnrealEngine/Projects/MUMOSA/DerelictCorridorMegascans/Content/UI/WBP_EvidenceOverlay.uasset', 'rb') as f:
    data = bytearray(f.read())

text_to_replace = b'Select an object to inspect'
replacement = b'                              '

if text_to_replace in data:
    data = data.replace(text_to_replace, replacement)
    with open('G:/Workspace/UnrealEngine/Projects/MUMOSA/DerelictCorridorMegascans/Content/UI/WBP_EvidenceOverlay.uasset', 'wb') as f:
        f.write(data)
    print("Done! Patched placeholder text in WBP_EvidenceOverlay.uasset")
else:
    print("Placeholder text not found")