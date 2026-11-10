KCD2VR R188 - every weapon moves in sync with the hand
Marker: KCD2VR-R188-ALL-WEAPONS-IN-SYNC-20261110
Base: R187. Copy the files over the repo (same paths), then run build_release.bat.

Weapon drawn with the hand (WEAPONS page 1 row 1, VrWeaponLeadR180, same setting as before)
   - Miki: "the same thing for BOW or other weapons ... that all weapons do
     not flow in hand, but move in sync".
   - R180 / R182 drew only sword / axe / mace / dagger ahead of their last
     motion, so they stay in the fingers. Now EVERY held item gets it: bow,
     crossbow, gun, shield, torch, knife.
   - Display only. The bow and crossbow aim, the shot direction, contact
     and hits keep using the real (physical) pose, so aiming is unchanged.
   - The bow's placement check (bow_placement_lifetime.inl) compares the
     drawn bow with the pose it was placed at, both are the led pose, so it
     still matches.
   - Not while an impale pins the weapon (unchanged).
   - One switch for all: OFF / 0.5 / 1 / 1.5 / 2 cycles.
   - File: src/vrik/physical_native.inl (the leadOkR180 condition), row text,
     docs.

Note: keep "Same-cycle weapon placement trial" OFF (your INI has it OFF).
It writes the unled pose and would fight this.
Tests: weapon_lead 14/14, menu_layout 36/36, settings checker 0 mismatches,
frik_pose_probe syntax check same as the baseline.
