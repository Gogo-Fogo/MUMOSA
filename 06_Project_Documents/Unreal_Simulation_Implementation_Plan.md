# Unreal Simulation Implementation Plan

Companion document to `Unreal_Simulation_Long_Term_Roadmap.md`.

This file is intentionally practical and staged. It is written so smaller AI models can work on one implementation stage at a time without needing to understand the entire MUMOSA project history.

## Project Goal

Build a PC-first Unreal Engine spatial review prototype for MUMOSA.

The user should be able to walk through a reconstructed incident scene, click evidence markers, read source-grounded AI/evidence details, toggle hazard/timeline states, and understand how this module would connect back to the dashboard.

## Non-Goals

Do not build these in the first implementation pass:

- Full dashboard.
- Real MUMOSA backend.
- Real AI inference.
- Real-time sensor fusion.
- Full VR interaction.
- Multiplayer.
- Real forensic reconstruction pipeline.
- Pixel Streaming.
- Production security/auth.

## Implementation Rules

1. Use C++ for core logic.
2. Use Blueprints only as thin editor/visual wrappers when needed.
3. Keep data separate from scene logic.
4. Make every feature demoable before moving to the next.
5. Prefer simple, readable systems over clever abstractions.
6. Do not depend on paid AI plugins for core functionality.
7. Keep PC mouse/keyboard working at every stage.

## Recommended Unreal Project Shape

```text
Source/MumosaSpatial/
  MumosaSpatial.Build.cs
  Public/
    Evidence/
    Timeline/
    UI/
    Data/
    Interaction/
  Private/
    Evidence/
    Timeline/
    UI/
    Data/
    Interaction/

Content/MUMOSA/
  Maps/
  Blueprints/
  UI/
  Materials/
  Data/
  Props/
  VFX/
  PointClouds/
```

## Stage 0: Create Project And Baseline

Purpose:

- Create the Unreal project and confirm the editor/build pipeline works.

Human actions:

- Install Unreal Engine 5.7.x.
- Create a new C++ project from the First Person template.
- Name it `MumosaSpatial` or similar.
- Confirm the default first-person map runs.

AI/code tasks:

- Create or update a project README.
- Add the recommended folder structure.
- Add a short architecture note in `Content/MUMOSA/Docs/` if desired.

Acceptance check:

- Project opens.
- Player can move with mouse/keyboard.
- C++ compiles.
- Folder structure exists.

Stop condition:

- Do not continue until the template scene runs.

## Stage 1: Create Core Data Types

Purpose:

- Define the evidence data model before building UI or interactions.

Files to create:

```text
Source/MumosaSpatial/Public/Data/MumosaEvidenceTypes.h
Source/MumosaSpatial/Private/Data/MumosaEvidenceTypes.cpp
```

Core types:

- `FMumosaSourceRecord`
- `FMumosaEvidenceMarkerRecord`
- `EMumosaConfidenceLevel`
- `EMumosaEvidenceStatus`

Suggested fields:

```text
MarkerId
Label
Summary
AIInterpretation
Confidence
Status
TimelineEvent
TimeRange
DiscrepancyNote
SuggestedNextStep
Sources[]
```

Source fields:

```text
SourceId
Title
SourceType
Timestamp
Attribution
FilePathOrUrl
Excerpt
```

Acceptance check:

- Project compiles.
- Types are Blueprint-visible where useful.
- No scene behavior yet.

Stop condition:

- Do not build markers until data types compile cleanly.

## Stage 2: Build Evidence Marker Actor

Purpose:

- Create clickable/selectable evidence markers in the world.

Files to create:

```text
Source/MumosaSpatial/Public/Evidence/MumosaEvidenceMarkerActor.h
Source/MumosaSpatial/Private/Evidence/MumosaEvidenceMarkerActor.cpp
```

Actor responsibilities:

- Store marker ID.
- Display marker label.
- Detect click/selection.
- Broadcast selection event.
- Highlight when selected.
- Allow selected/unselected visual state.

Suggested components:

- `USceneComponent* Root`
- `UStaticMeshComponent* MarkerMesh`
- `UTextRenderComponent* LabelText`
- Optional `USphereComponent* InteractionVolume`

Minimum functions:

```text
SetMarkerRecord(...)
GetMarkerId()
SetSelected(bool)
HandleClicked(...)
```

Acceptance check:

- At least three markers can be placed in the map.
- Labels are visible.
- Clicking a marker changes its visual state.
- Project compiles.

Stop condition:

- Do not build UI until marker selection works.

## Stage 3: Build Evidence Manager

Purpose:

- Centralize marker lookup and current selection.

Files to create:

```text
Source/MumosaSpatial/Public/Evidence/MumosaEvidenceManagerSubsystem.h
Source/MumosaSpatial/Private/Evidence/MumosaEvidenceManagerSubsystem.cpp
```

Recommended type:

- `UGameInstanceSubsystem` or `UWorldSubsystem`.

Responsibilities:

- Register/unregister markers.
- Store selected marker.
- Broadcast selection changes.
- Provide marker records by ID.
- Later load data from JSON/DataTable.

Minimum functions:

```text
RegisterMarker(AMumosaEvidenceMarkerActor*)
UnregisterMarker(AMumosaEvidenceMarkerActor*)
SelectMarkerById(FName MarkerId)
SelectMarker(AMumosaEvidenceMarkerActor*)
GetSelectedMarkerRecord()
```

Acceptance check:

- Clicking a marker updates manager selection.
- Only one marker is selected at a time.
- Logs show selected marker ID.

Stop condition:

- Do not connect UI until manager can identify selected marker.

## Stage 4: Create Static Sample Evidence Data

Purpose:

- Provide realistic prototype content without needing real MUMOSA media.

Files to create:

```text
Content/MUMOSA/Data/DT_MumosaEvidenceMarkers.uasset
```

Alternative for code-first:

```text
Content/MUMOSA/Data/east_palestine_demo_markers.json
```

Initial marker records:

- `ignition_zone`
- `smoke_drift`
- `railcar_cluster`
- `response_vehicle`
- `drone_pass_position`

Acceptance check:

- Marker labels and summaries come from data, not hardcoded text.
- At least one marker has `Medium` confidence.
- At least one marker has a discrepancy note.

Stop condition:

- Do not polish visuals until data can drive marker content.

## Stage 5: Build Evidence Source Panel UI

Purpose:

- Show source-grounded evidence details when a marker is selected.

Files to create:

```text
Source/MumosaSpatial/Public/UI/MumosaEvidencePanelWidget.h
Source/MumosaSpatial/Private/UI/MumosaEvidencePanelWidget.cpp
Content/MUMOSA/UI/WBP_EvidencePanel.uasset
```

Panel fields:

- Marker label.
- AI interpretation.
- Confidence.
- Status.
- Timeline event.
- Linked sources.
- Discrepancy note.
- Suggested next step.

Minimum behavior:

- Hidden by default.
- Opens/updates on marker selection.
- Close button hides panel.

Acceptance check:

- Click `ignition_zone`.
- Panel shows ignition details.
- Click `smoke_drift`.
- Panel updates to smoke details.
- Close button works.

Stop condition:

- Do not add timeline controls until panel reliably updates.

## Stage 6: Build Hazard Overlay System

Purpose:

- Show danger zones and uncertain/inferred areas in the scene.

Files to create:

```text
Source/MumosaSpatial/Public/Evidence/MumosaHazardZoneActor.h
Source/MumosaSpatial/Private/Evidence/MumosaHazardZoneActor.cpp
```

Actor responsibilities:

- Show/hide overlay.
- Store hazard ID.
- Store confidence/status.
- Use translucent material.

Initial hazard overlays:

- Smoke drift zone.
- Ignition candidate zone.
- Restricted/unsafe access zone.

UI:

- Add simple toggle: `Show Hazard Overlay`.

Acceptance check:

- Toggle shows/hides hazard zones.
- Hazard zones are visually distinct from evidence markers.
- User can still click markers.

Stop condition:

- Do not build timeline until overlay visibility can be controlled.

## Stage 7: Build Timeline State Controller

Purpose:

- Let users step through the event sequence spatially.

Files to create:

```text
Source/MumosaSpatial/Public/Timeline/MumosaTimelineStateController.h
Source/MumosaSpatial/Private/Timeline/MumosaTimelineStateController.cpp
```

Timeline states:

- `Before`
- `DerailmentOnset`
- `FireSmokeSpread`
- `ResponseContainment`

Responsibilities:

- Store current timeline state.
- Broadcast state changes.
- Show/hide markers by state.
- Show/hide hazard zones by state.
- Update UI label.

UI:

- Four buttons or one simple horizontal selector.

Acceptance check:

- Clicking timeline buttons changes visible markers/overlays.
- Current state label updates.
- Scene still supports marker selection after state changes.

Stop condition:

- Do not add dashboard handoff until timeline and markers work together.

## Stage 8: Build Dashboard Handoff Mock

Purpose:

- Simulate how the web dashboard would open the Unreal scene with context.

Files to create:

```text
Source/MumosaSpatial/Public/Integration/MumosaDashboardLaunchParams.h
Source/MumosaSpatial/Private/Integration/MumosaDashboardLaunchParams.cpp
```

Input fields:

```text
IncidentId
SceneId
FocusObjectId
TimeRange
Mode
```

Behavior:

- On game start, read mock launch params.
- Select/highlight `FocusObjectId`.
- Set matching timeline state if provided.
- Show a small "Opened from Dashboard" banner.

Simple first version:

- Hardcode launch params in a data asset or config file.

Better later version:

- Parse command-line args.

Acceptance check:

- Starting the level automatically highlights `ignition_zone`.
- UI explains that the scene was opened from the dashboard.
- Return button appears.

Stop condition:

- Do not implement real API calls yet.

## Stage 9: Build Return-To-Dashboard Mock

Purpose:

- Show that findings can return to the dashboard.

UI:

- Button: `Return To Dashboard`
- Button: `Flag Discrepancy`
- Optional text field or fixed note.

Output payload:

```json
{
  "selectedObjectId": "ignition_zone",
  "reviewStatus": "inspected",
  "flaggedDiscrepancy": true,
  "note": "Smoke direction conflicts with one witness statement."
}
```

Behavior:

- Log output payload.
- Show confirmation toast/panel.

Acceptance check:

- Clicking return prints or displays payload.
- Payload includes selected marker ID.
- User understands this would go back to dashboard.

Stop condition:

- Do not build real dashboard integration yet.

## Stage 10: Visual Blockout Improvement

Purpose:

- Make the scene credible enough for presentation.

Tasks:

- Replace major cubes with rail/industrial props.
- Add ground material.
- Add lighting.
- Add basic smoke/fire VFX.
- Add decals.
- Add responder vehicle placeholder.
- Add scene boundary.

Asset priority:

- Railcars.
- Tracks.
- Gravel/asphalt/concrete.
- Industrial tanks.
- Debris.
- Emergency vehicle.
- Hazard signs/tape.

Acceptance check:

- A viewer can identify the scene as an industrial/rail incident.
- Visuals support evidence review.
- Performance remains acceptable.

Stop condition:

- Do not chase photorealism before interaction is complete.

## Stage 11: Prototype Media Manifest

Purpose:

- Track substitute media used as source evidence.

Files to create:

```text
07_Prototype_Media/media_manifest.md
07_Prototype_Media/source_photos/
07_Prototype_Media/source_video/
07_Prototype_Media/processed_stills/
07_Prototype_Media/licenses_and_attribution/
```

Manifest fields:

```text
Asset ID
Filename
Source URL
Source organization
License/usage note
Prototype marker supported
Description
Attribution text
```

Acceptance check:

- Each source image/video used in UI has a manifest entry.
- Prototype clearly labels substitute media as representative.

Stop condition:

- Do not use random internet media without tracking source/usage.

## Stage 12: Point Cloud / Scan View Placeholder

Purpose:

- Represent the future LiDAR/photogrammetry/Gaussian splatting pipeline without requiring real reconstruction.

Options:

- Import a small sample point cloud.
- Use a stylized point-cloud material.
- Create a toggle that switches scene material to "scan view."
- Add explanation panel.

Acceptance check:

- User understands scan/digital twin concept.
- Scene labels measured vs inferred geometry.
- Prototype does not falsely claim real scan processing.

Stop condition:

- Keep this optional unless core interaction is complete.

## Stage 13: VR Readiness Pass

Purpose:

- Prepare for VR without breaking PC mode.

Tasks:

- Enable OpenXR.
- Add VR pawn in a separate map or mode.
- Add controller ray selection.
- Reuse same evidence marker selection interface.
- Add teleport locomotion.
- Add comfort settings.

Acceptance check:

- PC mode still works.
- VR selection can interact with markers.
- Evidence panel has a readable VR-friendly presentation.

Stop condition:

- Do not start this before PC marker/panel/timeline flow works.

## Stage 14: Testing And Fix Pass

Purpose:

- Validate that the prototype communicates the concept.

Test tasks:

- Walk through the scene.
- Select ignition zone.
- Identify source evidence.
- Toggle hazard overlay.
- Change timeline state.
- Return to dashboard.

Observe:

- Does user know what to click?
- Does user understand source grounding?
- Does user confuse VR/simulation with dashboard replacement?
- Does user understand confidence/discrepancy labels?

Acceptance check:

- Tester can explain the prototype in one sentence.
- Tester can verify at least one AI/spatial claim through source evidence.

## Stage 15: Presentation Build

Purpose:

- Freeze a stable version for class/demo.

Tasks:

- Create clean demo map.
- Remove broken/unused UI.
- Add title/instruction screen.
- Add reset button or restart key.
- Package Windows build if possible.
- Record backup walkthrough video.

Acceptance check:

- Demo can be run without the editor.
- If package fails, editor demo still works.
- Backup video exists.

## Work Packet Template For Smaller AI Models

Use this prompt shape when assigning one implementation stage:

```text
You are working on the MUMOSA Unreal spatial review prototype.
Implement only Stage X from Unreal_Simulation_Implementation_Plan.md.
Do not start later stages.
Preserve PC mouse/keyboard functionality.
Use C++ for core logic and Blueprints only as thin wrappers if required.
After changes, list files changed and explain how to verify the stage acceptance check.
```

## Minimum Viable Prototype Cut

If time gets tight, complete only:

1. Stage 0: Project setup.
2. Stage 1: Data types.
3. Stage 2: Evidence marker actor.
4. Stage 3: Evidence manager.
5. Stage 5: Evidence source panel.
6. Stage 6: Hazard overlay.
7. Stage 8: Dashboard handoff mock.
8. Stage 10: Visual blockout improvement.

This cut is enough to show the core idea:

> The dashboard opens a PC spatial review scene, the investigator clicks evidence markers, verifies source-grounded AI claims, sees hazards in physical context, and returns findings to the dashboard.

