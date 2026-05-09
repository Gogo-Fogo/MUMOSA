# Unreal Today MVP Build Plan

This plan is for building a simple but scalable MUMOSA Unreal proof-of-concept today, so it can be used tomorrow during user research alongside the new dashboard.

## Today Goal

Build a PC-first Unreal prototype that demonstrates:

```text
Dashboard handoff -> spatial scene review -> clickable evidence markers -> source-grounded details -> hazard/timeline context -> return/dashboard concept
```

This does not need to be visually perfect. It needs to clearly prove the interaction model and provide infrastructure we can extend later.

## Tomorrow User Research Goal

Participants should understand:

- The dashboard is the primary MUMOSA workspace.
- The Unreal prototype is an optional spatial review module.
- The user enters the simulation from a specific evidence item or timeline event.
- The simulation helps users understand evidence in physical context.
- Every marker is tied to source evidence, confidence, and possible discrepancy.
- PC/mouse-keyboard mode works even without VR hardware.

## MVP Scope

Must build today:

- Walkable first-person scene.
- 5 evidence markers.
- Click/select marker behavior.
- Evidence detail panel.
- Hazard overlay toggle.
- Simple timeline state selector.
- Mock dashboard handoff state.
- Return-to-dashboard button or ending screen.

Should build if time allows:

- Better industrial/rail visual dressing.
- Source thumbnails using prototype media.
- Intro screen explaining the relationship to the dashboard.
- Packaged Windows build.
- Backup walkthrough video.

Do not build today:

- Real VR.
- Real backend.
- Real AI inference.
- Real MUMOSA media ingestion.
- Pixel Streaming.
- Multiplayer/collaboration.
- Full point cloud pipeline.
- Complex photorealistic environment.

## Recommended Build Order

### Phase 1: Project Baseline

Target time:

- 30 to 60 minutes.

Tasks:

- Create/open Unreal 5.7.x C++ project from First Person template.
- Confirm mouse/keyboard movement works.
- Create `Content/MUMOSA/` folders.
- Create one level: `MUMOSA_SpatialReview_Demo`.
- Add simple rail/industrial blockout.

Acceptance check:

- User can walk around the scene.
- Scene has clear zones: railcar cluster, ignition zone, smoke drift, response vehicle, drone/bodycam area.

### Phase 2: Evidence Marker System

Target time:

- 60 to 90 minutes.

Tasks:

- Create reusable marker actor.
- Add marker ID and label.
- Add click selection.
- Add selected visual state.
- Place 5 markers:
  - `ignition_zone`
  - `smoke_drift`
  - `railcar_cluster`
  - `response_vehicle`
  - `sampling_zone`

Acceptance check:

- Clicking each marker visibly selects it.
- Only one marker is selected at a time.

### Phase 3: Evidence Panel

Target time:

- 60 to 90 minutes.

Tasks:

- Create UMG panel.
- Show selected marker details.
- Include:
  - marker title
  - AI interpretation
  - confidence
  - linked sources
  - timeline event
  - discrepancy note
  - suggested next step

Acceptance check:

- Selecting each marker updates the panel.
- At least one marker has medium confidence.
- At least one marker has discrepancy text.

### Phase 4: Hazard Overlay

Target time:

- 30 to 60 minutes.

Tasks:

- Add translucent danger/smoke/uncertainty volumes.
- Add UI button: `Toggle Hazard Overlay`.
- Use different color or material for confirmed vs inferred regions.

Acceptance check:

- User can toggle hazard overlay.
- Overlay supports the scene rather than blocking it.

### Phase 5: Timeline States

Target time:

- 45 to 75 minutes.

Tasks:

- Add simple timeline UI with 4 buttons:
  - `Before`
  - `Derailment`
  - `Smoke Spread`
  - `Response`
- Change marker or overlay visibility by state.
- Show current timeline label.

Acceptance check:

- Timeline buttons visibly change the scene or marker emphasis.
- The user can understand event progression.

### Phase 6: Dashboard Handoff Mock

Target time:

- 30 to 60 minutes.

Tasks:

- Add opening banner:

```text
Opened from MUMOSA Dashboard
Focus: Ignition Zone
Question: What evidence supports the ignition sequence?
```

- Start with `ignition_zone` highlighted.
- Add `Return to Dashboard` button.
- Return button shows a mock payload or ending screen.

Acceptance check:

- User understands the Unreal scene was launched from dashboard evidence.
- User understands this module sends findings back.

### Phase 7: Presentation/User Research Polish

Target time:

- 60 to 120 minutes.

Tasks:

- Add title/instructions screen.
- Add basic scene labels.
- Add a reset key or restart button.
- Add source thumbnails if available.
- Package Windows build if possible.
- Record fallback video walkthrough.

Acceptance check:

- Prototype can be demonstrated tomorrow even if packaging fails.
- A participant can use it with minimal explanation.

## Data Model For Today

Use hardcoded data first if needed, but structure it like JSON so it can be migrated later.

```json
{
  "incidentId": "rail_yard_demo",
  "sceneId": "spatial_review_demo",
  "focusObjectId": "ignition_zone",
  "markers": [
    {
      "id": "ignition_zone",
      "label": "Ignition Zone",
      "confidence": "Medium",
      "status": "Inferred",
      "timelineEvent": "Smoke Spread",
      "aiInterpretation": "AI suggests this area may be connected to the ignition sequence.",
      "sources": ["NTSB b-roll frame", "EPA cleanup photo", "Mock fire log excerpt"],
      "discrepancy": "Witness timing places visible flame two minutes later.",
      "suggestedNextStep": "Compare this marker against the timeline overlay."
    }
  ]
}
```

## Evidence Marker Content For Today

### Ignition Zone

- Confidence: Medium.
- Status: Inferred.
- Source examples: NTSB b-roll frame, EPA cleanup photo, mock fire log.
- Discrepancy: Witness timing conflict.

### Smoke Drift

- Confidence: Medium.
- Status: Inferred.
- Source examples: drone/UAS frame, air monitoring note.
- Discrepancy: Wind direction not fully verified.

### Railcar Cluster

- Confidence: High.
- Status: Confirmed.
- Source examples: EPA rail cleanup photo, track damage photo.
- Discrepancy: none.

### Response Vehicle

- Confidence: High.
- Status: Confirmed.
- Source examples: responder photo, mock dispatch note.
- Discrepancy: exact arrival time pending.

### Sampling Zone

- Confidence: High.
- Status: Confirmed.
- Source examples: EPA sampling photo, cleanup documentation.
- Discrepancy: sample collection time pending.

## Dashboard Link Options For Tomorrow

Use whichever is fastest and least fragile.

### Option A: Downloadable Windows Build

Best for:

- Highest fidelity.
- Reliable Unreal performance.
- No browser limitations.

How it works:

- Package the Unreal project as Windows executable.
- Dashboard includes a button/link: `Open Spatial Review`.
- During testing, facilitator opens the executable manually or through a local shortcut.

Risk:

- Participants may not be able to run it on their own machine.
- Packaging can fail under time pressure.

Recommendation:

- Best for tomorrow if testing is on your laptop.

### Option B: Itch.io Download Page

Best for:

- Simple distribution.
- Easy link from dashboard.
- Participants can download the build.

Risk:

- Still requires local download.
- Windows security warnings may appear.
- Upload time can be annoying.

Recommendation:

- Good if you want a public/shareable prototype page.

### Option C: WebGL / Browser Build

Best for:

- Dashboard can link directly to browser-based prototype.

Risk:

- Unreal does not have the same easy WebGL path Unity has.
- Not recommended for today.

Recommendation:

- Do not use for this Unreal prototype today.

### Option D: Pixel Streaming

Best for:

- Future high-fidelity browser access.

Risk:

- Too much setup for today.
- Requires server/GPU/network configuration.

Recommendation:

- Future architecture only.

### Option E: Local Protocol / Deep Link

Best for:

- Future realistic dashboard-to-app launch.

Example:

```text
mumosa://scene-review?incident=rail_yard_demo&focus=ignition_zone
```

Risk:

- Requires app installer/protocol registration.
- Not worth doing today.

Recommendation:

- Document it, do not implement today.

## Recommended Link Strategy For Tomorrow

Use a simple staged handoff:

1. Dashboard button says `Open Spatial Review`.
2. For the research session, the facilitator opens the Unreal packaged build or editor demo.
3. Unreal starts already focused on the ignition zone.
4. Participant uses the Unreal scene.
5. Participant clicks `Return to Dashboard`.
6. Facilitator returns them to the dashboard prototype.

This is enough for user research because the research question is whether the handoff and simulation concept make sense, not whether deployment plumbing is finished.

## Backup Plan

If Unreal packaging fails:

- Run from the Unreal editor.
- Screen share the editor viewport.
- Record a walkthrough video.
- Use the video during research as a fallback.

If marker clicking fails:

- Use keyboard keys `1-5` to select markers.
- Keep the evidence panel working.

If timeline fails:

- Use static labels and one hazard overlay.

If UI fails:

- Use in-world text panels near each marker.

## Definition Of Done For Tonight

The prototype is ready for tomorrow if:

1. It opens reliably on your machine.
2. Mouse/keyboard movement works.
3. User can inspect at least 5 markers.
4. Selecting a marker shows source-grounded details.
5. At least one confidence/discrepancy example is visible.
6. Hazard overlay can be shown or explained.
7. Dashboard handoff is visible through banner/start state.
8. Return-to-dashboard concept is visible.
9. You have a fallback video or screenshots.

