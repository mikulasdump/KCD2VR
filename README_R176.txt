KCD2VR R176 - stone throwing + chest dagger removed (source patch, on top of R175)
=================================================================================

Copy over your source folder (18 files, full replacements), then build_release.bat (it builds the new pak too).
Marker: KCD2VR-R176-STONES-NO-CHEST-DAGGER-20261109

1. Stone throwing (src/subsystems/throw_drop/stone_throw_r176.h, new)
---------------------------------------------------------------------
The game's own distraction throw (defaultProfile.xml):
  * crouched, RB press = distract_init (the aim)
  * RB release = decoy_throw
  * A = decoy_abort
No VR button sent RB in normal play. Now:
  * crouched: the game's "crouch" or "distract" action map is enabled. Read by the view lens, the same named-map
    reader as the look / follow prompts (activity_lens59c10r1.cpp).
  * EMPTY left hand (no item), raised to about head height: no more than 20 cm below the headset
    (frik_pose_probe StoneThrowPoseR176)
  * then left Grip = RB held = the game shows the stone aim. Let go = RB up = the throw. A cancels.
  * The knife pouch (hip / chest) and grabbing low are below that height, so they stay as they are.
  * Standing up you get no RB from this (RB is Quick Speak there).
  * Decided at the start of each Grip press, ends with the release; not in menus / chooser / dialogue / activities.
  * Log row (F-recording): "R176 STONE_THROW left grip -> RB held ..." / "RB released (the game throws)".
  * You need stones in the inventory, like in the normal game.
Switch: Experiments GEAR page row 2 "Stone throw (crouched)" (VrStoneThrowR176, default ON, live).

2. Chest dagger removed (no texture, never worked)
--------------------------------------------------
* DLL: VrChestDagger is not read any more, always OFF. Its per-frame tick never runs. The COMBAT/INTERACTION menu
  button "CHEST DAGGER" is gone and so is its Experiments row (GEAR page row 2 is now the stone throw).
* Companion pak: zzkcd2vr_chest_sheath_d33.lua and the 3 sheath model files are no longer packed
  (tools/companion_sources_d39.py, tools/package_release_helpers_r19.py). Checked: the new pak = R170 pak minus
  those 4 files, nothing else changed.
* The knife pouch at the chest (VrKnifeChestZoneR109) still works, only the visual is gone.

Tests
-----
* stone_throw_r176_test 17/17 (new).
* tutorial_native_r159_test 75/75 (GEAR row 2, no chest dagger row), training_vanilla_r163_test 116/116.
* melee_grip_depth, scenery_big_props, minimal_hud, hud_infinity, start_vrik: still pass.
* Settings checker 0 mismatches. activity_lens59c10r1.cpp compiles clean here.
* frik_pose_probe: same 38 old shim errors.
