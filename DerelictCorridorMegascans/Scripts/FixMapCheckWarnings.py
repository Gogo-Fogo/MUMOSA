import unreal


EXTRA_MATERIAL_ACTORS = {
    "StaticMeshActor_UAID_387C764919CE3F5402_1936879322",
    "StaticMeshActor_UAID_387C764919CE405402_1182493502",
    "StaticMeshActor_UAID_E04F43E64BA9615402_2030062290",
    "StaticMeshActor_UAID_387C764919CE405402_1161748501",
    "StaticMeshActor_UAID_387C764910C2BB5002_1773116897",
}

NO_SIMPLE_COLLISION_PHYSICS_ACTORS = {
    "StaticMeshActor_UAID_E04F43E64BA96E4F02_1696174358",
    "StaticMeshActor_UAID_E04F43E64BA96F4F02_1162685563",
    "StaticMeshActor_UAID_E04F43E64BA96F4F02_1162682560",
    "StaticMeshActor_UAID_E04F43E64BA96F4F02_1162680558",
    "StaticMeshActor_UAID_E04F43E64BA96E4F02_1132495330",
    "StaticMeshActor_UAID_E04F43E64BA96E4F02_1696177361",
    "StaticMeshActor_UAID_E04F43E64BA96E4F02_1132492326",
    "StaticMeshActor_UAID_E04F43E64BA96F4F02_1162676554",
    "StaticMeshActor_UAID_E04F43E64BA96F4F02_1162677555",
}


def actor_matches(actor, names):
    actor_name = actor.get_name()
    actor_label = actor.get_actor_label()
    return actor_name in names or actor_label in names


def get_static_mesh_component(actor):
    components = actor.get_components_by_class(unreal.StaticMeshComponent)
    return components[0] if components else None


def trim_extra_material_overrides(component):
    static_mesh = component.static_mesh
    if not static_mesh:
        return False

    slot_count = len(static_mesh.static_materials)
    overrides = list(component.get_editor_property("override_materials"))
    if len(overrides) <= slot_count:
        return False

    component.modify()
    component.set_editor_property("override_materials", overrides[:slot_count])
    unreal.log(
        "Trimmed material overrides on {} from {} to {}".format(
            component.get_path_name(), len(overrides), slot_count
        )
    )
    return True


def disable_invalid_physics(component):
    is_simulating = False
    for property_name in ("body_instance",):
        try:
            body_instance = component.get_editor_property(property_name)
            is_simulating = bool(body_instance.get_editor_property("simulate_physics"))
            break
        except Exception:
            pass

    try:
        is_simulating = component.is_simulating_physics() or is_simulating
    except Exception:
        pass

    if not is_simulating:
        return False

    component.modify()
    component.set_simulate_physics(False)
    unreal.log("Disabled simulate physics on {}".format(component.get_path_name()))
    return True


def main():
    editor_actor_subsystem = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
    actors = editor_actor_subsystem.get_all_level_actors()

    changed_actors = []
    seen_material_names = set()
    seen_physics_names = set()

    for actor in actors:
        component = get_static_mesh_component(actor)
        if not component:
            continue

        changed = False
        if actor_matches(actor, EXTRA_MATERIAL_ACTORS):
            seen_material_names.add(actor.get_name())
            changed = trim_extra_material_overrides(component) or changed

        if actor_matches(actor, NO_SIMPLE_COLLISION_PHYSICS_ACTORS):
            seen_physics_names.add(actor.get_name())
            changed = disable_invalid_physics(component) or changed

        if changed:
            actor.modify()
            changed_actors.append(actor.get_actor_label())

    missing_material = sorted(EXTRA_MATERIAL_ACTORS - seen_material_names)
    missing_physics = sorted(NO_SIMPLE_COLLISION_PHYSICS_ACTORS - seen_physics_names)
    if missing_material:
        unreal.log_warning("Material override actors not found: {}".format(missing_material))
    if missing_physics:
        unreal.log_warning("Physics actors not found: {}".format(missing_physics))

    if changed_actors:
        unreal.EditorLevelLibrary.save_current_level()
    unreal.log("Map check warning fix complete. Changed {} actor(s).".format(len(changed_actors)))


main()
