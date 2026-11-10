KCD2VR R182 - weapon lead smooth again
Marker: KCD2VR-R182-SMOOTH-WEAPON-LEAD-20261109
Base: R181. Copy the files over the repo (same paths), then run build_release.bat.
This zip has NO KCD2VR.ini. Keep your own INI in the game folder.

Weapon drawn with the hand (VrWeaponLeadR180, OBJECTS page 3 row 6)
   - Miki: "in the hand by 1.0, but not smooth movement like before".
   - Why it was rough: R180 pushed the weapon ahead by the raw motion of the
     last pose cycle. Cycles are not all the same length (about 9..14 ms) and
     one hand sample is a little noisy, so the push jumped from frame to
     frame.
   - Now: the weapon's speed (move and turn, per second) is measured over the
     real tracking time of each sample and smoothed (30 ms, about 3 cycles).
     The cycle length is smoothed the same way.
     lead = smoothed speed x smoothed cycle length x your setting
   - Steady motion gets the same lead as R180 (1.0 = one cycle).
   - A single noisy step no longer kicks the weapon.
   - A weapon held still has no lead.
   - Caps and resets are unchanged: 10 cm / 30 deg, a gap over 60 ms or a
     new weapon resets.
   - Files: src/vrik/weapon_lead_r180.h,
            src/vrik/physical_native.inl (passes the sample's tracking time).

Tests: weapon_lead 14/14 (new: noisy step smoothed, still = no lead).
frik_pose_probe syntax check: same 38 shim errors as the baseline.
