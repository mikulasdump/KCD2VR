KCD2VR R189 - stabs land, bow / torch lead
Marker: KCD2VR-R189-STABS-LAND-20261110
Base: R188. Copy the files over the repo (same paths), then run build_release.bat.

1) Stabs not registering (about 20 %) - not the energy
   - Your R188 log has 13 committed stabs. 3 of them touched the enemy only
     58, 135 and 141 ms AFTER the commit. Every one of those contacts was
     written as kind=UNKNOWN, gesture 0 (GEOM_IMPACT ... queued=0): a contact
     with no attack attached, so it was never sent to the game. 3 of 13 is
     your 20 %. The stabs that did land had full energy (most 1.00).
   - Why: a committed attack gave its identity to a contact only in the exact
     sample where the blade was last seen moving forward. And a stab was
     retired on its first backward frame. The blade meets the body, your
     hand stops or pulls back, and the attack was gone in that same frame.
   - Now:
       a) a contact up to 150 ms after the attack's last forward sample still
          belongs to that attack (src/vrik/stab_contact_r189.h,
          src/vrik/physical_weapon.h, both places that join a contact to
          its attack);
       b) a committed stab is not retired by a backward frame during its
          first 250 ms (src/vrik/committed_stroke_h8.inl). The recognizer's
          own rearm / new candidate / quiet rules still end it.
   - One attack is still one hit: the queue refuses a second contact of the
     same attack (EXACT_CONTACT_ALREADY_QUEUED).
   - Cuts get a) too. 1 of your cuts was lost the same way (255 ms, that one
     stays out, it is past 150 ms).

2) Bow / torch still drift
   - In R188 the lead was skipped while a held item sits in the "display
     rejoin" state (its collision sweep is still catching up). The bow and
     torch are often in it. Now only an impale pin skips the lead
     (src/vrik/physical_native.inl).
   - If the bow / torch still trail your hand after this, press Shift+F2
     while moving it and send me that log. Then I can see which writer
     places them.

Note: your log shows arm=1.00 even at hand/point 0.16. "Swing power: arm,
not wrist" (WEAPONS page 1 row 7) looks OFF in your menu.

Tests: new stab_contact_r189 6/6. All 109 tests that include
physical_weapon.h give the same results as before.
frik_pose_probe syntax check: same 38 shim errors as the baseline.
