# Dashboard To VR User Flow

This document defines the prototype flow for moving from the MUMOSA dashboard into the optional VR scene review mode.

## Design Principle

The dashboard stays primary. VR is entered only when the investigator needs spatial understanding, scene reconstruction, hazard review, or training context.

## Entry Points

The VR handoff can appear from these dashboard moments:

- Event overview: "Open reconstructed scene."
- Evidence detail: "View this evidence in scene."
- Timeline and schema view: "Inspect event location in VR."
- Hazard card: "Review hazard zone spatially."

## Recommended Flow

1. The investigator starts in the standard MUMOSA dashboard.
2. The investigator asks a question or opens an incident.
3. MUMOSA shows an AI summary with source cues, evidence cards, and a next step.
4. The investigator selects a spatial item, such as "ignition zone," "smoke drift," "railcar cluster," or "response vehicle."
5. The dashboard shows a transition screen explaining what VR will add.
6. If a headset is connected, the user can launch VR scene review.
7. If no headset is connected, the user can pair a device, continue in desktop 3D preview, or open mobile AR.
8. In VR, the user inspects spatial markers and source-linked evidence.
9. The user returns to the dashboard with selected findings or notes carried back into the evidence/timeline view.

## Intermediate Handoff Screen

The handoff screen is important because it explains why the user is leaving the normal dashboard.

Suggested title:

> Open Spatial Scene Review

Suggested copy:

> MUMOSA detected that this evidence has spatial context. Use VR when you need to walk through the reconstructed scene, inspect hazard zones, or understand where evidence sits in physical space.

Primary action:

- Launch VR Scene Review

Secondary actions:

- Pair Headset
- Continue In Desktop Preview
- Send To Mobile AR
- Stay In Dashboard

Status states:

- Headset connected.
- No headset detected.
- Pairing in progress.
- Scene model loading.
- Source links available.
- Desktop preview available.

## Device Pairing Scenario

If the user does not have a headset connected, the prototype should not fail. It should show a calm fallback path.

Suggested copy:

> No VR device is currently connected. You can pair a headset, open a desktop 3D preview, or continue reviewing the same evidence in the dashboard.

Prototype actions:

- "Pair Device" opens a mock pairing panel.
- "Desktop Preview" opens a non-VR scene view.
- "Back To Dashboard" returns to the evidence detail screen.

## VR Scene Review Screen

The VR scene review page should show:

- Reconstructed scene viewport.
- Spatial labels for key evidence.
- Selected object panel.
- Linked source list.
- Confidence or uncertainty indicator.
- Suggested next step.
- Return-to-dashboard action.

Example spatial labels:

- Smoke drift.
- Ignition zone.
- Railcar cluster.
- Response vehicle.
- Body-camera position.
- Drone pass path.

Example selected object panel:

- Object: Ignition zone.
- AI interpretation: Possible start point connected to smoke spread sequence.
- Linked sources: Drone pass 2, camera 05, official fire log.
- Confidence: Medium.
- Suggested next step: Compare west-zone timeline overlay or inspect report discrepancy.

## What Axure Should Demonstrate

The Axure prototype does not need real VR. It needs to demonstrate the decision flow.

Required clickable moments:

- Dashboard evidence card opens the VR handoff screen.
- Handoff screen opens the VR scene review mockup.
- "No device detected" opens pairing/fallback state.
- Selecting a spatial marker changes the selected object panel.
- Return action brings the user back to the dashboard evidence or timeline screen.

## Success Criteria

The flow is successful if a first-time viewer understands:

- Why VR exists.
- When VR should be used.
- What evidence is being reviewed in VR.
- How the VR scene stays connected to source-grounded dashboard evidence.
- How the user can continue without a headset.

