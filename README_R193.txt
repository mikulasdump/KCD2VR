KCD2VR R193 - home menu with 5 buttons, weapon at stick turns, one-frame hand springs
Marker: KCD2VR-R193-HOME-MENU-20261110
Base: R190. This zip has all R191, R192 and R193 files. Copy them over the repo
(same paths), then run build_release.bat.
KCD2VR.ini is NOT in this zip, keep your own.

1) Home menu (player view)
   Right page, top to bottom:
     HUD
     VR CONFIG
     GRAPHICS
     IMMERSION AND CONTROLS
     COMBAT
     VRIK | NATIVE CONTROLS
     RECALIBRATE VRIK (SHIFT+F8)
   Left page: MONO / STEREO only.
   - HUD opens the VR HUD page. It was called UI SETTINGS.
   - COMBAT is new. For now it has the Enemies page (enemy combat, fast
     block). The Combat step fills it (Normal = your beta values, Hardcore =
     your current values, custom, sensitivity pages).
   - IMMERSION AND CONTROLS has only its own 2 pages now (Body + movement,
     Controls + hands). The R192 HUD page is gone from it: all of those rows
     (HUD mode, palm look, depth/distance, cross, tutorials, block message)
     are already on the HUD menu.
   - Removed from Home:
       FPS LIMIT: it is on GRAPHICS (FPS CAP + target).
       UI / CONTROLLER CONTROLS: it is on VR CONFIG > CONTROLS.
   - Developer mode only now: RESTORE CALIBRATION, Auto restore calibration,
     Best VR settings, SAVE SETTINGS (every control saves itself anyway).
     VRIK EXPERIMENTS, WEAPON / FIST SENSITIVITY and the two experiment
     buttons stay developer-only as before.
   - NATIVE TOUCH is renamed NATIVE CONTROLS.
   Code: src/vr_menu.cpp, src/vr_menu.h, src/release_menu_r92.h.

2) Weapon moves sideways at a joystick turn
   - Your R191 log: at a stick turn Henry's root turns about 2.1 deg per frame.
   - R191 also led the turn: the weapon was put on Henry's next yaw while the
     view stays on the current one. At 40-60 cm from the body axis that is
     1.5-2 cm sideways (medial / lateral).
   - The turn is no longer led. Only the walking step is (that part you said
     is better). src/vrik/root_lead_r191.h.

3) One-frame springs in the hands
   - What the log shows:
       - The VR pose itself has no one-frame jumps: 0 in 10155 frames per
         hand, model space.
       - The camera eye path is smooth.
       - Your game frame times: mostly 10-15 ms, but 4 % are 15-65 ms long.
   - In R191 the eye and the weapon were led by the LAST raw step. A long
     frame has a double step, so the eye sprang ahead for one frame and the
     hands seemed to jump against the view. Hand-to-eye frame-to-frame
     change, p98: 12 mm with the raw lead, 5 mm without.
   - Now the step is smoothed over about 3 frames. A long frame moves the
     lead only 35 % of the difference. Standing still = the lead goes to 0
     in a few frames.
   - The camera (2 calls per frame) and both hands use one smoothed step per
     pose sample, so they always agree.
   - "Not that smooth in every mode": the same springs, on every hand
     smoothing setting. If you still see a spring, Shift+F2 while walking,
     and tell me roughly when (minute) it happened.
   - The setting is the same: VRIK page 2 row 8, VrBodyLeadR191 (OFF = no
     lead).

Tests
   - root_lead_r191: 11/11 (hitch smoothing, same frame twice, standing
     still, no turn lead).
   - release_menu_r192: 45/45 (sections: IMMERSION AND CONTROLS 2 pages,
     COMBAT 1 page, < > stay inside).
   - minimal_hud_r141: 260/262 (the same 2 old INI CRLF checks).
   - menu_layout_r184: 42/42.
   - release_body_height_r98: 7/7.
   - The 109 physical_weapon tests are the same as before.
   - Settings checker: 0 mismatches.
   - Syntax baselines unchanged (38 / 119 / 658).
   - Home click areas are checked at compile time: no overlap, inside the
     page.
