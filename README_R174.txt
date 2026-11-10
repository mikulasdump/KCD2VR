KCD2VR R174 - hands stay with the view + bow fix 2 (source patch, on top of R171, includes R172 + R173)
=====================================================================================================

Copy over your source folder (20 files, full replacements), then build_release.bat.
Marker: KCD2VR-R174-HANDS-WITH-VIEW-20261109

1. Hands moved with Henry when he turned under you (body follow R173, and FREE PLUS fights)
------------------------------------------------------------------------------------------
How it was: the view and the body frame (head, hands, weapon) were each held on their own.
* view: from the camera turn
* body: from Henry's root turn
R173 only counted a root turn in the same frames as a camera turn, and only within 150 ms of our stick input.
The game turns the root AFTER the camera (turn lag). So the late part of the root turn was not held out, and the
hands went with Henry.

Now there is ONE body yaw, and it follows the view (src/vrik/body_follow_r173.h bodyYaw):
  body yaw = the yaw held out of the view (R133 + R173)
           + how far the root has turned against the camera since that hold began
So head and hands keep the same relation they have with no hold at all, whatever the root does and however late.
* No hold (view offset 0) = body 0 = exactly the old calibrated frame, nothing changes there.
* While the game turns Henry to the enemy in FREE PLUS it is the same as before: the same numbers when the root
  and the camera turn together. Only when they turn apart do the hands now stay with the view.
* Files:
  - src/camera_hook.cpp computes it once per camera apply, next to the R133 / R173 hold.
  - frik_pose_probe.cpp (CalFrameR133) and vrik/weapon_tracking.inl use it instead of offsetBody + appliedBody.

2. Bow still used the game's aim (your 19:30 log: 3 of 3 "reason=NO_NATIVE_CHARGE_ACTION")
-----------------------------------------------------------------------------------------
* bow_charge_observer.inl: Henry's charge action was remembered only when the physics lock was free. Now it is
  always remembered (it is only the pointer, after the type and actor check).
* native_bow_commands.inl / bow_physical_aim_d65c.h: the arrow direction no longer needs the charge action at all.
  Only the old "exact action" bind needs it. The R172 pending shot path aims without it.

Not changed: "Extra hand prediction trial" - you said it did not help. It stays OFF by default. Use OFF.

Tests
-----
* body_follow_r173_test 33/33. New: root lags the camera = hands stay; root catches up = same as the view;
  no root read = keep; hold gone = 0.
* bow_release_lifecycle_d65_test 88/88. New: no charge owner = no exact bind, but the shot still aims.
* training_vanilla_r163_test 116/116, tutorial_native_r159_test 72/72, settings checker 0 mismatches.
* camera_hook / frik_pose_probe / action_time_owner syntax check: same old shim error counts, none new.

Test: body follow ON. Hold your hands still in front of you and turn your head 90 deg. Henry turns, your hands should
stay where the controllers are. Same in a FREE PLUS fight while he turns to the enemy. Bow: aim off to the side of where
you look and shoot.
