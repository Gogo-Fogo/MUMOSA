# Evidence Capture & Spatial Review Pipeline

## Overview

This document captures the end-to-end pipeline from crisis scene capture to spatial evidence review, covering the logistics discussion around data flow, AI processing, storage, and rendering. It bridges the gap between the prototype's interaction model and the production deployment MUMOSA would use.

---

## Pipeline Flow

```
Drone / Robot / Bodycam Capture
        │
        ▼
   Raw Media (video, photos, LiDAR)
        │
        ▼
   AI Processing
   ┌─────────────────────────────────────┐
   │  Option A: Low-Fidelity (Fast)       │
   │  Gaussian Splatting → ~5-15 min      │
   │  Good enough for active response     │
   │                                      │
   │  Option B: High-Fidelity (Thorough)  │
   │  Photogrammetry → 30 min - 2+ hrs   │
   │  Full mesh + textures                │
   │  Used for post-crisis investigation  │
   └─────────────────────────────────────┘
        │
        ▼
   AI Extraction
   ┌─────────────────────────────────────┐
   │  VLMs extract evidence markers      │
   │  Identify: hazards, objects, events │
   │  Link timestamps + source media     │
   │  Generate AI interpretations        │
   │  Flag discrepancies / conflicts     │
   └─────────────────────────────────────┘
        │
        ▼
   Spatial Scene (Unreal)
   ┌─────────────────────────────────────┐
   │  Scene rendered with Nanite         │
   │  Evidence markers placed in space   │
   │  Investigator walks through +       │
   │  inspects source-grounded claims    │
   └─────────────────────────────────────┘
```

---

## Capture Methods

### Drone / UAS
- High-speed overlapping photos or video
- 5-10 minute flight per scene
- Ideal for large-scale reconstruction (rail yard, industrial site, disaster zone)
- Works with both photogrammetry and Gaussian splatting

### Robot / UGV (e.g. Spot)
- Walks through collapsed or hazardous structures
- 360-degree cameras build interior reconstruction
- Carries LiDAR for precise measurement

### Body Cameras
- GPS + IMU metadata pins 2D video into 3D coordinates
- Spatial mapping: floating video screens positioned where responders stood
- Provides "ground truth" for forensic reconstruction

### LiDAR (where available)
- Instant geometry capture
- Mathematically precise measurements (skid marks, ledge heights, distances)
- Low-res gray mesh — needs texture baking for visual review
- Best combined with photogrammetry: LiDAR for structure, photos for color

---

## AI Processing Pipeline

### Step 1: Reconstruction

| Method | Output | Time | Best For |
|---|---|---|---|
| Gaussian Splatting | Neural point cloud | 5-15 min | Fast review, active response |
| Photogrammetry (RealityCapture) | Dense mesh + textures | 30 min - 2+ hrs | Forensics, evidence-grade |
| LiDAR + Photo overlay | Measured mesh + textures | Instant geometry + baking | Measurement-critical scenes |

**AI is already accelerating Step 1.** Gaussian splatting is neural — it learns the 3D scene from 2D images. Newer methods (2025-2026) claim 1-2 min for small scenes. Real-time feed → instant splat is active research.

### Step 2: Evidence Extraction
Vision-Language Models (VLMs) analyze the reconstruction to extract:
- Key events and their timestamps
- Hazard zones and danger areas
- Objects, vehicles, infrastructure damage
- Witness statements from bodycam audio
- Discrepancies between sources

These become spatial markers with:
- Position in 3D space
- AI interpretation
- Confidence level
- Linked source footage
- Related timeline event

### Step 3: Source Grounding
Every AI claim is linked back to the source material it was extracted from:
- "AI suggests this ignition zone — source: drone pass 2 at 9:22 PM"
- "Confidence: Medium — witness timing conflicts by 2 minutes"
- "Click to view source footage"

---

## Fidelity Tiers (SA Levels Alignment)

Mapped to MUMOSA's Situational Awareness levels:

| Tier | SA Level | Fidelity | Processing Time | Use Case |
|---|---|---|---|---|
| Low | Level 1 (Perception) | Gaussian splat, low-res | 5-15 min | Active response — hot zones, blocked routes, danger areas |
| Medium | Level 2 (Comprehension) | Cleaned mesh, evidence markers | 30-60 min | Post-crisis investigation — timeline, source review |
| High | Level 3 (Projection) | Full reconstruction + source media | 1-2+ hrs | Forensic deep-dive, training, legal evidence |

This tiered approach matches real operational constraints:
- Responders need answers *now* — low fidelity is enough to see danger zones
- Investigators need *accuracy* later — high fidelity processes in the background
- Analysts need *complete context* — full source archives for months after

---

## Storage Architecture

### Data Sizes Per Incident

| Asset Type | Size | Notes |
|---|---|---|
| Raw drone footage (4K, 10 min) | 10-50 GB | The biggest hog |
| Gaussian splat (processed) | 100-500 MB | Surprisingly compact |
| Photogrammetry mesh + textures | 1-5 GB | Scene-dependent |
| Source photos / images | 1-5 GB | Per incident |
| Evidence metadata (JSON) | Kilobytes | Markers, annotations, notes |

### Storage Tiers

| Tier | Media | Capacity | Access |
|---|---|---|---|
| **Hot** (active case) | Splat + markers + current sources | 1-2 GB per case | Local SSD, always available |
| **Warm** (recent cases) | Raw footage + full source media | 50-100 GB per case | Cloud / NAS, on-demand recall |
| **Cold** (closed cases) | Archived full package | 50-100 GB per case | Cloud glacier / tape, reopened only if needed |

### Field Kit
- Ruggedized laptop + 2 TB external SSD
- Splat processes on-device (Toughbook with RTX 5000 or similar)
- Raw footage is NOT stored locally — uploaded to central storage when network is available
- Local working set: splat + evidence markers = few hundred MB

### Chain Of Custody
- Raw footage is write-once archived, cryptographically signed
- Processed reconstructions are derivatives, not replacements
- Access control, audit logging, and signed URLs for sensitive media
- Role-based access (investigator vs. analyst vs. commander)

---

## Nanite & Geometry

### Why Nanite Matters
Raw photogrammetry scans are messy — holes, floating fragments, stretched geometry, baked-in lighting. Traditional game pipelines require retopology (hours of cleanup per object). Nanite renders the raw scan directly at full quality — no retopo needed for static review scenes.

### Hybrid Approach
- **Environment (debris, ground, distant)** → Raw scan + Nanite, artifacts and all
- **Key objects (railcar, ignition zone)** → AI-assisted cleanup → Nanite-ready
- **Collision** → Simple primitive proxies (invisible boxes/capsules) — the investigator isn't playing physics, they're reviewing evidence

### AI Geometry Cleanup (Emerging)
AI tools can already retopo, fill holes, and rebuild UVs from photogrammetry. By the time MUMOSA would deploy, this will be automated:
- Gaussian splat → mesh conversion with AI hole-filling
- One-click cleanup for important objects
- Bulk processing for environment scatter

### Current Prototype
The Derelict Corridor Megascans pack provides photoscanned debris and industrial assets that are Nanite-ready in UE5.7. These demonstrate the visual quality the full pipeline would produce, without needing a real drone-to-splat processing pipeline in the student project.

---

## Query & AI Integration

The user asks a natural-language question through the dashboard:

> *"Where is the immediate danger?"*

The AI (local or API):
1. Queries the evidence record store
2. Determines which markers are relevant
3. Highlights them in the Unreal scene via the MCP bridge
4. Populates the source panel with matching records

This decouples the "thinking" (AI) from the "showing" (Unreal). The MCP bridge is the connector — it receives commands from the AI and executes them in the editor (or packaged build).

For the MVP, this is mocked: hardcoded JSON evidence records that are structured exactly as if they came from an AI query. The data shape is the same, just the source changes.

---

## Client Narrative

> *"MUMOSA's spatial review module uses the same photogrammetry and AI pipeline that powers modern 3D reconstruction. Drones and robots capture the scene, AI processes it into grounded evidence markers, and investigators walk through it — immediately in low fidelity for active response, or in full detail for post-crisis forensics. Nanite renders the raw scans without retopology, and the evidence markers are always linked back to source material so AI never presents unsupported claims."*

---

## Related Documents

- [VR/Simulation Rationale](VR_Simulation_Rationale.md) — Why VR exists and when to use it
- [Simulation System Benefits, Risks & Mitigations](Simulation_System_Benefits_Risks_Mitigations.md) — Full analysis
- [Unreal Engine Setup Plan](Unreal_Engine_Setup_Plan.md) — PC-first development approach
- [Presentation Talking Points](Presentation_Talking_Points.md) — How to present this
