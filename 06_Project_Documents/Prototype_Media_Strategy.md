# Prototype Media Strategy

The MUMOSA Unreal prototype does not require official MUMOSA media. It requires believable representative evidence that demonstrates the workflow:

```text
reconstructed scene -> evidence markers -> source footage/images -> AI interpretation -> confidence/discrepancy -> timeline/hazard context
```

## Main Rule

Do not claim that substitute media is official MUMOSA footage.

Use this label:

```text
Representative source evidence
Used for educational prototype purposes. Not official MUMOSA system data.
```

## Best Free / Legal Source Categories

Use government/public-domain-style sources first.

### EPA East Palestine Photos

Useful for:

- Rail tracks.
- Excavations.
- Tanks.
- Sampling.
- Machinery.
- Cleanup areas.
- Air monitoring.
- Damaged/burned cars.

Source:

- https://www.epa.gov/east-palestine-oh-train-derailment/photos

### NTSB B-Roll

Useful for:

- Investigator footage.
- Drone/UAS operations.
- Incident context.
- Realistic official evidence feel.

Source:

- https://www.youtube.com/watch?v=7AyXTVkVBT4

### FEMA Media Library

Useful for:

- Disaster response.
- Cleanup.
- Flooding/fire/emergency operations.
- Responders.
- Damage assessment.

Source:

- https://www.fema.gov/fema-media-library

### DVIDS

Useful for:

- Public-domain military/emergency training footage.
- Drones.
- Hazard response exercises.
- Rescue drills.
- Command operations.

Usage note:

- DVIDS states media is generally public domain unless otherwise marked, with credit requested.

Source:

- https://api.dvidshub.net/docs/copyright

### DHS Media

Useful for:

- Public-domain safety, emergency, disaster, and agency media.

Usage note:

- DHS states most DHS-produced multimedia is public domain in the U.S. and available for education/informational purposes.

Source:

- https://www.dhs.gov/photo-video-audio-use-guidelines

## Media Types To Collect

Minimum useful library:

- 3 to 5 photos of rail/industrial cleanup.
- 1 short video clip or stills from drone/UAS footage.
- 1 responder/field operations image.
- 1 environmental sampling/air monitoring image.
- 1 map or aerial-style image.
- Optional smoke/fire/disaster training footage.
- Optional point-cloud-looking screenshot or sample scan.

## Evidence Package Mapping

Prototype incident:

- Rail Yard Chemical Derailment Demo.

### Marker 1: Ignition Zone

Sources:

- NTSB b-roll frame.
- EPA crash site recovery photo.
- Official-style incident log excerpt.

### Marker 2: Smoke Drift

Sources:

- Drone/UAS footage frame.
- Air monitoring photo.
- Timeline note.

### Marker 3: Railcar Cluster

Sources:

- EPA burned tank car cleanup photo.
- Rail track cleanup photo.

### Marker 4: Response Vehicle

Sources:

- FEMA/DVIDS responder image.
- Mock dispatch note.

### Marker 5: Sampling Zone

Sources:

- EPA soil/water sampling photo.
- Cleanup documentation.

## How Media Appears In Unreal

Media should be used as source objects.

Examples:

- Floating evidence panels.
- Source thumbnails.
- Clickable video screens.
- Bodycam/drone position markers.
- Timeline-linked snapshots.
- Confidence/discrepancy references.

## Folder Structure

Prototype media should live in:

```text
07_Prototype_Media/
  source_photos/
  source_video/
  reference_links/
  processed_stills/
  licenses_and_attribution/
  README.md
  media_manifest.md
```

## Next Step

Before downloading media, add each candidate link to:

```text
07_Prototype_Media/media_manifest.md
```

Then download, rename, and process only the assets that directly support a marker or timeline state.

