KCD2VR R177 - block row, eye hold, 45 deg body follow
Marker: KCD2VR-R177-BLOCK-ROW-EYE-HOLD-20261109
Base: R176. Copy the files over the repo (same paths), then run build_release.bat.

1) Body follows HMD (VRIK+ page 2)
   - Free look default is now 45 deg (was 30). This changes the row default,
     KCD2VR.ini VrBodyFollowDegR173=45 and the fallback in the loader.
   - New: eye position hold. Henry turns around his root, and his eyes sit about
     12 cm in front of it, so every turn under you used to swing the view sideways
     by a few cm. That is the "view shifts, not natural" you saw at 30 deg.
     While a body hold is active (R133 FREE PLUS or R173 turning), that swing is
     now taken back. The view, head, hands and weapon all stay where they were,
     then glide back into Henry's real eyes (0.9 s time constant, no jump).
     Horizontal only. A jump larger than 40 cm or 20 deg is never held
     (teleport, mount).
     Files: src/vrik/body_follow_r173.h (OrbitHold / orbitStep),
            src/subsystems/vrik/tracked_camera_service.inl (camera),
            src/subsystems/vrik/pose_tracking_goals.inl (head anchor),
            src/vrik/weapon_tracking.inl (weapon base head).

2) Enemies block your swing (Experiments NPCS page row 7, replaces Clinch status)
   - Cycles through four modes:
     BY SKILL (40-75 %) / OFTEN (50-85 %) / ALWAYS / ALWAYS PERFECT.
   - Writes VrDirectNpcBlock.
   - New chances (src/subsystems/direct/npc_force_block153.h):
       mode 0 BY SKILL: skill 0 = 40 %, rising to 75 % at skill 30+
       mode 1 OFTEN:    skill 0 = 50 %, rising to 85 % at skill 30+
       (was 20 -> 60 % and a flat 65 %)

3) BLOCK page: new slider "Block ON: hold the guard this long first"
   - Writes VrGuardEnterDwellMs, 0..600 ms in 10 ms steps.
   - It sits between Entry and Release. Lower = your block comes in sooner.
     The existing sliders on that page are unchanged: width, height and depth
     (where the block is), entry (weapon sensitivity), release, settle and
     distance.
     Files: src/vr_menu.cpp, src/vr_menu.h.

Tests: body_follow 39/39, npc_force_block 56/56, training 116/116,
tutorial 81/81, bow 88/88, stones 17/17, ranged 23/23.
Settings checker: 0 mismatches.
