KCD2VR R178 - block OFF, weapon turns in the fist, arm swing power
Marker: KCD2VR-R178-HANDLE-PIVOT-ARM-POWER-20261109
Base: R177. Copy the files over the repo (same paths), then run build_release.bat.

1) Enemies block your swing: new OFF (NPCS page row 7)
   - The row now cycles BY SKILL -> OFTEN -> ALWAYS -> ALWAYS PERFECT -> OFF.
   - OFF is VrDirectNpcBlock=4. The roll never runs, so enemies block only
     when the game itself decides to. BY SKILL still makes those blocks come
     up faster.
   - Before this there was no real off: 0 still rolled while
     VrDirectNpcBlockBySkill was on.
   - Files: npc_force_block153.h (chance() returns 0 for mode 4),
            mod.cpp and direct/host.inl (clamp 0..4),
            live_settings_d39.h (row cycle + label).

2) Weapon turns in the fist (OBJECTS page 3 row 6, VrWeaponHandlePivotR178, default ON)
   - The cause of the slide: the weapon trails the controller only in rotation
     (tip weight), and its grip point is pinned to the WRIST. The wrist sits
     about 3 cm off the handle centre line (the C55 GRIP_CLOSURE log line
     shows localWrist 0.024 / -0.066 / 0.016). So every degree of lag swung
     the handle around the wrist, and it slid out or in along itself, inside
     your fist.
   - Now the lag turns around the point on the handle centre line next to the
     wrist, where the fist closes. The handle stays where your real hand is
     and only the tip trails.
   - Hand and weapon stay one piece. The wrist moves a few mm with the lag
     instead, never more than 3 cm.
   - One hand only. Two hands keep their own constraint. No lag = no change.
   - Files: src/vrik/weapon_feel_r178.h (new, handlePoint),
            src/vrik/physical_weapon.h (anchored grip candidate).

3) Swing power: arm, not wrist (OBJECTS page 3 row 7, VrSwingPowerArmR178, default ON)
   - Before, the hit energy came only from the struck point's speed, so a
     quick wrist flick hit as hard as a full arm swing.
   - Now the impact record also stores the wrist's speed. Energy is scaled by
     the wrist speed divided by the struck point's speed:
       0.30 or more (arm swing, thrust) = full
       0.08 or less (pure flick)        = 45 %
       smooth in between
       unknown                          = unchanged
     The game's RPG, armour and perks still decide the damage after that.
   - The GEOM_IMPACT log line now shows arm= (the factor) and hand/point=
     (the measured ratio). If your normal full swings show arm below 1.00,
     tell me the hand/point numbers and I will move the 0.30 edge.
   - Files: physical_weapon.h (ImpactRecord handSpeedR178 / pointSpeedR178),
            frik_pose_probe.cpp (joules x factor).

   Both new rows replace repeated rows on OBJECTS page 3. "INSTANT
   interactions" is still on OBJECTS page 2 and "Item pickup reach" is still
   on OBJECTS page 1.

Tests:
- New: weapon_feel_r178 15/15.
- tutorial 84/84, npc_force_block 58/58, training 116/116, body_follow 39/39,
  grip depth 11/11, scenery props 30/30.
- All 109 host tests that include physical_weapon.h give the same results
  before and after this patch.
- Settings checker: 0 mismatches.
