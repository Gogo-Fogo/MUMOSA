# MUMOSA Digital Prototype — Axure RP 11 Build Specification

## 1. Design System

### Colors
| Token | Hex | Usage |
|---|---|---|
| `--bg-page` | `#F5F1EB` | Main page background (warm cream) |
| `--bg-card` | `#FFFFFF` | Card backgrounds |
| `--bg-header` | `#F5F1EB` | Top header area |
| `--text-primary` | `#1A1A1A` | Headings, primary text |
| `--text-secondary` | `#5C5C5C` | Body text, descriptions |
| `--text-muted` | `#8A8A8A` | Metadata, captions |
| `--accent-tag` | `#E8E4DE` | Inactive tag background |
| `--accent-tag-text` | `#4A4A4A` | Tag text |
| `--nav-active` | `#2D2D2D` | Active nav pill (dark charcoal) |
| `--nav-active-text` | `#FFFFFF` | Active nav text |
| `--nav-inactive-bg` | `#FFFFFF` | Inactive nav pill |
| `--nav-inactive-border` | `#D9D5CF` | Inactive nav border |
| `--border-card` | `#EAE6E0` | Card borders |
| `--section-label` | `#B85C38` | Burnt orange section labels (e.g., "SHARED ENTRY") |
| `--confidence-medium` | `#D4A017` | Medium confidence badge |
| `--confidence-high` | `#4A7C59` | High confidence badge |
| `--search-bg` | `#FFFFFF` | Search input background |
| `--search-border` | `#D9D5CF` | Search input border |
| `--image-placeholder` | `#4A5568` | Evidence image background |
| `--timeline-line` | `#D9D5CF` | Timeline connector line |
| `--timeline-dot` | `#2D2D2D` | Timeline event dots |

### Typography
| Role | Font | Size | Weight | Color |
|---|---|---|---|---|
| H1 (Page Title) | Serif (Georgia/Times) | 32px | 700 | `--text-primary` |
| H2 (Section Title) | Serif (Georgia/Times) | 28px | 700 | `--text-primary` |
| H3 (Card Title) | Sans-serif (Inter/System) | 16px | 600 | `--text-primary` |
| Body | Sans-serif | 14px | 400 | `--text-secondary` |
| Caption | Sans-serif | 12px | 400 | `--text-muted` |
| Tag/Label | Sans-serif | 11px | 500 | `--accent-tag-text` |
| Nav Pill | Sans-serif | 13px | 500 | varies |
| Section Label | Sans-serif | 11px | 600 | `--section-label` |

### Spacing & Layout
- **Page max-width:** 1200px, centered
- **Page padding:** 40px horizontal, 32px vertical
- **Card padding:** 24px
- **Card border-radius:** 12px
- **Card shadow:** `0 1px 3px rgba(0,0,0,0.04)`
- **Card gap:** 20px
- **Tag border-radius:** 999px (full pill)
- **Tag padding:** 4px 12px
- **Nav pill border-radius:** 999px
- **Nav pill padding:** 8px 20px

---

## 2. Global Masters (Reuse These)

### Master: `Global Header`
- **Widget:** Rectangle (1200×140)
- **Fill:** `--bg-header`
- **Contents (inside):**
  - Label: "MUMOSA DIGITAL PROTOTYPE" — 11px, `--section-label`, top-left, 24px from edges
  - H1: "MUlti-MOdAl Situation Awareness" — 32px Serif, `--text-primary`, below label, 8px gap
  - Subtitle: "Post-crisis investigation workspace..." — 14px, `--text-secondary`, below H1, 8px gap
  - Tags (top-right corner):
    - "Post-crisis investigator view" — pill, `--accent-tag`
    - "Spring 2026 concept prototype" — pill, `--accent-tag`
    - "Client-aligned direction" — pill with subtle red tint `#F5E6E6`, text `#B85C38`

### Master: `Global Navigation`
- **Widget:** Horizontal row of 5 Rectangle widgets
- **Pills:**
  - Default state: 8px/20px padding, radius 999px, bg `#FFFFFF`, border 1px `--nav-inactive-border`, text `--text-primary`
  - Active state: bg `--nav-active`, text `#FFFFFF`, no border
- **Labels:**
  1. "Incident Entry"
  2. "Event Overview"
  3. "Evidence Review"
  4. "Timeline + Schema"
  5. "VR Scene Review"
- **Position:** Below header, 24px gap, left-aligned with page content
- **Interaction:** OnClick → Open Link → corresponding page

---

## 3. Page 1: Incident Entry

### Page Background
- Color: `--bg-page`

### Layout Structure (Top to Bottom)
1. Global Header (Master)
2. Global Navigation (Master) — "Incident Entry" active
3. Main Card Container (white card, full width)

### Inside Main Card

#### Section Label Row
- Left: "SHARED ENTRY" — 11px, `--section-label`, all-caps
- Right: "Open search first. Filters refine after entry." — 12px, `--text-muted`

#### H2 Title
- "Start with the incident, not the profession" — 28px Serif, `--text-primary`

#### Two-Column Grid (inside card)
**Left Column (~65% width): Search Module Card**
- White sub-card, border `--border-card`, radius 12px, padding 24px
- Header row:
  - Left: "Search crisis incidents" — H3 (16px bold)
  - Right: "Natural-language entry" — tag pill, bg `#E8F0E8`, text `#4A7C59`
- Search Input:
  - Rectangle: full width, 48px height, radius 8px, bg `--search-bg`, border 1px `--search-border`
  - Icon: Search icon (16px), left padding 16px
  - Placeholder text: "Search an event, location, hazard, or question..." — 14px, `--text-muted`
- Suggested Queries (3 rows below input, 12px gap):
  - Each: full-width rectangle, 40px height, bg `#FAFAF8`, border `--border-card`, radius 8px, padding 16px
  - Text: 14px, `--text-secondary`
  - Queries:
    1. "What happened in East Palestine on Feb. 3?"
    2. "Show conflicting evidence about ignition source"
    3. "Find drone imagery tied to the derailment"

**Right Column (~35% width): Recent Investigations Card**
- White sub-card, border `--border-card`, radius 12px, padding 24px
- Header row:
  - Left: "Recent investigations" — H3
  - Right: "5 active" — tag pill, bg `#E8E4DE`
- Investigation List (Repeater recommended):
  - Row height: ~60px, border-bottom 1px `--border-card` (except last)
  - Each row:
    - Title: 14px bold, `--text-primary`
    - Subtitle: 12px, `--text-muted`
  - Items:
    1. "East Palestine Train Derailment" / "147 evidence objects · 6 source collections"
    2. "Baltimore Warehouse Fire" / "93 evidence objects · 4 source collections"
    3. "Bridge Collision Drill Archive" / "Training scenario · 3 reconstructions"

#### Bottom Filters Section (below two-column grid, 20px gap)
- Full-width white sub-card, border `--border-card`, radius 12px, padding 24px
- Header row:
  - Left: "Available refinement filters" — H3
  - Right: "Supportive, not blocking" — tag pill, bg `#F5E6E6`, text `#B85C38`
- Filter Pills Row (horizontal, 8px gap):
  - Each: pill, bg `#FFFFFF`, border 1px `--nav-inactive-border`, padding 6px 16px, 13px medium
  - Filters: "Source type", "Date range", "Evidence modality", "Confidence", "Location / zone", "Role emphasis"
- Footer text (below pills, 12px): "The same core system can later surface different priorities..." — 12px, `--text-muted`

---

## 4. Page 2: Event Overview

### Page Background
- Color: `--bg-page`

### Layout
1. Global Header (Master)
2. Global Navigation (Master) — "Event Overview" active
3. Main Card Container

### Inside Main Card

#### Top Meta Row
- Left: "PRIMARY DASHBOARD" — 11px, `--section-label`, all-caps
- Right: "Incident: East Palestine Train Derailment" — 12px, `--text-muted`

#### H2 Title
- "Event overview centers understanding and verification" — 28px Serif

#### Two-Column Grid

**Left Column (~55%): AI Synthesis Card**
- White sub-card, border `--border-card`, radius 12px, padding 24px
- Header row:
  - Left: "AI synthesis" — H3
  - Right: "Grounded in 12 sources" — tag pill, bg `#E8F0E8`, text `#4A7C59`
- Summary Paragraph:
  - Text: "A train derailment occurred near East Palestine at approximately 9:00 PM EST. Hazard escalation followed multiple sub-events including derailment, ignition, smoke spread, and controlled venting."
  - 14px, `--text-secondary`, line-height 1.6
- Analyst Note Box:
  - Full width, bg `#FAFAF8`, border-left 3px `#D4A017`, padding 16px, radius 4px
  - Label: "Analyst note" — 12px bold, `#D4A017`
  - Text: "Summary confidence is medium until camera sequence 4 and drone pass 2 are reviewed together." — 13px, `--text-secondary`

**Right Column (~45%): Supporting Sources Card**
- White sub-card, border `--border-card`, radius 12px, padding 24px
- Header row:
  - Left: "Supporting sources" — H3
  - Right: "Ranked evidence" — tag pill, bg `#E8E4DE`
- Source List (Repeater):
  - Each row: ~70px height, border-bottom 1px `--border-card`
  - Row structure:
    - Source type label: 12px bold, `--text-primary` (e.g., "Textual report excerpt")
    - Source detail: 12px, `--text-muted` (e.g., "Local station log · 9:14 PM · high relevance")
  - Items:
    1. "Textual report excerpt" / "Local station log · 9:14 PM · high relevance"
    2. "Drone image sequence" / "Overflight 2 · 9:22 PM · smoke expansion visible"
    3. "Street camera capture" / "Camera 05 · 8:58 PM · derailment onset"

#### Suggested Actions Row (below grid, 20px gap)
- Full-width white sub-card, border `--border-card`, radius 12px, padding 24px
- Header row:
  - Left: "Suggested next actions" — H3
  - Right: "AI guidance with traceability" — tag pill, bg `#F5E6E6`, text `#B85C38`
- Three Equal Columns (flex, gap 16px):
  - Each column: white sub-card, border `--border-card`, radius 8px, padding 16px
  - Column structure:
    - Action title: 14px bold, `--text-primary`
    - Action subtitle: 13px, `--text-muted`
  - Actions:
    1. "Compare conflicting source times" / "Open timeline with camera + report overlay"
    2. "Inspect hazard spread" / "Open VR scene review of smoke and fire zones"
    3. "Review evidence cluster" / "Open linked visual evidence and schema nodes"
  - **Interaction:** OnClick → Set Panel State / Open Link to corresponding tab

---

## 5. Page 3: Evidence Review

### Page Background
- Color: `--bg-page`

### Layout
1. Global Header (Master)
2. Global Navigation (Master) — "Evidence Review" active
3. Main Card Container

### Inside Main Card

#### Top Meta Row
- Left: "EVIDENCE REVIEW" — 11px, `--section-label`, all-caps
- Right: "Question: What evidence supports the initial ignition zone?" — 12px, `--text-muted`

#### H2 Title
- "Trace AI claims back to source material" — 28px Serif

#### Two-Column Grid

**Left Column (~55%): Visual Evidence Card**
- White sub-card, border `--border-card`, radius 12px, padding 24px
- Header row:
  - Left: "Visual evidence" — H3
  - Right: "image + annotation" — tag pill, bg `#E8E4DE`
- Image Container:
  - Rectangle: full width, 320px height, radius 8px, fill `--image-placeholder`
  - Placeholder image (use Image widget, import thermal/drone photo)
  - Annotation Labels (text widgets on top of image):
    - "Ignition candidate" — pill, bg `rgba(0,0,0,0.7)`, text `#FFFFFF`, positioned upper-left
    - "Secondary debris field" — pill, bg `rgba(0,0,0,0.7)`, text `#FFFFFF`, positioned lower-right
- Caption Bar (below image, full width, bg `#F5F1EB`, padding 12px, radius 4px):
  - Text: "Drone pass 2 · 9:22 PM · thermal plume visible at west-facing cars" — 13px, `--text-secondary`

**Right Column (~45%): AI Explanation Card**
- White sub-card, border `--border-card`, radius 12px, padding 24px
- Header row:
  - Left: "AI explanation" — H3
  - Right: "Confidence: medium" — tag pill, bg `#F5E6E6`, text `#D4A017`
- Explanation Paragraph:
  - Text: "The model associates the ignition zone with the western segment of the derailment due to matching timing between camera footage, thermal imagery, and textual incident reports."
  - 14px, `--text-secondary`, line-height 1.6
- Source Chain Box:
  - Full width, bg `#FAFAF8`, padding 16px, radius 8px
  - Label: "Source chain" — 12px bold, `--text-primary`
  - Text: "Camera 05 → Drone pass 2 → Fire log excerpt" — 13px, `--text-secondary`
- Discrepancy Box:
  - Full width, bg `#F5E6E6`, padding 16px, radius 8px
  - Label: "Potential discrepancy" — 12px bold, `#B85C38`
  - Text: "Witness report places first visible flame 2 minutes later" — 13px, `--text-secondary`

#### Bottom Filters Row
- Full-width white sub-card, border `--border-card`, radius 12px, padding 24px
- Header row:
  - Left: "Refine the evidence set" — H3
  - Right: "Post-entry filters" — tag pill, bg `#E8E4DE`
- Filter Pills Row:
  - Active pill: "Visual" — bg `--nav-active`, text `#FFFFFF`
  - Inactive pills: "Textual", "Schema", "Simulation", "8:45 - 9:30 PM", "West zone", "High confidence first"
  - Style: same as Incident Entry filter pills

---

## 6. Page 4: Timeline + Schema

### Page Background
- Color: `--bg-page`

### Layout
1. Global Header (Master)
2. Global Navigation (Master) — "Timeline + Schema" active
3. Main Card Container

### Inside Main Card

#### Top Meta Row
- Left: "CHRONOLOGY + STRUCTURE" — 11px, `--section-label`, all-caps
- Right: "Timeline and schema stay tied to evidence, not abstracted away from it." — 12px, `--text-muted`

#### H2 Title
- "Follow how sub-events connect over time" — 28px Serif

#### Incident Progression Card (full width)
- White sub-card, border `--border-card`, radius 12px, padding 24px
- Header row:
  - Left: "Incident progression" — H3
  - Right: "Cross-modal timeline" — tag pill, bg `#E8F0E8`, text `#4A7C59`
- Timeline Visual (horizontal):
  - Line: Rectangle, 4px height, bg `--timeline-line`, spanning full width
  - 4 Event Nodes evenly spaced on the line:
    - Each node: Circle, 16px diameter, bg `--timeline-dot`
    - Below each node: Time label (e.g., "8:58 PM") — 12px bold, `--section-label`
    - Below time: Event name (e.g., "Derailment onset") — 13px bold, `--text-primary`
    - Below event: Sources (e.g., "Street camera + report excerpt") — 12px, `--text-muted`
  - Events:
    1. 8:58 PM — Derailment onset — Street camera + report excerpt
    2. 9:05 PM — Fire ignition — Witness report + image inference
    3. 9:22 PM — Smoke expansion mapped — Drone pass + thermal imagery
    4. 10:10 PM — Hazard zone widened — Simulation revision + official memo

#### Bottom Two-Column Grid

**Left Column (~60%): Schema Relationships Card**
- White sub-card, border `--border-card`, radius 12px, padding 24px
- Header row:
  - Left: "Schema relationships" — H3
  - Right: "Entity graph" — tag pill, bg `#E8E4DE`
- Graph Canvas:
  - Rectangle: full width, 280px height, bg `#FAFAF8`, radius 8px
  - Nodes (text widgets or small rectangles with radius 999px):
    - "Derailment" — pill, bg `--nav-active`, text `#FFFFFF`, left side
    - "Ignition" — pill, bg `#FFFFFF`, border 1px `--nav-inactive-border`, center-top
    - "Smoke Spread" — pill, bg `#FFFFFF`, border 1px `--nav-inactive-border`, center
    - "Evacuation" — pill, bg `#FFFFFF`, border 1px `--nav-inactive-border`, center-right
  - Connection Lines:
    - Use horizontal/vertical lines, 2px, dashed (`--timeline-line`)
    - Derailment → Ignition (solid line)
    - Ignition → Smoke Spread (dashed line)
    - Smoke Spread → Evacuation (dashed line)
    - Derailment → Smoke Spread (dashed curved line — use a diagonal line widget, 1px, dashed)

**Right Column (~40%): Helper Panel Card**
- White sub-card, border `--border-card`, radius 12px, padding 24px
- Header row:
  - Left: "What this view is for" — H3
  - Right: "Reduce fragmentation" — tag pill, bg `#F5E6E6`, text `#B85C38`
- Bullet List:
  - Three text rows, each with a bullet (●) prefix
  - 14px, `--text-secondary`, line-height 1.8
  - Items:
    1. "Compare conflicting time claims across source types."
    2. "See which sub-events are supported by which evidence clusters."
    3. "Decide when a spatial review in VR adds value."

---

## 7. Page 5: VR Scene Review

### Page Background
- Color: `--bg-page`

### Layout
1. Global Header (Master)
2. Global Navigation (Master) — "VR Scene Review" active
3. Main Card Container

### Inside Main Card

#### Top Meta Row
- Left: "SECONDARY SPATIAL MODE" — 11px, `--section-label`, all-caps
- Right: "Best for spatial context, not broad information foraging." — 12px, `--text-muted`

#### H2 Title
- "VR supports scene reconstruction and hazard review" — 28px Serif

#### Two-Column Grid

**Left Column (~55%): Spatial Evidence Review Card**
- White sub-card, border `--border-card`, radius 12px, padding 24px
- Header row:
  - Left: "Spatial evidence review" — H3
  - Right: "Reconstructed scene" — tag pill, bg `#E8F0E8`, text `#4A7C59`
- Scene Canvas:
  - Rectangle: full width, 340px height, bg `#4A5568`, radius 8px
  - Simulated 3D scene (use a dark gradient placeholder image, or dark rectangle with subtle gradient)
  - Floating Labels (positioned absolutely within card):
    - "Smoke drift" — pill, bg `rgba(255,255,255,0.15)`, text `#FFFFFF`, border 1px `rgba(255,255,255,0.3)`, top-center
    - "Ignition zone" — pill, bg `rgba(255,255,255,0.15)`, text `#FFFFFF`, border 1px `rgba(255,255,255,0.3)`, center
    - "Railcar cluster" — pill, bg `rgba(255,255,255,0.15)`, text `#FFFFFF`, border 1px `rgba(255,255,255,0.3)`, left-center
    - "Response vehicle" — pill, bg `rgba(255,255,255,0.15)`, text `#FFFFFF`, border 1px `rgba(255,255,255,0.3)`, bottom-right

**Right Column (~45%): Selected Object Panel Card**
- White sub-card, border `--border-card`, radius 12px, padding 24px
- Header row:
  - Left: "Selected object: ignition zone" — H3
  - Right: "Confidence: medium" — tag pill, bg `#F5E6E6`, text `#D4A017`
- Explanation Paragraph:
  - Text: "AI highlights the current selection with a subtle visual outline and proposes related evidence for verification rather than asserting a single truth."
  - 14px, `--text-secondary`, line-height 1.6
- Info Rows (3 sections, 16px gap):
  1. **Linked sources**
     - Label: 12px bold, `--text-primary`
     - Text: "Drone pass 2, camera 05, official fire log" — 13px, `--text-secondary`
     - In a bg `#FAFAF8` box, padding 12px, radius 6px
  2. **Possible relation**
     - Label: 12px bold, `--text-primary`
     - Text: "Connected to derailment onset and smoke spread sequence" — 13px, `--text-secondary`
     - In bg `#FAFAF8` box
  3. **Suggested next step**
     - Label: 12px bold, `--text-primary`
     - Text: "Open west-zone timeline overlay or compare report discrepancy" — 13px, `--text-secondary`
     - In bg `#FAFAF8` box

#### Why VR Is Included Row (bottom, full width)
- White sub-card, border `--border-card`, radius 12px, padding 24px
- Header row:
  - Left: "Why VR is included" — H3
  - Right: "Client-aligned future direction" — tag pill, bg `#E8E4DE`
- Three Equal Columns:
  - Each: white sub-card, border `--border-card`, radius 8px, padding 16px
  - Column structure:
    - Title: 14px bold, `--text-primary`
    - Description: 13px, `--text-muted`
  - Items:
    1. "Scene review" / "Understand where evidence sits in physical space"
    2. "Hazard interpretation" / "See danger zones, spread, and routes in context"
    3. "Training value" / "Supports after-action learning without replacing the dashboard"

---

## 8. Interaction & Dynamic Panel Strategy

### Tab Switching (Global)
Each page is a **separate Axure page** (not dynamic panel states) because the URL should change per tab for prototype realism.
- Navigation pills: **Global Master** with raised events or just duplicate the nav row on each page with the correct active state.
- **Alternative:** Make the nav a Dynamic Panel with 5 states (one per active tab) and place it on each page in the correct state. This is cleaner for maintenance.

### Search Input (Incident Entry)
- OnTextChange → Show/Hide suggested queries based on input length (if > 0, show; else hide)
- OnClick of suggested query → Set text of search input to that value

### Recent Investigations List
- Use a **Repeater** widget for the list.
- OnItemLoad → Set text of Title and Subtitle from repeater dataset.
- OnClick → Open Link → Event Overview page (pass incident name via global variable if needed).

### Supporting Sources (Event Overview) & Evidence List
- Use **Repeater** widgets.
- Dataset columns: `Title`, `Detail`, `Type` (optional)

### Timeline Nodes
- Each node can be a **Group** containing: Circle + Time text + Event text + Sources text.
- OnClick of node → Scroll to Evidence Review / highlight related evidence (advanced: use global variables).

### Schema Graph
- Static widget placement is fine for the prototype.
- If you want interactivity: Make each node a button that shows a tooltip (dynamic panel) with node details OnMouseEnter.

### Evidence Review Annotation Labels
- These are text widgets or small rectangles floating on top of the image.
- OnMouseEnter → Show tooltip panel with annotation details.

### VR Scene Labels
- Same as evidence annotations: floating labels on top of dark canvas.
- OnClick → Update "Selected object" panel on the right with corresponding object data.
- **Dynamic Panel approach for right panel:** Create a dynamic panel with multiple states (one per selectable object: ignition zone, smoke drift, railcar cluster, response vehicle). Clicking a label sets the panel to the matching state.

---

## 9. Asset Checklist

### Images Needed
- [ ] Thermal/drone image for Evidence Review (dark with orange heat signature)
- [ ] Dark gradient/scene image for VR Scene Review (simulated 3D reconstruction)
- [ ] Optional: Header logo/icon if desired

### Icons (use Font Awesome or Material Icons if embedding HTML, or import as SVG)
- [ ] Search icon (Incident Entry search bar)
- [ ] Optional: Chevron or arrow icons for navigation cues

---

## 10. Quick Build Order (Recommended)

1. **Set up Global Styles** in Axure: Save colors and text styles.
2. **Build Global Header** as a Master.
3. **Build Navigation** as a Master (5 states).
4. **Build Page 1 (Incident Entry)** first — it establishes the grid and card system.
5. **Duplicate Page 1** for Pages 2–5, then replace content per this spec.
6. **Add interactions** after all pages are structurally complete.
7. **Preview and adjust** spacing by comparing side-by-side with these screenshots.

---

*Specification based on MUMOSA Spring 2026 concept prototype screenshots.*
