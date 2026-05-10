# MUMOSA Project Index

This folder contains the organized working materials for the MUMOSA digital prototype project — a multimodal situational-awareness dashboard for post-crisis investigation.

## Project Overview

MUMOSA is an interactive multimodal Q/A workspace for forensic reconstruction. The core workflow:

1. **Ask** — Select an incident/timeframe and ask a natural-language question
2. **Inspect** — View the AI answer with source-grounded textual and visual evidence
3. **Compare** — Trace claims back to source material, check confidence and discrepancies
4. **Map** — Build a trusted timeline and schema-backed event relationship map
5. **Simulate** — Optionally inspect simulation/VR evidence for spatial context

---

## Quick Links

### Prototype Deliverables

| Link | Description |
|---|---|
| [Figma Prototype](https://www.figma.com/design/p5qdO0wtwuaDpVldrRV3Pn) | Live Figma dashboard prototype |
| [Revised Figma Dashboard Plan](02_Digital_Prototype/Revised_Figma_Dashboard_Plan.md) | Full Figma redesign spec aligning to the MUMOSA paper |
| [Axure Build Spec](02_Digital_Prototype/Axure_RP11_Build_Spec.md) | Detailed Axure RP 11 build specification (design system, screens, interactions) |
| [Axure Edit Guide](02_Digital_Prototype/Axure_Edit_Guide_Revised_Dashboard.md) | Migration guide from old to revised dashboard screens |
| [Axure Prototype File](02_Digital_Prototype/MUMOSA_Digital_Prototype_Axure.rp) | Axure RP source file |

### Unreal Simulation Module

| Link | Description |
|---|---|
| [Engine Setup Plan](06_Project_Documents/Unreal_Engine_Setup_Plan.md) | PC-first First Person C++ template setup, plugins, folder structure |
| [MVP Build Plan](06_Project_Documents/Unreal_Today_MVP_Build_Plan.md) | "Build today" plan — 7 phases for a user-research-ready prototype |
| [Implementation Plan](06_Project_Documents/Unreal_Simulation_Implementation_Plan.md) | 15-stage staged implementation from project creation to presentation |
| [Long-Term Roadmap](06_Project_Documents/Unreal_Simulation_Long_Term_Roadmap.md) | Milestones over weeks/months including VR readiness |
| [Benefits, Risks & Mitigations](06_Project_Documents/Simulation_System_Benefits_Risks_Mitigations.md) | Analysis of spatial simulation benefits, drawbacks, and design rules |
| [Evidence Capture Pipeline](06_Project_Documents/Evidence_Capture_Pipeline.md) | End-to-end data flow: drone capture → AI processing → storage tiers → Nanite rendering |

### Team & Strategy Documents

| Link | Description |
|---|---|
| [Team Alignment Addendum](06_Project_Documents/Team_Alignment_Addendum.md) | Decision record: one user, one workflow, one incident |
| [VR/Simulation Rationale](06_Project_Documents/VR_Simulation_Rationale.md) | Why VR is a "power tool," not a dashboard replacement |
| [Dashboard-to-VR User Flow](06_Project_Documents/Dashboard_To_VR_User_Flow.md) | Handoff flow with fallback paths for no-headset scenarios |
| [Presentation Talking Points](06_Project_Documents/Presentation_Talking_Points.md) | 20-minute presentation structure and one-sentence pitch |
| [Prototype Testing Plan](06_Project_Documents/Prototype_Testing_Plan.md) | 5-task usability test script with success criteria |
| [Prototype Media Strategy](06_Project_Documents/Prototype_Media_Strategy.md) | Sourcing representative public-domain media for the Unreal scene |
| [Dev Environment](06_Project_Documents/Dev_Environment.md) | Rig specs — RTX 5070 Ti, 64 GB RAM, 2 TB NVMe |

### Reference Materials

| Link | Description |
|---|---|
| [Project Board Export](01_Project_Overview/MUMOSA_Project_Board.pdf) | High-level visual project board PDF |
| [Extracted Notes](05_Extracted_Notes/MUMOSA_Project_Extracted_Text.txt) | Raw notes from project board, usability scripts, and feedback |
| [Annotated Literature Review](03_Research/Annotated_Literature_Review_Georgi_Tsvetanski.pdf) | Literature review PDF |
| [Media Manifest](07_Prototype_Media/media_manifest.md) | Tracked source media with attribution |

---

## Folder Structure

### `01_Project_Overview`
High-level project board exports used as the main visual reference.
- `MUMOSA_Project_Board.pdf`, `MUMOSA_Project_Board.png`

### `02_Digital_Prototype`
Digital prototype deliverables and implementation guidance for both Figma and Axure RP 11.
- `MUMOSA_Digital_Prototype_Axure.rp` — Axure source file
- `Axure_RP11_Build_Spec.md` — Full build spec (colors, typography, 5-page layout, interactions)
- `Axure_Edit_Guide_Revised_Dashboard.md` — Instructions to migrate from old to revised screens
- `Revised_Figma_Dashboard_Plan.md` — Figma redesign aligned to the MUMOSA paper
- `Figma_Prototype_Link.md` — Figma URL reference

### `03_Research`
Research sources and literature review materials.
- `Annotated_Literature_Review_Georgi_Tsvetanski.pdf`
- `Literature_Review/`
- `Additional_Research/`

### `04_Paper_Prototype_VR`
Photos of the paper prototype and VR spatial interface sketches.
- `VR_Paper_Prototype_01.jpg` through `VR_Paper_Prototype_05.jpg`

### `05_Extracted_Notes`
Extracted text and rough notes from the project board and source materials.
- `MUMOSA_Project_Extracted_Text.txt`

### `06_Project_Documents`
Clean project documentation synthesized from team alignment and VR/simulation board notes. Covers strategy, user flows, testing, simulation analysis, Unreal plans, and media strategy. See Quick Links above for full listing.

### `07_Prototype_Media`
Representative media library for Unreal simulation evidence. Public-domain/government/Creative Commons substitute media with attribution tracking.
- `README.md`, `media_manifest.md`
- `source_photos/`, `source_video/`, `processed_stills/`
- `reference_links/`, `licenses_and_attribution/`

---

## Current Prototype Direction

The revised prototype focuses on MUMOSA as an **interactive multimodal Q/A workspace for post-crisis investigation**:

1. Select an incident and timeframe
2. Ask a natural-language question
3. Inspect the answer backed by textual and visual evidence
4. Compare sources, confidence, and discrepancies
5. Build a trusted timeline and schema-backed event map
6. Use simulation/VR as an optional spatial evidence mode

**Navigation (revised):** `Ask` → `Q/A Results` → `Compare Evidence` → `Timeline + Event Map` → `Simulation Evidence`

The strongest critique target is the **Timeline + Event Map** view — it must clearly show event relationships, source links, evidence confidence, and discrepancies rather than acting as a detached graph. See the [Revised Figma Dashboard Plan](02_Digital_Prototype/Revised_Figma_Dashboard_Plan.md) for details.
