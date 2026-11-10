KCD2VR R185 - shoulders come forward when you reach
Marker: KCD2VR-R185-SHOULDERS-REACH-20261109
Base: R184. Copy the files over the repo (same paths), then run build_release.bat.
No KCD2VR.ini in this zip. Keep your own.

Bug fix: the VR trunk never came forward when you reach
   - Miki: "the shoulders are not moving forward (but without moving the head
     forward) - the whole body feels not mine ... it is not a new thing".
   - Why: Henry's trunk hung from the head target only. When you reach out,
     your real shoulders come forward and your head stays. Henry's shoulders
     stayed where his head put them, so his arms ran out of length and the
     hands stopped short of your controllers. The R183 chest back made it
     4 cm worse.
   - Now the trunk follows how far your hands are in front of your head
     (horizontal, along the body forward, the farther hand counts):
       hands within 30 cm in front:  Chest back as set (default 4 cm back)
       hands 60 cm or more in front: trunk 5 cm FORWARD, the shoulders lead
                                     the arms
       between:                      smooth, never jumps
   - The view and your hands never move. Only the head target the body hangs
     from moves.
   - Works with Chest back OFF too. The reach part is the bug fix and is not
     tied to the option.
   - Files: src/vrik/chest_back_r183.h (trunkCm, signed shift),
            src/subsystems/vrik/pose_tracking_goals.inl (now applied after
            the hand goals, it uses them).

VrikChestHandBias = 1 (answer, nothing changed)
   - It TURNS (yaw) Henry's chest toward your hands when both are in front of
     you and apart: dead zone 10 deg, at most 40 deg, scaled by how clearly
     the hands are out front.
   - File: src/subsystems/vrik/pose_torso_stage.inl.
   - It never moves the chest or the shoulders forward. That was the bug
     above.

Tests: chest_back 13/13 (reach curve, smooth, never jumps back, works with
the option OFF).
frik_pose_probe syntax check: same 38 shim errors as the baseline.
