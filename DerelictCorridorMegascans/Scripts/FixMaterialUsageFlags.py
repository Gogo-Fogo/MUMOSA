import unreal


FLAGS_BY_ASSET = {
    "/Fab/Materials/VT/M_MS_Srf_VT": [
        ["bUsedWithNanite", "used_with_nanite", "use_with_nanite"],
        ["bUsedWithInstancedStaticMeshes", "used_with_instanced_static_meshes", "use_with_instanced_static_meshes"],
    ],
    "/Fab/Materials/VT/M_MS_Base_VT": [
        ["bUsedWithNanite", "used_with_nanite", "use_with_nanite"],
        ["bUsedWithInstancedStaticMeshes", "used_with_instanced_static_meshes", "use_with_instanced_static_meshes"],
    ],
    "/Game/DerelictCorridor/Assets/Fab/Megascans/3D/Ind_Con_Pile_Rubble_Gravel_Patch_01/MI_Ind_Con_Pile_Rubble_Gravel_Patch_01_A": [
        ["bUsedWithInstancedStaticMeshes", "used_with_instanced_static_meshes", "use_with_instanced_static_meshes"]
    ],
    "/Game/DerelictCorridor/Assets/Fab/Megascans/Surfaces/Urb_Wall_Plaster_Old_09/MI_Urb_Wall_Plaster_Old_09_B": [
        ["bUsedWithInstancedStaticMeshes", "used_with_instanced_static_meshes", "use_with_instanced_static_meshes"]
    ],
}


def get_material_usage_target(asset):
    if isinstance(asset, unreal.Material):
        return asset

    if isinstance(asset, unreal.MaterialInstance):
        parent = try_get_editor_property(asset, "parent")
        if parent:
            unreal.log(f"{asset.get_path_name()} is a material instance; applying usage flags to parent {parent.get_path_name()}")
            return parent

    return asset


def try_get_editor_property(asset, property_name):
    try:
        return asset.get_editor_property(property_name)
    except Exception:
        return None


def try_set_editor_property(asset, property_name, value):
    try:
        asset.set_editor_property(property_name, value)
        return True
    except Exception:
        return False


def set_flag(asset, candidate_names):
    for property_name in candidate_names:
        current_value = try_get_editor_property(asset, property_name)
        if current_value is None:
            continue

        if current_value:
            unreal.log(f"{asset.get_path_name()} already has {property_name}=true")
            return False

        if try_set_editor_property(asset, property_name, True):
            unreal.log(f"Set {property_name}=true on {asset.get_path_name()}")
            return True

    unreal.log_warning(f"{asset.get_path_name()} did not expose any of: {candidate_names}")
    return False


def apply_material_usage(asset, candidate_names):
    asset = get_material_usage_target(asset)
    usage_enum = getattr(unreal, "MaterialUsage", None)
    set_usage = getattr(unreal.MaterialEditingLibrary, "set_material_usage", None)
    if not usage_enum or not set_usage:
        return False

    usage_by_name = {
        "bUsedWithNanite": "MATUSAGE_NANITE",
        "used_with_nanite": "MATUSAGE_NANITE",
        "use_with_nanite": "MATUSAGE_NANITE",
        "bUsedWithInstancedStaticMeshes": "MATUSAGE_INSTANCED_STATIC_MESHES",
        "used_with_instanced_static_meshes": "MATUSAGE_INSTANCED_STATIC_MESHES",
        "use_with_instanced_static_meshes": "MATUSAGE_INSTANCED_STATIC_MESHES",
    }
    for property_name in candidate_names:
        enum_name = usage_by_name.get(property_name)
        usage_value = getattr(usage_enum, enum_name, None) if enum_name else None
        if usage_value:
            try:
                set_usage(asset, usage_value)
                unreal.log(f"Applied material usage {enum_name} to {asset.get_path_name()}")
                return True
            except Exception as exc:
                unreal.log_warning(f"Could not apply {enum_name} to {asset.get_path_name()}: {exc}")
    return False


dirty_assets = []

for asset_path, flag_names in FLAGS_BY_ASSET.items():
    asset = unreal.EditorAssetLibrary.load_asset(asset_path)
    if not asset:
        unreal.log_warning(f"Could not load material asset: {asset_path}")
        continue

    changed = False
    assets_to_save = {asset_path}
    for candidate_names in flag_names:
        usage_target = get_material_usage_target(asset)
        if usage_target != asset:
            assets_to_save.add(usage_target.get_path_name().split(".")[0])

        changed = set_flag(usage_target, candidate_names) or apply_material_usage(asset, candidate_names) or changed

    if changed:
        for save_path in sorted(assets_to_save):
            save_asset = unreal.EditorAssetLibrary.load_asset(save_path)
            if save_asset:
                unreal.EditorAssetLibrary.save_loaded_asset(save_asset)
                dirty_assets.append(save_path)
                unreal.log(f"Saved material usage flags: {save_path}")
    else:
        unreal.log(f"Material usage flags already set: {asset_path}")

unreal.log(f"Material usage flag fix complete. Saved {len(dirty_assets)} asset(s).")
