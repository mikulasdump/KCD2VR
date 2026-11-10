KCD2VR R191 - hands stay with the view when walking
Marker: KCD2VR-R191-BODY-LEAD-20261110
Base: R190. Copy the files over the repo (same paths), then run build_release.bat.
KCD2VR.ini is NOT in this zip. Keep your own game INI. The new key is added
with its default (1) the first time the menu saves.

What you saw
   Walking forward the hands slide forward, walking back they slide back a
   little. The weapon stays on the controller. The hands do not stay in the
   same place relative to the HMD.

What your Shift+F2 log shows (D68 ATTACHMENT, 586 samples, 252 while moving)
   - Between making the pose and placing the weapon, Henry's root moves one
     walking step: median 3.4 cm, max 5.1 cm (3.6 cm at 3.3 m/s, 0.7 cm at
     0.6 m/s).
   - The next pose starts exactly from that placement root, so the step is
     the game moving Henry once per frame.
   - The camera eye and the held weapon are placed on this root. The game
     draws the skinned body (hands, chest) one step further along.
   - Result: the body is ahead of the view when you walk forward and behind
     it when you walk back. With the stick turn it is turned a little further
     too. The weapon matches the view, so it looked right and only the hands
     moved. This is also the same "one cycle" R180 found for the handle.

What changed
   - The camera eye and the drawn weapon are now placed on the root predicted
     ahead by (last step x setting), position and turn. Eye, weapon and body
     then use the same root.
       src/vrik/root_lead_r191.h (new)
       src/subsystems/vrik/tracked_camera_service.inl (eye)
       src/vrik/physical_native.inl (weapon placement)
   - Standing still = no step = nothing changes.
   - A step over 25 cm or a turn over 20 deg (teleport, cut scene) is ignored.
   - Display only. Hits, contact, bow / crossbow aim and the game's position
     are not touched.

New setting (never refused, live)
   Experiments > VRIK tab > page 2 > row 8
   "Hands stay with the view when walking"
   INI: VrBodyLeadR191   default 1
   Values: -1, -0.5, OFF, 0.5, 1, 1.5, 2 steps

How to test
   1. Hold both controllers still in front of you and walk forward and back
      (also run).
   2. The hands should stay in place relative to your view.
   3. If they still slide forward while walking forward, try 1.5 or 2.
   4. If they now slide BACK while walking forward, try 0.5.
   5. If it is worse in every way, set OFF (= R190) and send me a Shift+F2
      log while walking.
   6. Also look down while walking: the chest should no longer stick out
      forward.

Tests
   - New root_lead_r191: 10/10 checks pass.
   - menu_layout_r184: 42/42, now 155 rows.
   - tutorial_native_r159: 84/84.
   - All 109 tests that include physical_weapon.h give the same results as
     before.
   - Settings checker: 0 mismatches.
   - Syntax baselines unchanged (frik_pose_probe 38, camera_hook 119,
     vr_menu 658).
