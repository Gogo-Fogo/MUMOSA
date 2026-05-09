# VR And Simulation Rationale

## Short Thesis

VR should not replace the MUMOSA dashboard. It should act as a specialized "power tool" for spatial evidence review, training, and forensic preservation when a 2D dashboard cannot explain the scene clearly enough.

## Problem

Post-crisis investigators need to understand what happened, where it happened, when it happened, and how different evidence sources connect. The problem is not just missing data. The problem is cognitive overload from fragmented sources, stress, time pressure, and difficult physical environments.

Investigators may need to compare:

- Body-camera footage.
- Drone imagery.
- Official reports.
- Witness statements.
- Hazard records.
- Schema graph relationships.
- Simulation outputs.
- Reconstructed scene geometry.

## Solution

The prototype can frame VR as a spatial workspace connected to the main MUMOSA dashboard.

Instead of treating the schema graph as only a flat diagram, the VR mode lets investigators walk through a reconstructed crisis site and inspect evidence markers in place. The user can select an object, hazard, zone, or timeline event and see the source material behind it.

## Cognitive Goal

Responders and investigators operate with a drained mental battery during and after crisis events. Spatial memory can reduce friction by making abstract AI summaries more concrete.

The VR mode helps users:

- Remember where evidence occurred.
- Understand spatial relationships between hazards, people, vehicles, and event zones.
- Reduce overload from long text summaries.
- Build familiarity with a scene before training or review.
- Discuss a scene with remote experts in a shared space.

## Trust And Source Verification

The key trust feature is not "AI tells the answer." The key trust feature is "show source."

The prototype should demonstrate that an investigator can select an AI-generated event, hazard, or object and immediately inspect the source evidence behind it.

Examples:

- Click "ignition zone" and see the linked drone pass, camera angle, or fire report.
- Click "smoke drift" and see the source confidence and related timeline event.
- Click a floating body-camera marker and open the relevant video moment.
- Compare an AI summary with the official log or responder feed that supports it.

## Evidence Capture Pipeline

The board screenshots suggest a realistic future pipeline for building spatial scenes:

1. Drones, robots, body cameras, and photographs collect overlapping visual data.
2. Photogrammetry, Gaussian Splatting, or LiDAR reconstruct the physical scene.
3. GPS and IMU metadata place 2D video feeds into 3D coordinates.
4. Vision-language models extract key events, hazards, witness statements, and objects.
5. MUMOSA turns those extracted items into clickable spatial markers.
6. The investigator reviews each marker with links back to source evidence.

## Why Gaussian Splatting And LiDAR Matter

Gaussian Splatting:

- Can turn overlapping photos or quick video walkthroughs into realistic 3D scenes.
- Helps create a visually understandable digital twin of a crisis site.
- Supports later forensic review after the original scene has changed or been cleaned up.

LiDAR:

- Adds measurement precision.
- Supports physical questions like distances, ledge heights, object placement, skid marks, and spatial paths.
- Helps investigators navigate angles that were not directly recorded in a normal photo or video.

## Safety And Forensic Preservation

VR has value because crisis sites are dangerous and temporary.

Safety value:

- Investigators can explore unsafe areas without physical exposure.
- Remote experts can review a site without needing to fly in immediately.
- Responders can train in a familiar simulated environment before entering a risky scene.

Forensic preservation value:

- A digital twin can freeze the scene before cleanup, weather, repairs, or human movement change the evidence.
- Investigators can revisit the scene months later to check details missed during the initial response.

## Practicality Risks

VR also has real limitations that the project should name clearly.

Risks:

- High hardware cost.
- Motion sickness or accessibility issues.
- Need for strong data processing and network capacity.
- Heavy 3D point-cloud or splat files.
- Risk of distracting from the core dashboard workflow.

## Mitigation Strategy

The mitigation is a web-first ecosystem.

Primary experience:

- Standard accessible web dashboard.
- Works without a headset.
- Supports search, Q/A, source review, timeline, and evidence comparison.

Optional immersive mode:

- VR only when spatial review is useful.
- Mobile AR can bridge 2D and 3D on tablets or phones.
- Headsets are treated as advanced equipment, not required equipment.

## What The Prototype Should Prove

The prototype should prove that the dashboard and VR mode can work together:

- The dashboard gives the user overview, search, chronology, and source-grounded reasoning.
- The VR mode gives the user spatial context, hazard interpretation, and physical scene review.
- The handoff between them feels deliberate, not random.
- Every AI-supported claim has visible source evidence.

