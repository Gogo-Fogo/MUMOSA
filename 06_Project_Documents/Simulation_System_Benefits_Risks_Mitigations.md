# Simulation System Benefits, Risks, And Mitigations

This document summarizes the likely benefits, drawbacks, and mitigation strategies for the MUMOSA spatial simulation / VR review module. It is based on the project documents, extracted board notes, dashboard plan, testing plan, and a light scan of the research PDFs in the folder.

## Working Definition

The simulation tool is the spatial review layer of MUMOSA.

It should let a post-crisis investigator explore a reconstructed incident scene on PC first, with VR as an intended extension. The user should be able to inspect AI-labeled evidence markers, understand hazards and event sequence in physical context, verify source material, and return findings to the dashboard.

## Core System Benefits

### 1. Better Spatial Understanding

Benefit:

- Investigators can see where evidence occurred instead of only reading about it.
- Physical relationships become easier to understand: object positions, hazards, routes, smoke/fire spread, responder location, and proximity between events.
- A scene walkthrough can make schema/event relationships feel less abstract.

Why it matters:

- The existing project notes repeatedly identify fragmented evidence and unclear schema relationships as a problem.
- The simulation can turn abstract event nodes into place-based evidence.

Mitigation / design requirement:

- Every spatial marker should explain what it represents, why it matters, and which dashboard evidence it connects to.
- Avoid decorative 3D labels that look impressive but do not support investigation.

### 2. Reduced Cognitive Load

Benefit:

- Spatial memory can make complex AI summaries easier to understand.
- Users can offload some mental work onto the environment: "the smoke drift was over there," "the bodycam came from this position," "the ignition candidate is near this cluster."
- A scene can support overview-to-detail thinking: walk the scene, inspect an object, return to the larger timeline.

Why it matters:

- The notes and research folder repeatedly emphasize cognitive load, situational awareness, and decision-making under pressure.
- The simulation supports comprehension rather than just display.

Mitigation / design requirement:

- Keep the first prototype focused on a small number of meaningful markers.
- Use progressive disclosure: show the scene first, then reveal details on selection.
- Provide a desktop mode so users can review calmly without headset friction.

### 3. Stronger Source-Grounded AI Trust

Benefit:

- AI-generated findings become easier to verify when each marker links to source footage, reports, timestamps, or confidence/discrepancy notes.
- Users can treat AI as an investigative lead instead of a black-box answer.

Why it matters:

- The MUMOSA direction is source-grounded Q/A, not unsupported AI narration.
- The dashboard plan emphasizes answer + sources, evidence comparison, and timeline/schema traceability.

Mitigation / design requirement:

- Each spatial marker should include:
  - source name
  - source type
  - timestamp or time range
  - confidence level
  - related timeline event
  - discrepancy warning if applicable
- The UI should avoid wording like "AI proves." Prefer "AI suggests," "linked evidence indicates," or "requires verification."

### 4. Forensic Preservation

Benefit:

- A digital twin or reconstructed scene can preserve a crisis site after it is cleaned, repaired, weathered, or physically unsafe.
- Investigators can revisit the scene later and inspect details that were missed during the initial visit.

Why it matters:

- The board notes explicitly frame VR as a "time machine" for forensic preservation.
- LiDAR, photogrammetry, and Gaussian splatting are useful because crisis scenes are temporary.

Mitigation / design requirement:

- Distinguish raw evidence from reconstructed interpretation.
- Label whether a scene element is measured, inferred, simulated, or manually placed.
- Keep an audit trail for every marker and scene change in a future version.

### 5. Safer Review Of Dangerous Sites

Benefit:

- Investigators and remote experts can explore hazardous areas without entering collapsed, contaminated, burning, or unstable environments.
- Simulation can support training and after-action review without exposing users to danger.

Why it matters:

- The screenshots mention drones, robots, hot zones, hazardous structures, and remote experts.

Mitigation / design requirement:

- Present VR as review/training support, not as a replacement for official field procedures.
- Include disclaimers or status labels for uncertain hazards.
- Support non-VR desktop access for users who cannot or should not use a headset.

### 6. Training And Mental Model Building

Benefit:

- Responders can practice recognizing key cues before facing a similar scene in person.
- Simulation can build familiarity and reduce anticipatory anxiety.
- Scenario replay can help users compare decisions with actual outcomes.

Why it matters:

- Several project notes frame MUMOSA as both investigation support and training support.
- The source paper direction includes crisis documentation and training resources.

Mitigation / design requirement:

- Keep training as a secondary mode for the current prototype.
- Do not mix training scoring with forensic investigation unless the mode is clearly labeled.
- Use guided scenarios only after the basic scene review flow works.

### 7. Better Collaboration

Benefit:

- Remote experts can discuss the same scene, object, and evidence marker without needing to travel immediately.
- Shared spatial references reduce ambiguity: "look at marker 04 near the railcar cluster" is clearer than "look at the image on page 8."

Why it matters:

- The board notes mention global teams standing in the same virtual room.
- Emergency response research often emphasizes multi-team coordination.

Mitigation / design requirement:

- For the first prototype, simulate collaboration through saved notes or selected markers.
- Future versions can add multi-user sessions, shared pointers, voice, or annotated walkthroughs.

## Major Risks And Drawbacks

### 1. VR Could Distract From The Dashboard

Risk:

- Reviewers may think the project has become a VR product instead of a MUMOSA extension.
- Users may enter VR for broad information foraging, which is slower than dashboard search.

Mitigation:

- Keep the dashboard as the primary workflow.
- Frame the simulation as "spatial evidence review" or "simulation evidence."
- Enter simulation only from a specific evidence object, hazard, timeline event, or source-backed question.

Prototype implication:

- Start the simulation with a selected object or task, not an empty open world.

### 2. Hardware And Accessibility Barriers

Risk:

- Headsets are expensive.
- Some users experience motion sickness.
- Older users or users with disabilities may not tolerate VR.
- Agencies may not have standardized VR equipment.

Mitigation:

- PC-first mouse and keyboard mode.
- Optional VR mode later.
- Desktop 3D preview as a fallback.
- Mobile AR as a future bridge.
- Comfort settings: teleport movement, snap turn, seated mode, reduced motion.

Prototype implication:

- Build and present the simulation as working on PC, with VR as an extension.

### 3. Data Size And Streaming Burden

Risk:

- Point clouds, Gaussian splats, LiDAR scans, drone video, and bodycam archives can be huge.
- Users should not store terabytes of evidence locally.
- Loading everything into a scene would be slow and unnecessary.

Mitigation:

- Store raw media in cloud/object storage.
- Stream only relevant assets for the selected incident, object, and time range.
- Use low-resolution previews first, then load high-resolution sources on demand.
- Cache small scene packages locally when needed.
- Use edge compute for future low-latency response settings.

Prototype implication:

- Fake the pipeline with small local assets and JSON metadata.
- Clearly document that production would use cloud storage and selective streaming.

### 4. Latency During Active Response

Risk:

- First responders cannot wait for full 3D reconstruction during an active incident.
- Network quality may be poor.
- Low latency matters more than photorealism during response.

Mitigation:

- Separate active response from post-crisis investigation.
- For active response, prioritize lightweight alerts, low-res feeds, and edge processing.
- For post-crisis investigation, allow heavier reconstruction and AI analysis.

Prototype implication:

- Current prototype should focus on post-crisis review and training.
- Future roadmap can include real-time response support.

### 5. False Sense Of Accuracy

Risk:

- A realistic 3D scene may feel more certain than it actually is.
- Gaussian splats or reconstructed geometry can look convincing while still containing gaps, artifacts, or inferred content.
- Users might confuse visual realism with forensic certainty.

Mitigation:

- Label evidence confidence.
- Distinguish measured, inferred, simulated, and unknown elements.
- Show source links for each marker.
- Use discrepancy warnings.
- Include uncertainty overlays, not just polished visuals.

Prototype implication:

- Add marker states such as confirmed, inferred, conflicting, and pending.

### 6. AI Error And Overtrust

Risk:

- AI could mislabel objects, extract the wrong event, hallucinate relationships, or overstate confidence.
- Users may trust AI labels because they appear inside a realistic scene.

Mitigation:

- Make source verification the main interaction.
- Use "AI suggests" language.
- Require source visibility for claims.
- Provide a way to flag discrepancies or add investigator notes.

Prototype implication:

- At least one marker should show a conflict or medium-confidence warning.

### 7. Privacy, Security, And Chain Of Custody

Risk:

- Footage may contain victims, private property, responder identities, protected information, or legally sensitive evidence.
- Forensic evidence needs access control, audit logs, and preservation of original files.

Mitigation:

- Use role-based access.
- Use signed links or secure tokens for media.
- Preserve original evidence separately from processed reconstructions.
- Track who viewed, annotated, changed, or exported evidence.
- Redact sensitive media where appropriate.

Prototype implication:

- Mention access control and audit trail in the architecture plan.
- Do not imply open public access to evidence.

### 8. Complex Toolchain

Risk:

- Unreal/Unity, point clouds, photogrammetry, LiDAR, cloud media, dashboard APIs, and AI extraction can become too much for one semester.

Mitigation:

- Build the prototype in layers:
  - PC walkthrough.
  - Evidence markers.
  - Source panel.
  - Timeline states.
  - Dashboard handoff mock.
  - VR support later.
- Treat point clouds/Gaussian splatting as production pipeline concepts unless a small sample asset is available.

Prototype implication:

- The first build should prove the interaction model, not the full reconstruction pipeline.

## Tool Implications

### Unity

Best if the priority is fast interaction:

- PC walkthrough.
- Clickable markers.
- JSON-driven evidence.
- VR-ready later through XR tooling.
- Easier scripting and iteration.

Risk:

- Less naturally suited to ultra-realistic digital twin presentation without extra asset work.

### Unreal Engine

Best if the priority is forensic digital twin realism:

- Strong visual fidelity.
- Stronger built-in story for LiDAR point clouds and large photogrammetry-style environments.
- Better future story for pixel streaming and high-end spatial review.
- Blueprints can support AI-assisted rapid prototyping.

Risk:

- Heavier setup and more room to lose time in engine complexity.

### Web / Three.js / Babylon.js

Best if the priority is dashboard integration:

- Runs directly in browser.
- Easy link from dashboard.
- No install.
- Good for lightweight 3D previews.

Risk:

- Harder to make a high-fidelity forensic/VR scene quickly.

## Recommended Position

For the class/project:

- Build the simulation as a PC-first spatial review prototype.
- Keep VR as an intended extension.
- Use a small, controlled scene.
- Use fake but realistic evidence metadata.
- Show source-grounded markers, hazard overlays, and timeline states.

For the future system:

- Dashboard remains web-first.
- Raw media lives in cloud/object storage.
- Spatial scene assets are streamed selectively.
- High-fidelity Unreal/Unity scene opens from dashboard using incident ID, selected object ID, and time range.
- Active response uses lower-latency edge workflows.
- Post-crisis investigation uses heavier reconstruction, AI analysis, and source verification.

## Design Rules For The Simulation Tool

1. Do not let VR replace the dashboard.
2. Start from a specific investigative question or selected evidence object.
3. Keep every AI claim tied to source evidence.
4. Show uncertainty, confidence, and discrepancies.
5. Support mouse and keyboard first.
6. Make VR optional, not mandatory.
7. Use spatial review only when spatial context adds value.
8. Avoid overbuilding real-time sensor fusion in the first version.
9. Treat realism as useful only when it improves forensic understanding.
10. Make the return path to the dashboard obvious.

## Best Prototype Scenario

Use one incident scenario:

- East Palestine train derailment or a fictionalized rail/industrial incident.

Use one user:

- Post-crisis investigator.

Use one task:

- Verify the likely ignition zone and understand how it relates to smoke spread, railcar cluster, responder route, and source evidence.

Use five marker types:

- Ignition zone.
- Smoke drift.
- Railcar cluster.
- Response vehicle.
- Bodycam/drone source position.

Use four timeline states:

- Before.
- Derailment onset.
- Fire/smoke spread.
- Response/containment.

Use one clear success claim:

> The simulation helps investigators understand and verify spatial evidence that is difficult to interpret from text, images, or schema graphs alone.

