KCD2VR R175 - crossbow / gun: RT alone shoots (source patch, on top of R174)
============================================================================

Copy over your source folder (13 files, full replacements), then build_release.bat.
Marker: KCD2VR-R175-RT-SHOOTS-20261109

What it does (src/subsystems/ranged_reload/ranged_rt_fire_r175.h, new)
-----------------------------------------------------------------------
The game only fires a crossbow / gun while it aims (LT held). With a crossbow (class 10/14/15) or a firearm (13)
in Henry's hand and your left trigger NOT pressed:
  * RT press -> the mod sends LT (aim) at once, and RT only after 250 ms (the game has to be in the aim first)
  * a short tap that ends before 250 ms still fires: RT is sent for 80 ms at 250 ms
  * after the shot LT stays 150 ms, so the aim is not dropped in the same frame as the shot
  * a press that starts while YOU hold LT is passed through unchanged (LT+RT as before)
  * not in the VR menu, a system menu, the quick talk chooser, theater, dialogue choices, minigames / activities
  * decided at the start of each press, ends with the release; no latch

Also: the R167 "LT = LB at the look / follow prompt" is never used while a crossbow / gun is in hand
(src/mod.cpp, R167 blocked condition). If a follow / look prompt map was enabled near you, your LT went to the game
as LB, so it never aimed. That may be why it stopped working.

Switch: Experiments NPCS page row 5 "Crossbow / gun: RT alone shoots" (VrRangedRtFireR175, default ON, live).
It replaces the paused "Accepted-hit reaction lab" row, which was status only and could not be switched.
Files:
  * src/frik_pose_probe.cpp/.h: HeldCrossbowOrGunR175() from the same fresh equipment publication as the RT owner.
  * src/mod.cpp: the step right after the R167 block, on the pad that goes to the game. Log row (F-recording only):
    "R175 RANGED_RT_FIRE right trigger -> LT aim + RT shot (crossbow / gun)" / "released".
  * live_settings_d39.h / live_runtime_d40.inl / KCD2VR.ini / docs/SETTINGS.md / settings checker (0 mismatches).

Tests
-----
* ranged_rt_fire_r175_test 23/23 (new): hold, tap, own LT, OFF, no crossbow, menu (also mid press), hysteresis.
* tutorial_native_r159_test 73/73 (row 5), training_vanilla_r163_test 116/116.
* frik_pose_probe syntax check: same old 38 shim errors, none new.

If 250 ms is too short (the bolt does not fly the first time), tell me. Making the lead longer is one number.
