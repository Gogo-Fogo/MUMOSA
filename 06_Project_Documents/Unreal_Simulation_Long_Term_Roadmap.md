# Unreal Simulation Long-Term Roadmap

This roadmap assumes the team chooses Unreal Engine for the MUMOSA spatial simulation module. The dashboard remains a separate web-first system built by others. This roadmap focuses on the PC/VR simulation layer.

## Product Position

The simulation module is not a standalone game and not a replacement for the MUMOSA dashboard.

It is a source-grounded spatial review tool that lets investigators walk through a reconstructed incident scene, inspect AI-labeled evidence in context, verify sources, understand hazards, and return findings to the dashboard.

## Guiding Principles

1. PC-first, VR-ready.
2. Source-grounded evidence before visual spectacle.
3. Dashboard remains primary.
4. Unreal is used for spatial realism, scene review, and future digital-twin workflows.
5. AI tools accelerate development, but the core system stays code-first and understandable.
6. Build in layers so each milestone produces something presentable.

## Working Tool Stack

Core:

- Unreal Engine 5.7.x.
- C++ code-first project structure.
- First Person Template for PC navigation.
- Enhanced Input.
- UMG for source/evidence panels.
- JSON or DataTables for evidence data.
- Niagara for smoke, fire, hazard, and uncertainty overlays.
- Fab/Megascans for realistic environment props and materials.

Optional / later:

- LiDAR Point Cloud plugin.
- OpenXR and VR Template components.
- Python Editor Script Plugin.
- MCP bridge for editor automation.
- Pixel Streaming for future browser-based high-fidelity access.
- RealityCapture or other photogrammetry tools for real reconstructed scenes.

AI workflow:

- Codex/OpenCode for code generation, docs, debugging, data schemas, and architecture.
- MCP/editor bridge only after the basic project exists.
- NWIRO/Rekall/StraySpark-style tools as optional accelerators, not core dependencies.

## Milestone 0: Project Setup And Decision Lock

Goal:

- Establish the Unreal project and avoid tool sprawl.

Deliverables:

- Unreal 5.7.x installed.
- New C++ project created from First Person Template.
- Folder structure established.
- Version control initialized if desired.
- Initial README inside the Unreal project.
- Decision note confirming PC-first, VR-ready direction.

Success criteria:

- Project opens cleanly.
- First-person movement works.
- We know where source files, maps, UI, materials, data, and docs live.

Recommended folders:

```text
Source/MumosaSpatial/
Content/MUMOSA/Maps/
Content/MUMOSA/UI/
Content/MUMOSA/Blueprints/
Content/MUMOSA/Materials/
Content/MUMOSA/Data/
Content/MUMOSA/Props/
Content/MUMOSA/VFX/
Content/MUMOSA/PointClouds/
```

## Milestone 1: Walkable Spatial Review Blockout

Goal:

- Build the smallest playable scene that communicates "investigator walks through a reconstructed incident."

Deliverables:

- One walkable level.
- Basic industrial/rail/disaster blockout.
- Player spawn.
- Simple lighting.
- Navigation boundaries.
- Placeholder scene labels.

Recommended scene:

- Rail/industrial incident inspired by East Palestine.
- Railcar cluster.
- Smoke/ignition area.
- Response vehicle area.
- Bodycam/drone source positions.

Success criteria:

- A user can walk around with mouse and keyboard.
- The scene has enough landmarks to support evidence placement.
- It feels like a review environment, not an empty game map.

## Milestone 2: Evidence Marker System

Goal:

- Make source-grounded spatial evidence the core interaction.

Deliverables:

- `EvidenceMarkerActor` C++ class.
- Marker label.
- Marker ID.
- Click/select interaction.
- Highlight selected marker.
- Marker data structure.
- Test markers placed in the level.

Example marker types:

- Ignition zone.
- Smoke drift.
- Railcar cluster.
- Response vehicle.
- Drone pass path.
- Bodycam position.

Success criteria:

- User can select markers in the scene.
- Selected marker visibly changes state.
- The marker has structured evidence data behind it.

## Milestone 3: Evidence Source Panel

Goal:

- Show that every AI/spatial claim connects back to source evidence.

Deliverables:

- UMG evidence panel.
- Marker title.
- AI interpretation.
- Linked sources.
- Confidence status.
- Timestamp/time range.
- Related timeline event.
- Discrepancy warning field.
- Close/back behavior.

Example selected marker:

```text
Selected object: Ignition zone
AI interpretation: Possible origin connected to smoke spread sequence.
Linked sources: Drone pass 2, camera 05, official fire log.
Confidence: Medium
Timeline event: 9:05 PM fire ignition
Suggested next step: Compare west-zone timeline overlay.
```

Success criteria:

- Clicking a marker opens or updates the evidence panel.
- The panel makes the source-grounded trust model obvious.
- At least one marker shows medium confidence or a discrepancy.

## Milestone 4: JSON/Data-Driven Evidence

Goal:

- Make marker content editable without hardcoding every object.

Deliverables:

- Evidence JSON or DataTable format.
- Loader/manager class.
- Marker lookup by ID.
- Sample incident data file.
- Clear schema for dashboard handoff later.

Example data:

```json
{
  "incidentId": "east_palestine_demo",
  "markers": [
    {
      "id": "ignition_zone",
      "label": "Ignition Zone",
      "confidence": "Medium",
      "timelineEvent": "9:05 PM Fire Ignition",
      "sources": ["Drone pass 2", "Camera 05", "Official fire log"],
      "summary": "Possible origin connected to smoke spread sequence.",
      "discrepancy": "Witness report places first visible flame two minutes later."
    }
  ]
}
```

Success criteria:

- Changing the data file changes marker details.
- Evidence content is separate from visual scene placement.
- The system is ready for future dashboard/API integration.

## Milestone 5: Hazard And Uncertainty Overlays

Goal:

- Demonstrate that simulation helps users understand danger and uncertainty spatially.

Deliverables:

- Toggleable hazard overlay.
- Smoke/fire/danger zone visual.
- Confidence color system.
- Uncertainty or inferred-area styling.
- Legend or UI explanation.

Recommended states:

- Confirmed evidence.
- Inferred evidence.
- Conflicting evidence.
- Pending/unverified evidence.

Success criteria:

- User can toggle overlays on/off.
- Hazards do not obscure evidence.
- Realism does not imply certainty.

## Milestone 6: Timeline State Controller

Goal:

- Let users understand event sequence in the scene.

Deliverables:

- Timeline UI.
- Four state buttons or slider positions:
  - Before.
  - Derailment onset.
  - Fire/smoke spread.
  - Response/containment.
- Scene changes per state.
- Marker visibility or highlight changes per state.

Success criteria:

- The user can move through time.
- The scene supports chronology, not just location.
- Timeline events match the dashboard planning docs.

## Milestone 7: Dashboard Handoff Mock

Goal:

- Show how the dashboard would launch the simulation with context.

Deliverables:

- Simulated launch parameters.
- Focus marker support.
- "Return to Dashboard" button.
- Mock return payload.
- Documentation of handoff contract.

Example input:

```json
{
  "incidentId": "east_palestine_demo",
  "sceneId": "west_yard_reconstruction",
  "focusObjectId": "ignition_zone",
  "timeRange": "20:58-22:10",
  "mode": "desktop"
}
```

Example return:

```json
{
  "selectedObjectId": "ignition_zone",
  "reviewStatus": "inspected",
  "flaggedDiscrepancy": true,
  "note": "Smoke direction conflicts with one witness statement."
}
```

Success criteria:

- Prototype can start with a selected marker highlighted.
- User understands the simulation was opened from dashboard evidence.
- Return behavior is visible even if not connected to a real dashboard yet.

## Milestone 8: Visual Fidelity Pass

Goal:

- Improve the scene enough to feel credible without losing time to asset hunting.

Deliverables:

- Replace key blockouts with realistic props.
- Add environment materials.
- Add lighting pass.
- Add decals: scorch marks, dirt, hazard tape, cracks, labels.
- Add simple smoke/fire/dust effects.
- Add camera/post-process settings.

Asset priorities:

- Industrial/rail props.
- Debris/rubble.
- Asphalt/concrete/gravel.
- Emergency vehicle placeholder.
- Hazard signage.
- Smoke/fire VFX.

Success criteria:

- The prototype reads as a reconstructed crisis/industrial site.
- Realism supports investigation instead of distracting from it.

## Milestone 9: Point Cloud / Digital Twin Demonstration

Goal:

- Connect the prototype to the LiDAR/photogrammetry/Gaussian splatting concept without overpromising.

Deliverables:

- Import a small point cloud sample if available.
- Or create a "scan view" mode that visually simulates point cloud/digital twin evidence.
- Label measured vs inferred geometry.
- Add explanation panel for reconstruction source.

Success criteria:

- Viewer understands how future drones/robots/bodycams/LiDAR could feed the spatial scene.
- The prototype does not claim to perform real reconstruction unless it actually does.

## Milestone 10: VR Readiness

Goal:

- Prepare the same scene for VR interaction.

Deliverables:

- OpenXR enabled.
- VR pawn or VR mode branch.
- Controller ray selection for markers.
- Teleport movement.
- Comfort settings.
- Seated/standing mode consideration.

Success criteria:

- Existing evidence marker system can be selected in VR.
- VR does not require rebuilding the whole interaction model.
- PC remains fully supported.

## Milestone 11: User Testing And Iteration

Goal:

- Test whether the simulation helps people understand evidence better.

Test questions:

- Can users explain what the simulation is for?
- Can users find and inspect source-grounded evidence?
- Do users understand confidence and uncertainty?
- Does the timeline state controller help or confuse?
- Does the simulation feel connected to the dashboard?
- Does VR/3D feel useful, optional, or distracting?

Deliverables:

- Short testing script.
- Observation notes.
- List of issues.
- Revised prototype changes.

Success criteria:

- Users can explain that the simulation is spatial evidence review, not the whole product.
- Users can verify at least one AI claim through source-linked evidence.

## Milestone 12: Future Production Architecture

Goal:

- Define how this could become a real module after the class prototype.

Future components:

- Web dashboard launches scene with incident/object/time context.
- Cloud object storage stores media, reports, scans, and reconstruction files.
- Metadata/API layer serves evidence records.
- Signed URLs secure source footage.
- Edge compute supports low-latency active response only when needed.
- Pixel Streaming allows high-fidelity Unreal access in browser.
- Native desktop/VR app supports maximum fidelity when installed.

Production questions:

- Who owns source evidence?
- How is chain of custody maintained?
- Which media can be shown to which users?
- How are uncertain AI inferences labeled?
- What happens offline?
- What is the minimum viable data package for a scene?

Success criteria:

- The prototype has a credible path to real deployment without pretending the class build solves everything.

## Suggested Build Timeline

### Fast Class Prototype

Week 1:

- Milestones 0-2.
- Project setup, walkable blockout, markers.

Week 2:

- Milestones 3-5.
- Evidence panel, data-driven markers, hazard overlays.

Week 3:

- Milestones 6-8.
- Timeline states, dashboard handoff mock, visual pass.

Week 4:

- Milestones 9 and 11.
- Point-cloud/digital-twin demonstration, testing, presentation polish.

### Longer Technical Prototype

Month 1:

- PC-first source-grounded spatial review.

Month 2:

- Better data model, dashboard handoff, improved environment, scan demo.

Month 3:

- VR mode, testing, performance cleanup, clearer architecture.

Month 4+:

- Real asset pipeline, cloud integration, streaming, collaboration, and production security model.

## Definition Of Done For The First Strong Prototype

The first strong version is done when a user can:

1. Launch into a PC walkthrough scene.
2. See a reconstructed crisis environment.
3. Click at least five evidence markers.
4. Read source-grounded AI interpretation for each marker.
5. Notice confidence/discrepancy labels.
6. Toggle a hazard overlay.
7. Step through at least three timeline states.
8. Understand that the dashboard opened this spatial review.
9. Return to the dashboard conceptually.
10. Explain why the simulation is useful and when it should not be used.

