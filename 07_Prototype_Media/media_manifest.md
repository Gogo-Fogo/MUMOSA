# MUMOSA Prototype Media Manifest

Use this manifest to track every image, video, still frame, or scan-like asset used in the Unreal simulation prototype.

## Media Collection Rule

Do not use random internet media without tracking:

- source URL
- source organization
- license or use note
- attribution text
- which prototype marker it supports

## Recommended Source Links

EPA East Palestine photos:

- https://www.epa.gov/east-palestine-oh-train-derailment/photos

EPA East Palestine main page:

- https://www.epa.gov/east-palestine-oh-train-derailment

NTSB B-roll: Train Derailment in East Palestine, OH:

- https://www.youtube.com/watch?v=7AyXTVkVBT4

FEMA Media Library:

- https://www.fema.gov/fema-media-library

DVIDS Copyright Info:

- https://api.dvidshub.net/docs/copyright

DHS Photo, Video & Audio Use Guidelines:

- https://www.dhs.gov/photo-video-audio-use-guidelines

## Manifest Template

Copy this block for each asset.

```text
Asset ID:
Filename:
Media type:
Source URL:
Source organization:
License / usage note:
Attribution text:
Prototype incident:
Prototype marker supported:
Timeline state:
Description:
How used in Unreal:
Downloaded/processed by:
Date added:
```

## Planned Evidence Package

Prototype incident:

- Rail Yard Chemical Derailment Demo

Purpose:

- Simulate how MUMOSA could connect dashboard evidence, AI interpretation, and spatial scene markers.

### Marker 1: Ignition Zone

Needed sources:

- NTSB b-roll frame.
- EPA crash site recovery photo.
- Official-style mock incident log excerpt.

How used:

- Marker source panel.
- Timeline state: Fire/smoke spread.
- Confidence/discrepancy example.

### Marker 2: Smoke Drift

Needed sources:

- Drone/UAS footage frame.
- Air monitoring photo.
- Timeline note.

How used:

- Hazard overlay explanation.
- Smoke drift marker panel.
- Possible uncertainty/inferred region example.

### Marker 3: Railcar Cluster

Needed sources:

- EPA burned tank car cleanup photo.
- Rail track cleanup photo.

How used:

- Scene landmark.
- Evidence marker for object cluster.
- Relationship to ignition/smoke timeline.

### Marker 4: Response Vehicle

Needed sources:

- FEMA/DVIDS responder image.
- Mock dispatch note.

How used:

- Response route marker.
- Timeline state: Response/containment.
- Training/collaboration discussion.

### Marker 5: Sampling Zone

Needed sources:

- EPA soil/water sampling photo.
- Cleanup documentation.

How used:

- Environmental evidence marker.
- Source-grounded verification example.
- Post-crisis investigation framing.

## Example Filled Entry

```text
Asset ID: EPA_EP_001
Filename: pending
Media type: Photo
Source URL: https://www.epa.gov/east-palestine-oh-train-derailment/photos
Source organization: U.S. Environmental Protection Agency
License / usage note: Government/public agency source. Verify image-specific restrictions before final publication.
Attribution text: Source: U.S. EPA East Palestine Train Derailment photo archive.
Prototype incident: Rail Yard Chemical Derailment Demo
Prototype marker supported: Sampling Zone
Timeline state: Response/containment
Description: Environmental sampling or cleanup operation near derailment site.
How used in Unreal: Thumbnail/source card linked to sampling zone marker.
Downloaded/processed by:
Date added:
```

