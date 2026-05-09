# Revised Figma Dashboard Plan

## Purpose

Re-approach the Figma dashboard so it aligns more tightly with the MUMOSA source paper. The prototype should show MUMOSA as an interactive multi-modal Q/A workspace, not just a polished crisis dashboard.

The key interaction to demonstrate is:

`select incident/timeframe -> ask question -> inspect answer -> compare text and visual evidence -> build event timeline/schema understanding -> optionally inspect simulation evidence`

## Design Position

### Primary User

Post-crisis investigator, fire investigator, CSI-style analyst, or crisis analysis unit member.

### Primary Use Case

Forensic reconstruction of a complex incident using grounded evidence across text, images, schema graphs, and 3D/simulation data.

### Secondary Use Case

Responder training, especially using simulation/VR evidence to understand danger zones, route planning, and scene layout.

### Future Direction Only

Real-time crisis response, live sensor fusion, agentic alerts, and fully dynamic incoming streams. These can be hinted at, but should not drive the current prototype.

## Source Alignment

The MUMOSA paper organizes the dashboard around four evidence panels:

- Textual evidence
- Visual evidence
- Schema evidence
- Simulation evidence

It also describes the dashboard as supporting situational awareness:

- Level 1: perceive essential elements of information
- Level 2: understand relationships and event sequence
- Level 3: project future states, mostly discussed as future work

The Figma prototype should emphasize Levels 1 and 2.

## Revised Screen Flow

### 1. Incident + Question Entry

Former screen: `Incident Entry`

Goal: make the starting point feel like MUMOSA's actual entry model.

Core elements:

- Incident selector set to `East Palestine Train Derailment`
- Date/timeframe selector
- Natural-language question input
- Suggested pre-generated Q/A prompts
- Recent investigations, visually secondary
- Optional filters as refinement after entry, not front-door blockers

Example prompts:

- `What time did the derailment in East Palestine happen?`
- `Was it hard to see at night during the initial response?`
- `Where is the immediate danger?`
- `What evidence supports the ignition sequence?`

Design note:

This screen should make it obvious that the user starts with an incident and question, not with a role picker.

### 2. Q/A Results Workspace

Former screen: `Event Overview`

Goal: make this the central dashboard screen.

Core elements:

- User question displayed at top
- Answer card with concise answer
- Match confidence or ranked-answer score
- Textual Evidence panel
- Visual Evidence panel
- Source metadata: title, date, source type, relevance
- Actions:
  - `Show other evidence`
  - `Browse collection`
  - `Compare sources`
  - `Add to timeline`

Example answer:

`About 50 cars derailed in East Palestine at about 9 p.m. EST Friday.`

Design note:

This should feel like the main product, not a summary page. The answer is useful because it is backed by visible evidence.

### 3. Evidence Comparison

Former screen: `Evidence Review`

Goal: show how users verify and compare claims.

Core elements:

- Left side: textual source passage with highlighted answer
- Right side: visual evidence/image with caption or generated description
- Source chain linking text, image, and related schema node
- Ranked evidence list
- Discrepancy callout
- Investigator notes/annotation area
- Actions:
  - `Save note`
  - `Add source to timeline`
  - `Open schema node`
  - `Show lower-ranked matches`

Design note:

This screen should support trust and forensic reasoning. It should not present AI as final truth; it should show how the user verifies the AI-supported answer.

### 4. Timeline + Event Map

Former screen: `Timeline + Schema`

Goal: make the schema usable and critique-resistant.

Core elements:

- Horizontal timeline with event sequence
- Event relationship map, not only a vague schema graph
- Selected event node detail panel
- Legend for node types and statuses
- Visual distinction for:
  - complex event
  - primitive event
  - evidence match
  - missing/pending event
  - discrepancy/conflict
- Source rows connected to selected node
- Participant roles/entities:
  - agent/causer
  - patient/entity affected
  - location
- Actions:
  - `Highlight source`
  - `Compare against visual evidence`
  - `Add investigator note`

Recommended label:

Use `Event relationship map` as the user-facing title, with `Schema-backed` as a tag. The word `schema` can stay in supporting copy, but should not carry the interface by itself.

Design note:

This screen must answer the known critique from the project notes: no legend, unclear node meaning, unclear source relationship, and hard-to-follow side panel.

### 5. Simulation Evidence

Former screen: `VR Scene Review`

Goal: align VR with the paper's simulation evidence panel.

Core elements:

- 3D reconstruction / bird's-eye scene placeholder
- Annotation overlays:
  - train car cluster
  - immediate danger zone
  - buildings at risk
  - responder route
- Prompt/result example:
  - User asks: `Where is the immediate danger?`
  - Simulation highlights train cars in orange/red
- Linked source panel
- Explanation that simulation is optional spatial review, not the only investigation path

Design note:

Prefer `Simulation Evidence` as the primary page label. VR can appear as a mode or future extension, but the paper's core term is simulation evidence.

## Navigation Labels

Recommended navigation:

1. `Ask`
2. `Q/A Results`
3. `Compare Evidence`
4. `Timeline + Event Map`
5. `Simulation Evidence`

Alternative, more descriptive labels:

1. `Incident + Question`
2. `Answer + Sources`
3. `Evidence Comparison`
4. `Timeline + Schema`
5. `Simulation Review`

## Visual Direction

Keep from first pass:

- warm cream background
- white evidence panels
- restrained charcoal navigation
- serif title treatment
- subtle green/red/gold confidence cues

Refine:

- make content denser and more dashboard-like
- reduce generic "AI synthesis" language
- increase visible source metadata
- show ranked evidence scores
- add explicit links between panels
- make status/confidence/discrepancy cues consistent

## Prototype Interaction Map

Minimum clickthrough interactions:

- Suggested question on Screen 1 opens Screen 2
- `Compare sources` opens Screen 3
- `Add to timeline` or timeline action opens Screen 4
- `Open simulation` opens Screen 5
- Navigation tabs move between screens

Optional interactions if time allows:

- Different questions swap answer/evidence content
- Timeline event nodes highlight related evidence
- Event map nodes change selected detail panel
- Simulation labels update the linked-source panel

## What To Avoid

- Do not make role selection the first required step.
- Do not let VR replace the dashboard.
- Do not imply full live response/sensor fusion is solved.
- Do not present AI answers without visible evidence.
- Do not use `schema` as an unexplained user-facing concept.
- Do not build a beautiful graph that fails to show source links.

## Revised Build Priority

1. Redesign Screen 2 as the true `Q/A Results Workspace`.
2. Redesign Screen 4 as `Timeline + Event Map` with legend, source links, and selected node details.
3. Update Screen 3 to include textual evidence beside visual evidence.
4. Rename Screen 5 to `Simulation Evidence` and add question-driven annotation behavior.
5. Lightly update Screen 1 navigation labels and prompt language.

## Final Recommendation

Keep the existing Figma visual system, but revise the information architecture. The dashboard story should become:

`MUMOSA helps investigators ask grounded questions, compare multimodal evidence, and construct a trusted event narrative.`

That is closer to the source paper and stronger for critique than a generic event overview dashboard.
