# Prototype Testing Plan

This testing plan is designed for the current Axure/Figma-style digital prototype. It focuses on whether the concept is understandable, not whether the system is technically complete.

## Testing Goal

Test whether users understand the MUMOSA workflow:

- Enter or select an incident.
- Ask an investigative question.
- Inspect an AI-supported answer.
- Verify sources.
- Understand the timeline/schema relationship.
- Decide when VR scene review is useful.

## What We Are Testing

Primary questions:

- Does the user understand that the dashboard is the main workspace?
- Does the user understand that AI claims should be verified through sources?
- Can the user follow the relationship between evidence, timeline, and schema/event map?
- Does the VR handoff make sense?
- Does the user understand VR as an optional spatial review mode rather than a replacement for the dashboard?

## What We Are Not Testing

Out of scope for this prototype:

- Real-time live sensor fusion.
- Real VR hardware performance.
- Production AI accuracy.
- Full profession-specific workflows.
- Full emergency dispatch or active-response operations.

## Suggested Participants

Use 3 to 5 lightweight testers if possible.

Good participants:

- Classmates unfamiliar with the full project.
- Someone who has seen the paper prototype.
- Someone with UX or HCI background.
- Someone who can roleplay a post-crisis investigator.

## Test Script

Opening prompt:

> Imagine you are a post-crisis investigator reviewing a recent incident. You are using MUMOSA to understand what happened and verify the evidence behind AI-supported findings.

Task 1:

> Start from the incident entry screen and find the main event overview.

Success signal:

- User knows what incident or case they are reviewing.
- User can describe the purpose of the dashboard.

Task 2:

> Find what MUMOSA thinks happened and identify at least one source behind that claim.

Success signal:

- User notices source cues.
- User understands that the AI answer is not unsupported.

Task 3:

> Use the evidence detail or comparison screen to check whether the claim is trustworthy.

Success signal:

- User can find source material, confidence, or discrepancy cues.
- User can explain what evidence supports or complicates the claim.

Task 4:

> Use the timeline/schema view to understand the order of events.

Success signal:

- User can identify sequence.
- User can connect at least one event to evidence.
- User is not confused by graph labels.

Task 5:

> Open the VR scene review for a spatial item and explain why you would or would not use it.

Success signal:

- User understands VR is optional.
- User understands it is useful for physical scene context.
- User can still continue if no headset is connected.

## Post-Test Questions

Ask these after the walkthrough:

- What do you think MUMOSA is mainly helping you do?
- Where did you feel most confident in the evidence?
- Where did you feel confused?
- Did the timeline/schema view help you understand the incident?
- Did the VR mode feel necessary, optional, or distracting?
- What should the prototype explain more clearly?

## Key Observations To Capture

During testing, write down:

- Where users hesitate.
- Which labels they misunderstand.
- Whether they notice source links.
- Whether the VR handoff feels connected to the dashboard.
- Whether they can explain the difference between dashboard review and VR review.
- Whether they expect a device pairing step.

## Success Criteria

The prototype is successful enough for class presentation if most testers can say:

- The dashboard is the primary workspace.
- MUMOSA helps reconstruct and verify what happened.
- AI claims are connected to evidence sources.
- The timeline/schema view shows relationships and sequence.
- VR is an optional mode for spatial scene review, hazard understanding, and training.

