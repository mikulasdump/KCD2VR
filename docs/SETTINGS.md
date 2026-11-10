# Settings

most tuning is in the VR menu (F7). the INI stores those choices. every key in KCD2VR.ini has a comment that says what
it does now, grep the key name there. the Experiments pages need Developer mode (setup window).

keep your INI and calibration when you update. an old key can have a number in its name (VrMotionTrialD67,
VrCrossbowReloadRouteR96 ...) because renaming it would throw away a saved choice. the number is the version that added
it, not something you need to know.

the INI is saved on exit. its values are the state at exit, not a timeline of what was active earlier. a feature i
tested and called working is ON when its key is missing, a saved OFF stays OFF. updating never replaces your INI.

values below marked "shipped" are what the KCD2VR.ini in the source root has.

## Experiments: where each setting is (R184)

every setting has one row. a tab with more pages: PAGE < > at the top walks them. NPCS is now WEAPONS, REPAIRS is now RANGED.

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
- **COMBAT 1/5: Enemies**: Free targeting, Enemies block your swing, Attack step veto, Attack auto-turn veto, Global native dodge window, Hit reaction by body region, Include stamina-only hits, Weapon kicks vs enemies
- **COMBAT 2/5: Free targeting +**: FREE PLUS: turn limit
- **COMBAT 3/5: Combos**: Combo: wait for the next swing, Combo: longest wait, Combo: keep the slot open, Combo: slot time, Combo: next direction help, Combo finish requirement, INSTANT masterstrikes / combos, Native paired backup
- **COMBAT 4/5: Directions**: Attack direction, Cut from below, Stab direction, Stab follow-through trial, Masterstrike reaction trial, Your finishers / masterstrikes, NPC finishers / combos on you, Punch stroke trial
- **COMBAT 5/5: Clinch + impale**: Player clinch initiation, Player clinch distance, Unarmed clinch trial, Impale: fast death, Impale: blade goes in fully, Physical corpse dragging, Corpse search, Shield block box
- **RANGED 1/2: Bow + crossbow**: Physical bow shot direction trial, Bow arm-line aim, Held arrow orientation, Stowed bow visibility, Crossbow / gun: RT alone shoots, Hand-driven crossbow reload, Crossbow reload route, Fast reload transitions
- **RANGED 2/2: Hand pull**: Heavy crossbow: pull string by hand, Medium crossbow: pull string by hand, Heavy crossbow crank travel, Stone throw (crouched)
- **VRIK+ 1/1: VRIK body**: Chest under the view: back, Body follows headset (trial), Body follows: free look, Body height, Horse riding VRIK, Start in VRIK, Start at Henry's eye height, Brief missing-body-pose hold
- **CRAFT+ 1/1: Crafting**: Alchemy: select what you look at, Alchemy transfer speed, Smith stroke recovery, Smith LT anvil assist, INSTANT crafting

## hands and weapons

**Motion timing** (`VrMotionTrialD67`, Home > VRIK EXPERIMENTS, or EXPERIMENTS / LIVE SETTINGS > VRIK > "Hand
presentation timing"). applies and saves at once, no restart.

| choice | value | what |
| --- | ---: | --- |
| BASELINE motion | 0 | the old baseline timing. |
| FRESH POSE timing | 2 | fresh-publication token pacing. |
| ELBOW reach preference | 3 | the elbow comparison timing. |
| Fresh Pose Uncapped | 4 | no native-frame XR-token wait, one extra pose-only refresh before the native update. shipped, and the default when the key is missing (since R124). |

Uncapped samples head and grips at the current OpenXR time with the NOW camera source, with the predicted camera
source it uses the latest runtime target. the head in that sample is only the hands' reference, it never replaces the
camera head. a busy lock or a stale target falls back to the last completed sample with its real age. "uncapped" means
no token pacing from the mod - it does not override an FPS limiter, the headset refresh or the real CPU/GPU cost.
Shift+F7 records it without turning the token wait back on.

**Elbow preference** (Experiments > VRIK, last row): D86 DEFAULT, R89 HAND-GUIDED (`VrElbowSolverR89`), FRIK TRIAL
(`VrFrikElbows`, shipped ON) and ELBOW REACH PREFERENCE (`VrElbowPreferenceD67`, the old D67 bottom elbow). when the INI
has more than one of them on, the solver takes them in this order: hand-guided, reach preference, FRIK. hand-guided points the elbow from your hand: behind the wrist along the hand and on the
little finger side, mixed with the body pole and pushed out of the chest. one solver for free arms and bow arms, it
keeps its history when a comparison changes so the elbow does not snap. none of them is "the natural one" yet.

keep the wrist on the controller. arm reach goes through the body and shoulders with fixed arm lengths. smoothing that
makes the hands trail behind is not a fix for timing.

two-hand primary bias (`VrTwoHandPrimaryBiasD65`) says how strongly the main hand leads a two-handed weapon (shipped 0.40). grip offsets are visual
alignment, not a way to get more attack reach.

**Two-hand weight feel** (`VrTwoHandWeightFeelR90`, Experiments > GEAR, OFF). ON lets the weapon weight slider
(`VrWeaponWeightFeel`, lower = heavier) also slow down how fast a two-handed weapon turns. the two-hand stop without
spring-back works either way.

## contact

Experiments > VRIK > Free-hand contact detail:
- NORMAL: five-point palm contact.
- PRECISE: palm/finger contact with finger fitting.
- BALANCED: five palm points plus thumb/index tips, 2 mm clearance, no finger fitting. same 18-ray / 1 ms query limit.

free controller motion is not filtered. station contact, hammer, tongs and the held workpiece have their own simpler
contact. none of these create rigid bodies.

## objects and corpses

- **Small props in the hand** (`VrStaticPropGrabR89`, Experiments > Objects page 2, shipped ON): grab small scenery
  props. two hands can't claim the same entity.
- **Loose scenery props (swap)** (`VrSceneryPropSwapR100`, Objects page 3 since R159, shipped ON): Grip on a mug/plate/candle that is
  level mesh makes it a loose prop you can hold. only where the mesh has a usable path.
- **Scenery props: size** (`VrSceneryBigPropsR158`, Experiments > Objects page 3, shipped ON since R166 - Miki's INI; a missing key = OFF): SMALL = up to 65 cm
  across, same as before. UP TO 1 M also takes stools, baskets and small crates. mass stays capped (1.5 kg loose copy,
  3 kg scenery) so a big one carries like a light box, one hand only. reach to the box centre grows to max 55 cm
  so you can grab it at the edge
- **Weapon in hand: depth** (`VrMeleeGripDepthR168`, WEAPONS page 1 row 2, R168, shipped 0.5 since R190): moves sword /
  axe / mace / dagger inside the hand, + = deeper (toward the wrist / palm), - = further out, -2..4 cm in 0.5 cm steps,
  live. `VrWristFromGripCm` moves the whole hand with the weapon instead.
- **Corpse dragging** (Experiments > NPCS "Physical corpse dragging"): needs a dead body from the game and an empty
  hand, no recorder or physical combat needed.
- **Corpse search** (`VrCorpseSearchSqueezeR89`, VRIK page 2, only used while corpse dragging is ON). ALWAYS (shipped):
  every empty hand looks for a body 10 times a second, so a fast squeeze on a limb is caught. ONLY ON SQUEEZE: the
  search runs only for 700 ms after the grip passes 0.15, no cost with open hands, but a squeeze that closes inside one
  frame can miss (squeeze again).
- dog petting (`VrPetGestureD59`) and riding hands are ON. petting is on Objects, riding hands on Body page 2. native
  mounting finishes, then VRIK starts after one second.

## crossbow reload

**Crossbow reload route** (`VrCrossbowReloadRouteR96`, Experiments > INSTANT, last row, needs
`VrCrossbowReloadD65C=true`). HAND ONLY (shipped, the bow pattern): the game's reload request waits while your hands do
it.
- light crossbow: hold it in the left hand, grip the string with the right hand and pull it back (about 9 cm of
  string travel).
- goat's foot / cranequin: grip at the chest takes the game's real spanner into the right hand (native cocking action,
  driven by the hand). heavy and medium can also grip the string at the front and pull it back by hand (R111/R117
  switches in the INI).
- all of them: grip behind the right shoulder takes a bolt into the right hand (same native factory and same hand
  writer as the bow arrow), open the hand on the rail to load.
- if the hands stay idle for 1.5 s after the game asked for a reload, the game reloads by itself.
- an incomplete pull, regripping, or a bolt released away from the rail pauses the same reload, it does not finish it.

NATIVE-DRIVEN (false) is the older route where the hands ride the game's reload animation. the old key
`VrCrossbowHandOnlyR89` is not read anymore. Instant reloads changes only the speed of the native tail, not who does
the reload.

## crafting and instant actions

Native keeps the original interaction. Semi keeps selected tool phases. Free and 6DOF use tracked movement where they
connect. instant interactions, crafting, fast reload transitions and combat have separate switches.

the alchemy 8x setting is a ceiling for Henry's pickup/return and pre-work turning, not a multiplier for the whole
station. book and station changes, distillation, pouring, grinding, bellows and the native work/effects keep their own
timing. speed does not replace a missing ownership or replay an inventory effect.

head hover can show ingredient names while the hand targets something else. looking is presentation, gripping is the
action.

crafting prompt size: **Crafting / minigame icon size** slider on the VR HUD page (`VrMinigamePromptScale`, shipped 0.4). at a
station the size is sent again every second. Experiments > Immersion has CRAFT HINTS sizes and TIMED HINTS (hints
show for ten seconds at station entry or a new work step, look at an open palm to see them again). Immersion > SHOW
HINTS is the game's own wh_ui_ShowHints, hints off wins over the timer.

**Smith LT anvil assist** (`VrSmithSemiAnchorsD67`, shipped ON): hold the left Trigger at the anvil for the original
anvil placement of the workpiece, release = the left hand is tracked again. only at the anvil in 6DOF. no effect on
the right-hand hammer.

**Dice: point to choose** (`VrDiceHandsD85`, Experiments > Objects page 2, OFF). when it is your turn to pick dice,
point the right hand at a die and the game's dice cursor jumps to it, then A keeps/unkeeps it like on the gamepad. X, Y,
B and the stick work as before. the row says why it falls back to the gamepad. taking, shaking and putting down the cup
is still Henry's own animation.

## combat

stab follow-through, unarmed clinch and masterstrike reactions have their own switches on the second combat page in
Experiments.

punch travel is in metres. more travel = fewer tiny false punches, but short real punches can be rejected. walking and
pulling a fist back must not count as a strike. swing and thrust sensitivity fail in different ways, one energy setting
does not explain every missed hit.

combo and defence options leave eligibility, perks, stamina, armor and results to the game. a visible reaction is not
proof that a named combo was accepted.

NPC defence by skill (`VrDirectNpcBlockBySkill`, shipped OFF since R190, Miki's hardcore setup): block raise, parry raise and dodge clip speed come from the
NPC's own melee skill. it also rolls once per swing if your opponent defends: 40 % at skill 0 (R177, was 25 %), 75 % at skill 30 and
more (the game's wh_cs_ForceBlock, only in a real fight: Henry in combat, no dialogue / cutscene / menu; R155). `VrDirectNpcBlock` 1 = 50 % at skill 0 .. 85 % at skill 30 (R177, was 65 %), 2 = always,
3 = always perfect block. never with a practice / quest weapon in your hand (the wooden sword of the prologue sparring, R156): the sparring
partner is scripted to let you hit him, he is not forced to block. `VrDirectNpcBlock` 4 = OFF (R178): the roll never runs, enemies block only when the game itself decides to (the BY SKILL raise speed still applies to those). Experiments NPCS row 7 cycles all five.

**Combo help** (R171, Experiments > VRIK page 2 rows 4-5 and NPCS page rows 3, 4, 8, the same switches as COMBAT FLOW > COMBO HELP):
- `VrComboRecoveryHold` (default ON since R171) "Combo: wait for the next swing": after a hit that opens the game's combo slot, Henry's
  attack waits at its end until you swing again (a manual block, the slot closing or the time ends it). in the 8 Nov 12:57 log both
  presses that came inside the slot were eaten (`release=UNSEEN`), the attack was already over. `VrComboRecoveryHoldMs` "Combo: longest wait" 0.25-1.5 s.
- `VrComboWindowExtend` "Combo: keep the slot open" + `VrComboWindowSeconds` "Combo: slot time" 0.5-3 s (the game's own slot is 0.5 s).
- `VrComboZoneAssist` "Combo: next direction help".

**FREE PLUS: turn limit** (R186, `VrFreePlusTurnLimitR186` default 90 deg, Experiments COMBAT page 2, live): FREE PLUS
took every game turn out of the view, also the game's 180 deg turn toward an attacker behind you, so Henry faced away
from you and his arms reached behind him (crossed). now the game's turn is taken out of the view only while Henry stays
within this angle of where your head looks; the rest turns your view with him like the native game. turning your head
toward him lets the hold work again; turns back toward you are always held. your own stick turns are never limited.
0 = off (old), 60 / 75 / 90 / 110 / 135.

**Chest under the view: back** (R183, `VrChestBackCmR183` shipped 10 cm since R190 (code default 4), Experiments VRIK+ page 1 row 1, live): the
camera comes from the calibrated head anchor, the VR body hangs from the solved head target. that target now sits this
far behind the view (horizontal, along the calibrated body forward), so chest, shoulders and belt hang back under the view
and do not stick out when you look down or move. the view and the controller hands do not move. 0 = old (0..10 cm).
R185 (bug fix, any setting): the trunk follows your reach - hands within 30 cm in front of your head = this setting
back, hands 60 cm or more in front = 5 cm FORWARD (shoulders lead the arms like a real reach), smooth between.
the row replaces "HUD event-only updates" (`VrHudWalkoffD70`, INI only now).

**Weapon drawn with the hand** (R180, `VrWeaponLeadR180` default 1, Experiments OBJECTS page 3 row 6, live): the mod
computes hand and weapon from one pose sample, but the game draws the skinned hand one pose cycle newer than the held
weapon, so the handle trailed the fingers while you moved. the held sword / axe / mace / dagger is now drawn ahead
along its own last-cycle motion by this many cycles (0 off, 0.5, 1, 1.5, 2; capped 10 cm / 30 deg). display only:
contact and hits use the real weapon pose. R188: every held item (bow, crossbow, gun, shield, torch, knife), not only
sword / axe / mace / dagger; the bow and crossbow aim and shot still use the real pose. R178 "Weapon turns in the fist" (`VrWeaponHandlePivotR178`) is retired.

**Palm look** (R192, `VrPalmLookR192` default ON, VR HUD page tick, IMMERSION AND CONTROLS page 3, Experiments VIEW
page 2 row 6, live): the open-palm glance (left palm = compass, right palm = health / stamina). it works only in the
IMMERSIVE or MINIMAL HUD mode, never in NORMAL. before R192 the glance WAS the Immersive HUD switch (`VrContextualHud`),
so ticking it turned a NORMAL HUD into IMMERSIVE. IMMERSIVE with palm look OFF = the full HUD (nothing to hide);
MINIMAL with palm look OFF = compass / health never shown.

**IMMERSION AND CONTROLS** (R192, player menu, no new keys except palm look): 4 pages. Body + movement: HMD based body
rotation (OFF / 30 / 45 deg = `VrBodyFollowHmdR173` + `VrBodyFollowDegR173`), free targeting (FREE / FREE PLUS =
`VrFreeTargetingModeR133`), hand smoothing (UNCAPPED 4 / CAPPED 2 = `VrMotionTrialD67`), body height, turning, walking,
crouch, vignette. Controls + hands: controller scheme (`VrControllerLayoutR118`, also sets `VrPadGlyphStyle`: PS VR2 =
PlayStation icons) and the old rows. "Button icons" and "Immersive HUD" / "Minimal HUD" rows are folded in.
R193 Home (player): HUD, VR CONFIG, GRAPHICS, IMMERSION AND CONTROLS, COMBAT, then VRIK / NATIVE CONTROLS and
RECALIBRATE VRIK; the left page has MONO / STEREO only. the HUD rows are only on the HUD menu now; COMBAT shows the
Enemies page (enemy combat, fast block) until the Combat step.

**Home > COMBAT** (R194, player menu): three tabs.
ENEMIES: "Enemy difficulty" (`VrEnemyPresetR194`, also Experiments COMBAT "Free targeting +" page row 2): NORMAL = the
beta (R139) starter values (VrNpcDifficulty 43, VrNpcDefenseSpeed 1.75, VrNpcBlockSpeed 1.0, VrNpcBlockAnimSpeed 1.4,
VrDirectNpcBlockBySkill true with VrDirectNpcBlockSpeedLow 2 / High 3, VrDirectStaggerRule 1, VrDirectNpcBlock 0),
HARDCORE = the R190 values (32, 1.7, 1.45, 1.55, by skill false, stagger rule 2, block rate 0), CUSTOM = keeps the
values; plus "Fast block response". CUSTOM: block rate (VrDirectNpcBlock: by skill / often / always / perfect / off),
difficulty slider 0..100 %, block speed BY SKILL on/off, low skill and high skill block speed sliders x1.0..x3.0 (turn BY
SKILL on), all-enemies block speed x1.0..x3.0 (VrNpcDefenseSpeed, turns BY SKILL off); any change there = CUSTOM.
SENSITIVITY opens the weapon sensitivity page (the old developer "WEAPON / FIST SENSITIVITY" page, with its STABS, FISTS,
BLOCK and SHIELD BOX pages), now open to players; BACK returns to COMBAT.

**Hands stay with the view when walking** (R191, `VrBodyLeadR191` default 1, Experiments VRIK page 2 row 8, live): the
Shift+F2 log showed Henry's root moves one walking step between the pose and the weapon placement, and the skinned
body is drawn on the newer root, while the camera eye and the held weapon used the older one. so walking forward the
hands (and the chest) slid forward against the view, walking back they slid back, the weapon stayed on the controller.
the eye and the drawn weapon now sit on the root predicted this many steps ahead (the last step x value; R193: position
only, no turn lead, and the step smoothed over about three frames so a long frame does not spring the hands): -1, -0.5, OFF, 0.5, 1, 1.5, 2. pick the value where the hands stay still while walking. standing still changes
nothing; a step over 25 cm or 20 deg (teleport, cut scene) is ignored. display only: hits, contact and aim unchanged.

**Swing power: arm, not wrist** (R178, `VrSwingPowerArmR178` default ON, Experiments OBJECTS page 3 row 7): the hit
energy came only from the struck point's speed, so a wrist flick hit as hard as an arm swing. now the wrist's speed
against the struck point's speed scales it: 0.30 or more (arm swing, thrust) = full, 0.08 or less (pure flick) = 45 %
of the energy, smooth between. the game's RPG / armour still decide the damage. the GEOM_IMPACT log line shows
`arm=` and `hand/point=`. OFF = old.

**Stone throw (crouched)** (R176, `VrStoneThrowR176` default ON, Experiments GEAR page row 2): the game's distraction throw is crouch
+ RB (aim), RB release (throw), A (cancel); no VR button sent RB in plain play. now: crouched (the game's crouch / distract action map is
enabled), an EMPTY left hand raised to about head height (max 20 cm below the headset) + left Grip = RB held; let go = the throw. the
knife pouch (hip / chest) and grabbing low stay as they are. the carried chest dagger display (`VrChestDagger`) is removed: its model had
no texture and it never worked; its Lua and model are no longer in the companion pak.

**Crossbow / gun: RT alone shoots** (R175, `VrRangedRtFireR175` default ON, Experiments NPCS page row 5): with a crossbow or a gun in
hand a right trigger press sends the aim (LT) at once and the shot (RT) after 0.25 s; a short tap still fires at 0.25 s; the aim stays
0.15 s after the shot. a press that starts while you hold LT yourself is the game's LT+RT as before. (R187: the R167 left trigger ->
LB route for the look / Follow prompts is removed; X does them as before.)

**Body follows headset (trial)** (R173, `VrBodyFollowHmdR173` shipped ON since R190, `VrBodyFollowDegR173` 15/30/45/60 deg (shipped 30 since R190, code default 45), Experiments VRIK+ page 1 rows 2 and 7):
turn your head past the free look angle and Henry turns after it with the game's own stick turn, smooth (faster the further past),
until he is back at the edge. the same yaw goes out of the view and the VR body frame (the R133 FREE PLUS hold), so the world and
your hands stay. stick forward is then within the free look angle of where you look. paused in a locked fight (FREE PLUS holds
that), while R133 gives an offset back, while you turn with the stick; not in menus, dialogue, cutscenes, activity views or on a
horse. recenter / load = 0. if the first turn goes the wrong way it corrects itself within 3 deg (`flips`).
Extra hand prediction (`VrHandLeadD67C` + `VrHandLeadMsD69`) is one row now: OFF / 3..33 ms.

R171 removed: the tutorial modes (`VrTutorialNativeHitsR159`, `VrTrainingVanillaNpcR163`), lively NPC (`VrLivelyNpcD73`, `VrNpcListeningGesturesD76`)
and the dialogue NPC look at the headset (`VrDialogueNpcLookHmd`). the keys are not read any more, NPCs look and move the game's way.

## room movement and dodge

physical walking uses the measured horizontal HMD movement together with the stick. the older offset-follow option is
a different thing and not one-to-one room movement. F1 changes body follow only.

lean height (`VrRoomscaleLeanHeightD70`, metres below the recentered height, 0..0.12, shipped 0.12). lowering the
head past it means leaning, not walking. a level lean and a step
can't be told apart perfectly with only a headset. native collision can shorten a step at a wall. recenter does not
replay old movement.

headset dodge only asks for side or backward dodges, with a speed threshold, quiet rearm and cooldown. it should not
dodge forward when you lean into a punch. the game decides if the dodge is legal. physical walking and headset dodge
are separate buttons.

## HUD, map and glyphs

palm HUD: look at your open **left** hand for the compass, open **right** hand for health/vitals/buffs. the hand has to
face the headset and be looked at. trigger, grip, buttons or a held item cancel it. one second of looking, then it
shows max three seconds. closing or turning the hand, losing tracking or looking away hides it. it uses head
direction, not eye tracking.

more HUD switches are on the VR HUD page and in the INI: Minimal HUD (`VrMinimalHudR141`), HUD at infinity, combat
crosshair size, weapon/potion slots hidden until thumbrest + stick. each has its INI comment.

R160: without developer mode the VR HUD page is one simple page: **Icon size** (`VrUiUnifiedScaleR160`, 100% = every
HUD element as big as its own size setting, the crafting icons are not touched), **Crafting / minigame icon size**
(`VrMinigamePromptScale`), HUD mode IMMERSIVE (palm glance, `VrContextualHud`) / MINIMAL IMMERSIVE (`VrMinimalHudR141`) /
NORMAL (both off), eleven [X]/[ ] ticks (block message, tutorials, compass, status icons, lower icons, quick slots
fade, combat crosshair, dot crosshair, cinematic bars, lockpick solid, slow HUD follow) and the HUD distance. developer mode keeps the
old pages, INTERACTION / PROMPTS has the tutorial box position too (`VrUiTutorialYR160`, + = down, 0 = game place).

the map uses the held paper. quest markers come from the native compass subset, not every objective in the world.
map profiling needs F2 or Shift+F2.

Xbox and PlayStation glyphs change the pictures, not the button layout.

## optional gore

the gore button cycles OFF, HEADS ONLY, HEADS + LIMBS. it needs the separate gore pak, which is never merged into the
main one. main gameplay works without it. cosmetic cutting doesn't replace the game's death or damage checks.
LIMBS means arms only (forearm at the elbow), legs are never cut (R153). the extra wound cap meshes from the gore pak are
off by default, `KCD2VR_Gore.limbCaps=true` in the console turns them on again.

## FPS

`VrFpsCapEnabled=false`, `VrFpsCap=90` shipped. the cap value is only stored while the cap is off. for the smoothest
head motion cap the game at the headset refresh in the GPU driver (NVIDIA / AMD frame limiter).
