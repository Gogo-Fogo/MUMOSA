import unreal

# Clean up test actors
actors = unreal.EditorLevelLibrary.get_all_level_actors()
for a in actors:
    name = a.get_name()
    if 'UIContainer' in name or 'UIText' in name or 'UIProcedural' in name:
        unreal.EditorLevelLibrary.destroy_actor(a)
        print(f"Destroyed: {name}")

# Also check for leftover LGUIPrefabLoadHelper
for a in actors:
    name = a.get_name()
    if 'LGUIPrefab' in name:
        unreal.EditorLevelLibrary.destroy_actor(a)
        print(f"Destroyed: {name}")

print("Cleanup done")
