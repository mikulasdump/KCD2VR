KCD2VR R179 - hits register again
Marker: KCD2VR-R179-HITS-BACK-20261109
Base: R178. Copy the files over the repo (same paths), then run build_release.bat.

1) No damage in R178 - my bug, fixed
   - In R178 I added the wrist speed to the impact record
     (src/vrik/physical_weapon.h, the line that starts the impact record).
     My new code ended with a // comment, and the rest of the original line
     came after it on the same line. So
       r.key / r.generation / r.epoch / r.request / r.sample
     were commented out.
   - Every impact record then had sample=0. Your log shows it:
     impact{valid=1 ... sample=0}, 0 GEOM_IMPACT lines, 0 F4_PHYSICAL_IMPACT
     lines. The R177 log had 145 GEOM_IMPACT lines. The hit reader
     (frik_pose_probe.cpp) drops a record with sample 0, so no hit ever
     reached the game.
   - Fix: the identity is back on its own line. I scanned every line I added
     since R176 for code hidden behind a comment. That was the only one.

2) Block row text
   - "Enemies block your swing" now also says OFF in its description.

Unchanged: the R178 handle pivot and the arm swing power. The swing power
could never work in R178 because no hit got through. It works from this
build on, and GEOM_IMPACT shows arm= and hand/point=.

Tests: tutorial 84/84, training 116/116, weapon_feel 15/15.
All 109 tests that include physical_weapon.h give the same results as before.
frik_pose_probe syntax check: same 38 shim errors as the baseline.
