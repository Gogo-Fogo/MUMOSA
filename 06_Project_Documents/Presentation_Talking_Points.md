# Presentation Talking Points

Use this as the practical story for presenting the prototype. The goal is not to explain every feature. The goal is to show a narrowed, client-aligned prototype with a clear problem, user, workflow, and next step.

## 20-Minute Presentation Shape

### 1. Opening

MUMOSA is a multimodal situational-awareness system. Our prototype focuses on the post-crisis investigation moment, where an investigator needs to reconstruct what happened and verify evidence across text, images, schema, and simulation.

### 2. Problem

Investigators face fragmented information after a crisis. Evidence is spread across reports, images, videos, maps, source logs, and simulation outputs. AI can help summarize this information, but it must stay source-grounded so users can verify what the system claims.

### 3. Persona

Our primary user is a post-crisis investigator, CSI, fire investigator, or similar analyst.

The user is not just browsing data. They are trying to answer:

- What happened?
- When did it happen?
- Where did it happen?
- What evidence supports the answer?
- What still needs to be checked?

### 4. Scope Decision

We narrowed the project to one main workflow:

> Select an incident, ask a question, inspect the answer with sources, compare evidence, review the timeline, and optionally enter VR for spatial scene review.

This keeps the prototype testable and prevents the team from building separate full journeys for every profession.

### 5. Dashboard Role

The dashboard is the primary system.

It supports:

- Incident entry.
- Open search or natural-language Q/A.
- Event overview.
- Source-grounded evidence review.
- Timeline and schema/event relationships.
- Confidence and discrepancy cues.

### 6. VR Role

VR is not the product by itself. VR is an optional spatial review mode.

It helps when the investigator needs to:

- Walk through a reconstructed scene.
- Understand hazard zones.
- See where evidence was physically located.
- Review a digital twin after the real scene has changed.
- Train or debrief in a lower-risk environment.

### 7. Paper To Digital Iteration

The paper prototype helped us explore the spatial concept first. The digital prototype turns that idea into a more structured workflow:

- Dashboard first.
- Source verification always visible.
- VR entered only when spatial review adds value.
- Fallback paths for users without VR equipment.

### 8. Prototype Walkthrough

Recommended walkthrough order:

1. Incident entry.
2. Event overview with AI summary and source cues.
3. Evidence detail showing source-grounded claim verification.
4. Timeline and schema/event map showing chronology and relationships.
5. VR handoff screen explaining why spatial review is useful.
6. VR scene review mockup with selected object, linked sources, and next step.
7. Return to dashboard.

### 9. Testing Plan

We are testing whether users understand the workflow and trust model:

- Can users tell what MUMOSA is helping them do?
- Can users find the source behind an AI claim?
- Can users explain when VR is useful and when it is unnecessary?
- Can users move from dashboard evidence into spatial review without confusion?

### 10. Next Steps

Next steps after the prototype:

- Run short usability tests.
- Refine the timeline/schema view.
- Improve the VR handoff screen.
- Add clearer confidence and discrepancy indicators.
- Decide whether mobile AR or desktop 3D preview should be the most practical non-headset fallback.

## One-Sentence Pitch

Our prototype shows MUMOSA as a source-grounded investigation dashboard where VR is an optional spatial review tool for understanding complex scenes, hazards, and evidence relationships.

