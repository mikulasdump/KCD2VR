KCD2VR R184 - Experiments sorted into tabs
Marker: KCD2VR-R184-TABS-SORTED-20261109
Base: R183. Copy the files over the repo (same paths), then run build_release.bat.
No KCD2VR.ini in this zip. Keep your own. No setting or key changed, only
where the rows sit in the menu.

What changed (src/subsystems/experimental/live_settings_d39.h, src/vr_menu.cpp)
   - One page table (kPageRows + kPageInfo) instead of 8 hand-made extra
     pages. Every page knows its tab and has a short title.
   - Every tab can have as many pages as it needs. "< PAGE" and "PAGE >" at
     the top walk only the pages of the selected tab, and the middle shows
     "title n / N". A tab with one page shows no page buttons.
   - Every setting has exactly ONE row. The repeated rows are gone:
     OBJECTS page 3 copies, and alchemy / anvil copies on CRAFT+.
     153 settings, none lost.
   - Tab names: NPCS is now WEAPONS (the weapon in your hand), REPAIRS is now
     RANGED (bow, crossbow, gun, stones). Same tab places as before.
   - Chest back is VRIK+ page 1 row 1, next to body follows headset.

Where everything is now:
- **BODY 1/1: Body**: 6DOF body follows head, Roomscale movement, Body movement smoothing, Right-stick turn, Shoulder travel warning, Physical walking 1:1 trial, Leaning height threshold
- **VRIK 1/2: Hands**: Free-hand contact detail, Hand-to-hand collision, Native hand bodies, Grab hand stays on controller, Finger contact trial, Extra hand filtering, Hand presentation timing, Elbow preference
- **VRIK 2/2: Elbows**: Continuous elbow target, Elbow trial: outward preference, Elbow trial: backward preference, Elbow pole hysteresis trial, Extra hand prediction trial, Contact stability trial, Free-hand contact budget fairness
- **ALCHEMY 1/1: Alchemy**: Tracked alchemy hands, Alchemy work mode, Alchemy pickup reach, Legacy work gestures, Craft reach trial, Legacy alchemy head trial, Alchemy tool contact trial, Alchemy release handoff trial
- **SMITH 1/1: Smithing**: Tracked smithing hands, Smithing work mode, Smith world tool collision, Anvil support, Smith tools follow hands, Station torso follows head, Station chest clearance
- **GEAR 1/2: Controls**: Controller layout, Quick access modifier, Shoulder holster layout, Knives: hip / chest pouch, Knives: chest pouch too, Index-finger pointing Quick Talk, Torch shoulder gesture, Visor temple gesture
- **GEAR 2/2: Holsters**: X tap = Follow, Hide weapons on your back, Hide all weapons at crafting tables
- **VIEW 1/2: HUD**: HUD at infinity (flat, never double), Combat direction cross, Minimal HUD, Tutorial messages, HUD: weapon/potion slots only while selecting, No black frames: hold last picture, Cutscene 16:9 bars, Eye colours: sRGB swapchain
- **VIEW 2/2: Map**: Lockpicking lock: solid, Static paper map, Paper map size, Map performance trial, Map second-hand support
- **OBJECTS 1/3: Interactions**: Item pickup reach, Physical interaction session, INSTANT interactions, INSTANT activities, Look selection from your eye, Held-object stow button, Dog petting, Face washing gesture trial
- **OBJECTS 2/3: Props**: Scenery props: size, Small props in the hand, Loose scenery props (swap), Held small props: firm grip, Prop impact sounds, Small-object wake lab, Right physical prop, Left physical prop
- **OBJECTS 3/3: Handover**: Pickup camera handover trial, Faster native pickup handover trial, First object placement trial, Dice: point to choose (trial)
- **WEAPONS 1/2: Weapon in hand**: Weapon drawn with the hand, Weapon in hand: depth, Fixed sword grip, Rigid primary grip trial, Same-cycle weapon placement trial, Grip anchored in free space, Swing power: arm, not wrist, Held-weapon wall slide
- **WEAPONS 2/2: Two hands**: Two-hand weight feel, Two-hand primary influence trial, Additional primary-hand influence, Two-hand release continuity, Current weapon arc coverage, Swing sound with your swing, Weapon grip release
- **COMBAT 1/4: Enemies**: Free targeting, Enemies block your swing, Attack step veto, Attack auto-turn veto, Global native dodge window, Hit reaction by body region, Include stamina-only hits, Weapon kicks vs enemies
- **COMBAT 2/4: Combos**: Combo: wait for the next swing, Combo: longest wait, Combo: keep the slot open, Combo: slot time, Combo: next direction help, Combo finish requirement, INSTANT masterstrikes / combos, Native paired backup
- **COMBAT 3/4: Directions**: Attack direction, Cut from below, Stab direction, Stab follow-through trial, Masterstrike reaction trial, Your finishers / masterstrikes, NPC finishers / combos on you, Punch stroke trial
- **COMBAT 4/4: Clinch + impale**: Player clinch initiation, Player clinch distance, Unarmed clinch trial, Impale: fast death, Impale: blade goes in fully, Physical corpse dragging, Corpse search, Shield block box
- **RANGED 1/2: Bow + crossbow**: Physical bow shot direction trial, Bow arm-line aim, Held arrow orientation, Stowed bow visibility, Crossbow / gun: RT alone shoots, Hand-driven crossbow reload, Crossbow reload route, Fast reload transitions
- **RANGED 2/2: Hand pull**: Heavy crossbow: pull string by hand, Medium crossbow: pull string by hand, Heavy crossbow crank travel, Stone throw (crouched)
- **VRIK+ 1/1: VRIK body**: Chest under the view: back, Body follows headset (trial), Body follows: free look, Body height, Horse riding VRIK, Start in VRIK, Start at Henry's eye height, Brief missing-body-pose hold
- **CRAFT+ 1/1: Crafting**: Alchemy: select what you look at, Alchemy transfer speed, Smith stroke recovery, Smith LT anvil assist, INSTANT crafting

Tests:
- New: menu_layout_r184 36/36 (every setting once, 153 rows, every tab
  walks its own pages and back).
- Updated to the new places: hud_infinity, minimal_hud, scenery_props,
  grip depth, training, tutorial, start_vrik.
- Fixed: start_facing (my R181 0.5 s settle had broken one of its checks).
- Settings checker: now reads the new page table, 0 mismatches.
- vr_menu syntax check: same error count as the baseline (shims).
- live_settings_d39_test and menu_settings_d65_test do not build. They did
  not build before this patch either (old removed fields, a missing .inc
  file).
