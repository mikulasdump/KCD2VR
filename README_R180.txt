KCD2VR R180 - weapon drawn with the hand, no neck clipping on turns
Marker: KCD2VR-R180-WEAPON-LEAD-NO-EYE-HOLD-20261109
Base: R179. Copy the files over the repo (same paths), then run build_release.bat.

1) Chest comes forward on turns, neck clips - R177 removed
   - My R177 "eye position hold" caused this. While Henry turned it held his
     eyes back, and it also held the VR head anchor back. Meanwhile his chest
     turned forward with his root, so the head stayed behind the chest and
     the neck stretched or clipped.
   - With free targeting plus, a stick turn also counts as a body hold, so it
     happened on stick turns too.
   - Fix: the three R177 additions are gone. These files are back to R176:
       tracked_camera_service.inl  (camera)
       pose_tracking_goals.inl     (head anchor)
       weapon_tracking.inl         (weapon base head)
   - The body follow 45 deg default stays. You said 45 already looked
     natural.
   - body_follow_r173.h keeps the pure rule only for its test. Nothing calls
     it.

2) Handle trails Henry's fingers - new "Weapon drawn with the hand"
   (OBJECTS page 3 row 6, VrWeaponLeadR180, default 1)
   - The mod computes hand and weapon from the same moment. Your logs show
     grip closure 0 mm, rebaseR98=0 and grace=0, so the mod never fell back
     to an older weapon position. The same-cycle trial did not help either.
     So the game draws the skinned hand one pose cycle newer than the weapon.
   - Now the held sword / axe / mace / dagger is drawn ahead along its own
     motion from the last cycle, position and rotation, by this many cycles:
       OFF / 0.5 / 1 / 1.5 / 2
     Capped at 10 cm / 30 deg. Reset on a gap over 60 ms or a new weapon.
   - Display only. Contact, hits and swing power still use the real weapon
     pose. Not for bows, crossbows, shields or a pinned impale.
   - Tuning: if the handle still trails the fingers, go higher. If it now
     runs ahead of the fingers, go lower.
   - Files: src/vrik/weapon_lead_r180.h (new),
            src/vrik/physical_native.inl (weapon placement).

3) R178 "Weapon turns in the fist" retired
   - It did not help with the slide. It is now off, its row is replaced by
     the new one, and VrWeaponHandlePivotR178 in the INI is ignored.

Note: your last log had no fight in it (0 combat states), so it could not
show whether hits work in R179. Please do one fight with R180.

Tests: new weapon_lead_r180 11/11, weapon_feel 15/15, tutorial 84/84,
training 116/116, body_follow 39/39, grip depth 11/11, scenery props 30/30.
All 109 tests that include physical_weapon.h give the same results as before.
Settings checker: 0 mismatches. Syntax checks: same error counts as the
baseline (frik_pose_probe 38, camera_hook 119).
