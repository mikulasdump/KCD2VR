KCD2VR R183 - chest back under the view
Marker: KCD2VR-R183-CHEST-BACK-20261109
Base: R182. Copy the files over the repo (same paths), then run build_release.bat.
No KCD2VR.ini in this zip. Keep your own. The new key works without it
(default 4 cm), and the menu writes it into your INI when you change it.

Chest under the view: back (VRIK+ page 2 row 6, VrChestBackCmR183, default 4 cm)
   - Miki: "the chest is just sticking out, too much ... move it like 3-5 cm
     back".
   - The camera is placed from the calibrated head anchor
     (tracked_frame51 eye). The VR body hangs from the solved head target.
     Both sat at the same point, and that left Henry's chest in front of your
     view when you look down or move.
   - Now the head target, and with it the whole trunk (chest, shoulders,
     belt), sits this many cm further back. The direction is horizontal,
     along the calibrated body forward.
   - The view does not move. Your controller hands do not move, they still
     come from the head anchor.
   - Values: OFF / 2 / 3 / 4 / 5 / 6 / 8 / 10 cm. Live, the next pose uses it.
   - Not while the dice body owns the pose.
   - The row replaces "HUD event-only updates" (a performance trial). That
     one is INI only now: VrHudWalkoffD70.
   - Files: src/vrik/chest_back_r183.h (new),
            src/subsystems/vrik/pose_tracking_goals.inl (head target),
            settings files.

Tests: new chest_back 6/6, tutorial 84/84, training 116/116.
Settings checker: 0 mismatches.
frik_pose_probe syntax check: same 38 shim errors as the baseline.
(live_settings_d39_test and menu_settings_d65_test do not build. They did
not build before this patch either: removed R171 fields and a missing .inc
file.)
