# Axure Edit Guide: Revised MUMOSA Dashboard

Use this guide to update the Axure prototype from the older five-screen dashboard into the revised MUMOSA workflow.

## Target Flow

1. `Ask`
2. `Q/A Results`
3. `Compare Evidence`
4. `Timeline + Event Map`
5. `Simulation Evidence`

The prototype story should be:

`select incident/timeframe -> ask question -> inspect answer with sources -> compare evidence -> build timeline/event map -> inspect simulation evidence`

## Global Edits

### Navigation Labels

Replace the old navigation labels:

- `Incident Entry` -> `Ask`
- `Event Overview` -> `Q/A Results`
- `Evidence Review` -> `Compare Evidence`
- `Timeline + Schema` -> `Timeline + Event Map`
- `VR Scene Review` -> `Simulation Evidence`

### Header Subtitle

Use:

`Post-crisis investigation workspace for grounded Q/A, multimodal evidence review, event mapping, and simulation evidence.`

### Global Tags

Use these tags in the header:

- `Forensic investigator workflow`
- `Source-grounded AI`
- `SA levels 1-2`

## Page 1: Ask

Former page: `Incident Entry`

### Main Title

`Ask a grounded question about a complex event`

### Main Content

Left card: `Incident and timeframe`

Include:

- Incident: `East Palestine Train Derailment`
- Location/date: `Ohio, USA | February 3, 2023`
- Timeframe: `Feb. 3, 8:30 PM - Feb. 4, 12:00 AM`
- Data scope: `Preprocessed documents, images, schema, and simulation evidence`

Question card:

`What time did the derailment in East Palestine happen?`

Helper text:

`Natural-language Q/A is the primary entry. Suggested prompts come from validated Q/A pairs and related evidence.`

Right card: `Suggested Q/A prompts`

Prompt rows:

- `Was it hard to see at night during the initial response?`
- `Where is the immediate danger?`
- `What evidence supports the ignition sequence?`
- `Show lower-ranked answers for derailment time.`

### Hotspots

- On suggested question `What time did the derailment...` or the main question card: open `Q/A Results`
- Top nav tabs: open their matching pages

## Page 2: Q/A Results

Former page: `Event Overview`

### Main Title

`Answer first, evidence always attached`

### Top Answer Card

Header:

`System answer`

Badge:

`Top match 0.82`

Answer:

`About 50 cars derailed in East Palestine at about 9 p.m. EST Friday.`

Supporting text:

`Generated from a ranked textual answer pair and linked to source passages and visual evidence for verification.`

Actions:

- `Show other evidence`
- `Browse collection`
- `Compare sources`
- `Add to timeline`

### Left Panel: Textual Evidence

Rows:

- `0.82` / `News article passage` / `About 50 cars derailed at about 9 p.m. EST Friday.`
- `0.71` / `Alternate report wording` / `The Norfolk Southern train derailed around 9 p.m. Friday night.`
- `0.53` / `Lower relevance` / `Agency response count; related but not direct time answer.`

### Right Panel: Visual Evidence

Use the current dark image/placeholder area.

Labels:

- `night scene`
- `derailment aftermath`

Caption:

`Matched image descriptions can prompt follow-up questions about visibility, hazards, and response constraints.`

### Hotspots

- `Compare sources`: open `Compare Evidence`
- `Add to timeline`: open `Timeline + Event Map`
- Visual evidence panel: open `Compare Evidence`

## Page 3: Compare Evidence

Former page: `Evidence Review`

### Main Title

`Trace the answer back to source material`

### Left Panel: Text Source Passage

Header:

`Text source passage`

Badge:

`Highlighted answer`

Passage:

`... About 50 cars derailed in East Palestine at about 9 p.m. EST Friday. Emergency crews responded as smoke and fire became visible near the rail line ...`

Source metadata:

`Source: local news article | published Feb. 4, 2023 | retrieved from document collection`

### Right Panel: Visual Source

Header:

`Visual source`

Badge:

`Caption + VLM text`

Image labels:

- `fire/smoke visible`
- `source image`

Caption/source row:

`Visibility and smoke evidence saved to timeline draft`

### Bottom Card: Trust and Discrepancy Check

Text:

`AI answer is treated as a lead, not final truth. The investigator compares ranked textual answers, image descriptions, schema matches, and notes any conflict before adding the item to the event timeline.`

Actions:

- `Save note`
- `Add source to timeline`
- `Open related event node`
- `Show lower-ranked matches`

### Hotspots

- `Add source to timeline`: open `Timeline + Event Map`
- `Open related event node`: open `Timeline + Event Map`
- Nav tab `Simulation Evidence`: open `Simulation Evidence`

## Page 4: Timeline + Event Map

Former page: `Timeline + Schema`

This is the most important page to improve.

### Main Title

`Build the event narrative across sources`

### Timeline Card

Header:

`Incident timeline draft`

Badge:

`Investigator-built`

Timeline events:

- `8:58 PM` / `Derailment onset` / `Camera + report`
- `9:05 PM` / `Fire ignition` / `Witness conflict`
- `9:22 PM` / `Smoke expansion` / `Drone + thermal`
- `10:10 PM` / `Hazard zone widened` / `Memo + simulation`

### Event Map Card

Rename `Schema relationships` to:

`Event relationship map`

Badge:

`Schema-backed`

Legend chips:

- `Complex event`
- `Evidence match`
- `Conflict`
- `Pending`

Nodes:

- `Derailment` / `Selected complex event`
- `Ignition` / `Conflicting reports`
- `Smoke spread` / `Inferred relation`
- `Evacuation` / `Confirmed memo`
- `Witness report` / `Time discrepancy`

Connectors:

- `Derailment -> Ignition`
- `Derailment -> Smoke spread`
- `Ignition -> Smoke spread`
- `Smoke spread -> Evacuation`
- `Witness report -> Ignition`

### Selected Node Details Panel

Header:

`Selected node details`

Badge:

`Derailment`

Tabs:

- `Evidence`
- `Roles`
- `Notes`

Body:

`The selected complex event anchors the timeline and links to matched source phrases, participant roles, and location evidence.`

Rows:

- `A0` / `Agent/causer` / `Norfolk Southern train / rail system`
- `A1` / `Affected entity` / `Railcars, residents, nearby environment`
- `Loc` / `Location` / `East Palestine, Ohio rail line`
- `Watch` / `Discrepancy` / `Witness timing conflicts with ignition sequence`

### Hotspots

- Timeline nodes: open or highlight related detail panel
- Event map nodes: update selected node details if you have time; otherwise leave static
- `Simulation Evidence` nav tab: open `Simulation Evidence`

## Page 5: Simulation Evidence

Former page: `VR Scene Review`

### Main Title

`Use simulation as an optional spatial review tool`

### Left Panel: 3D Reconstruction View

Header:

`3D reconstruction view`

Badge:

`Simulation evidence`

Scene labels:

- `Danger zone`
- `Train car cluster`
- `Buildings at risk`
- `Responder route`

### Right Panel: Question-Driven Annotation

Header:

`Question-driven annotation`

Badge:

`Not front-door VR`

Body:

`The user asks where immediate danger is located. The simulation highlights train car clusters and nearby buildings, while the dashboard keeps linked sources visible.`

Rows:

- `Source` / `Drone image sequence` / `24 reference images reconstruct bird-eye scene`
- `Mask` / `Semantic highlight` / `Orange/red masks identify immediate danger region`
- `Training` / `Responder value` / `Supports route planning and after-action learning`

Action:

`Return to evidence comparison`

### Hotspots

- `Return to evidence comparison`: open `Compare Evidence`
- Labels in scene: optional hotspot to selected source row

## Minimum Working Prototype

If time is tight, make only these interactions:

1. `Ask` suggested question -> `Q/A Results`
2. `Q/A Results` `Compare sources` -> `Compare Evidence`
3. `Compare Evidence` `Add source to timeline` -> `Timeline + Event Map`
4. `Timeline + Event Map` nav -> `Simulation Evidence`
5. `Simulation Evidence` `Return to evidence comparison` -> `Compare Evidence`

## Critique Defense

Use this wording if asked why the prototype changed:

`We revised the dashboard to better match the MUMOSA paper. The core contribution is not a generic AI summary dashboard; it is a source-grounded multimodal Q/A workspace where investigators ask questions, compare textual and visual evidence, use schema-backed event maps to build chronology, and optionally inspect simulation evidence for spatial understanding.`
