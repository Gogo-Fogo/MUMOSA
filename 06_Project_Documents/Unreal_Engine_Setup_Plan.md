# Unreal Engine Setup Plan

This plan defines how to set up Unreal Engine for the MUMOSA spatial review prototype in a way that works today and still supports the long-term roadmap.

## Short Answer

Unreal does have a VR Template, similar in spirit to Unity's VR template. It includes VR pawn setup, teleport locomotion, controller interaction, and OpenXR support.

However, for this prototype, start with the **First Person C++ template**, not the VR Template.

Reason:

- Tomorrow's user research needs a reliable mouse/keyboard PC demo.
- The simulation must work without a headset.
- VR is the goal, but PC is the fallback and research-safe version.
- The core infrastructure should be evidence markers, source panels, timeline states, and dashboard handoff, not headset setup.

Recommended approach:

```text
Start First Person C++ template -> build PC spatial review -> add VR/OpenXR later
```

## Today Setup

### Project Template

Use:

- `Games > First Person`
- C++ project
- Desktop target
- Starter content optional

Do not start with:

- VR Template
- Third Person Template
- Blank project

Why:

- First Person gives immediate WASD/mouse movement.
- It is familiar to users during testing.
- It reduces time lost to VR hardware setup.
- VR interaction can be added after the marker/panel system works.

### Project Name

Suggested:

```text
MumosaSpatial
```

### Initial Map

Create:

```text
Content/MUMOSA/Maps/MUMOSA_SpatialReview_Demo
```

This should be the main demo level for user research.

## Folder Setup

Create these folders inside Unreal:

```text
Content/MUMOSA/Maps
Content/MUMOSA/Blueprints
Content/MUMOSA/UI
Content/MUMOSA/Materials
Content/MUMOSA/Data
Content/MUMOSA/Props
Content/MUMOSA/VFX
Content/MUMOSA/PointClouds
Content/MUMOSA/Media
Content/MUMOSA/Docs
```

Create these source folders:

```text
Source/MumosaSpatial/Public/Data
Source/MumosaSpatial/Public/Evidence
Source/MumosaSpatial/Public/Timeline
Source/MumosaSpatial/Public/UI
Source/MumosaSpatial/Public/Integration
Source/MumosaSpatial/Private/Data
Source/MumosaSpatial/Private/Evidence
Source/MumosaSpatial/Private/Timeline
Source/MumosaSpatial/Private/UI
Source/MumosaSpatial/Private/Integration
```

## Plugins To Enable Today

Enable only what supports the immediate proof of concept.

### Enhanced Input

Purpose:

- Mouse/keyboard controls.
- Future controller mapping.

Status:

- Usually already used by modern Unreal templates.

### Modeling Tools Editor Mode

Purpose:

- Quick blockouts and simple edits inside Unreal.
- Useful if no 3D modeling tool is available.

Use today:

- Yes, if you need quick geometry cleanup.

### Niagara

Purpose:

- Smoke, fire, dust, hazard visuals.

Use today:

- Yes, but lightly.

### UMG

Purpose:

- Evidence panels, timeline buttons, dashboard handoff banner.

Use today:

- Yes.

### Python Editor Script Plugin

Purpose:

- Later automation for placing markers, batch importing assets, generating scene objects from JSON.

Use today:

- Optional. Enable only if we actually automate editor tasks today.

## Plugins To Delay

Do not enable these until the PC MVP works.

### OpenXR

Purpose:

- Future VR support.

Delay because:

- It is not needed for tomorrow's PC research prototype.
- VR setup can distract from the evidence-review flow.

### VR Template / VR Expansion Work

Purpose:

- Future headset mode.

Delay because:

- The same evidence marker system should work before VR is added.

### LiDAR Point Cloud Plugin

Purpose:

- Import real/synthetic point clouds.

Delay because:

- We do not yet have scan data.
- It can add complexity before the core interaction works.

Use later when:

- We have sample point cloud data.
- We want a "scan view" or digital twin demonstration.

### Pixel Streaming

Purpose:

- Browser access to a high-fidelity Unreal scene.

Delay because:

- Too much setup for today.
- Needs server/network/GPU planning.

Use later when:

- Dashboard integration needs browser-based Unreal access.

### NWIRO / Rekall / StraySpark / MCP Bridge

Purpose:

- AI/editor automation.

Delay because:

- First we need a clean Unreal project.
- Too many plugins can create debugging noise.

Use later when:

- We want AI to place actors, import data, or automate editor tasks.

## Nanite Plan

Use Nanite later, not as a first concern today.

### Today

Do not spend time optimizing around Nanite.

Use:

- Simple blockouts.
- A few realistic props.
- Basic materials.
- Simple VFX.

Reason:

- Nanite matters when we have high-poly assets, photogrammetry, scanned meshes, rubble piles, dense industrial props, or large detailed environments.
- Today the risk is not geometry performance. The risk is losing time before the interaction works.

### Long Term

Use Nanite for:

- Photogrammetry-style environment assets.
- High-poly debris.
- Industrial scene scans.
- Realistic rubble.
- Large static meshes.

Do not rely on Nanite for:

- Transparent materials.
- UI.
- Particle effects.
- Everything by default without checking asset behavior.

## Environment Plan

### Today Environment

Use a simple scene:

- Rail/industrial yard.
- Railcar cluster.
- Ignition zone.
- Smoke drift area.
- Response vehicle area.
- Sampling zone.
- Drone/bodycam source positions.

The scene can be:

- Blockout geometry.
- Free Fab/Megascans assets.
- Simple cubes with better materials.
- One or two free industrial/rail props if found quickly.

Rule:

- Do not asset-hunt for more than 45 minutes today.

### Free Asset Search Targets

Look for:

- Industrial environment.
- Rail tracks.
- Train/railcar prop.
- Warehouse or factory props.
- Debris/rubble.
- Emergency vehicle.
- Hazard signs/tape.
- Smoke/fire Niagara effects.
- Concrete/asphalt/gravel materials.

Good sources:

- Fab free content.
- Megascans/free materials.
- Epic sample content.
- Poly Haven for HDRIs/materials if needed.

### If No Good Free Environment Is Found

Use blockout.

The prototype still works if the scene is simple, as long as:

- Markers are clear.
- Evidence panel is clear.
- Hazard/timeline context is visible.
- Dashboard handoff is understandable.

## Short-Term Tool Stack For Today

Required:

- Unreal Engine 5.7.x.
- First Person C++ template.
- Enhanced Input.
- UMG.
- Basic C++ classes.
- Simple materials.
- Optional free Fab assets.

Helpful:

- Visual Studio 2022.
- GitHub Desktop.
- Git LFS already enabled.
- Codex/OpenCode for code implementation.

Avoid today:

- VR hardware setup.
- Pixel Streaming.
- MCP bridge setup.
- LiDAR plugin unless we already have data.
- Heavy paid environment packs.
- Complex AI world-builder tools.

## Long-Term Tool Stack

Core:

- Unreal Engine 5.7.x or later stable.
- C++ code-first systems.
- UMG/Common UI if UI grows.
- Enhanced Input.
- OpenXR for VR.
- LiDAR Point Cloud Plugin.
- Nanite for high-detail static geometry.
- Lumen for lighting.
- Niagara for hazards.
- Python Editor Script Plugin for automation.
- MCP/editor bridge for AI-assisted scene operations.

Pipeline:

- Dashboard sends scene launch parameters.
- Unreal loads incident data.
- Evidence markers load from JSON/API.
- Source media is streamed or loaded from cloud storage.
- Point clouds/scans are optional scene layers.
- VR mode uses same marker and evidence systems as PC mode.

Future deployment:

- Windows app for high-fidelity local demo.
- Itch.io or release page for simple distribution.
- Pixel Streaming for browser-based high-fidelity access.
- Deep links for dashboard-to-app launch.

## Recommended Today Build Sequence

1. Create First Person C++ project.
2. Create `Content/MUMOSA` folder structure.
3. Create simple rail/industrial blockout map.
4. Add 5 marker placeholders.
5. Implement marker selection.
6. Implement evidence panel.
7. Implement hazard overlay toggle.
8. Implement simple timeline state selector.
9. Add dashboard handoff banner.
10. Add return-to-dashboard mock.
11. Add free assets/materials only after the flow works.
12. Record backup walkthrough video.

## Decision Summary

Use First Person C++ template today.

Do not start with VR Template.

Do not center the build around Nanite, LiDAR, or AI world-building tools yet.

Build the scalable core:

```text
movement + markers + evidence data + UI panel + timeline/hazards + dashboard handoff
```

Then upgrade the visual and technical stack later:

```text
Nanite + LiDAR/point cloud + OpenXR VR + MCP/editor automation + Pixel Streaming
```

