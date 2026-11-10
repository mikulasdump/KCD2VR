KCD2VR R192 - immersion and controls menu, palm look only in immersive / minimal HUD
Marker: KCD2VR-R192-IMMERSION-MENU-20261110
Base: R190. This zip also has all R191 files (hands stay with the view when
walking), so it is fine if you did not copy R191 yet. Copy the files over the
repo (same paths), then run build_release.bat.
KCD2VR.ini is NOT in this zip, keep your own. The new key VrPalmLookR192 is
added with its default (ON) the first time the menu saves.

This is step 1 of the menu rebuild: IMMERSION AND CONTROLS. Next steps:
Combat, then HUD, VR config, Graphics. Your answer "normal is normal" (the
beta zip you sent is R139) goes into the Combat step: Normal = the beta
values (difficulty 43, defense 1.75, block anim 1.4, block speed 1, block by
skill ON), Hardcore = your current R190 values.

1) Palm look only works in IMMERSIVE or MINIMAL HUD
   - Before: the palm look WAS the "Immersive HUD" switch (VrContextualHud).
     Ticking palm look in a NORMAL HUD turned it into IMMERSIVE, so compass
     and health were hidden until you looked at your palm.
   - Now HUD mode and palm look are two settings:
       NORMAL    = full HUD, palm look does nothing
       IMMERSIVE = palm look ON: compass / health hidden, open palm shows them
                   palm look OFF: full HUD (there is nothing left to hide)
       MINIMAL   = palm look ON: as before (R141)
                   palm look OFF: compass / health are never shown
   - New key VrPalmLookR192, default ON, so IMMERSIVE / MINIMAL work as before.
   - Where:
       VR HUD page (player): new tick "Palm look" (the empty 12th slot)
       IMMERSION AND CONTROLS page 3 "HUD": "Palm look"
       Experiments VIEW page 2 row 6 "Palm look"
     The developer VR HUD page tick is now called "Immersive HUD" (before:
     "Immersive HUD (palm look)"), and so is COMBAT EXPERIMENTS "PALM HUD".
   - Code: src/ui_palm_look_r192.h (new), src/mod.cpp (palm glance enable).

2) IMMERSION AND CONTROLS (Home > IMMERSION AND CONTROLS), now 4 pages with a
   title. < PAGE and PAGE > wrap around. Every row uses the same saved key as
   the developer pages, so nothing is doubled and your values stay.
   Page 1 "Body + movement":
     - HMD based body rotation: OFF / 30 DEG / 45 DEG. Look further aside than
       this and Henry turns after your head. OFF = only the stick turns him.
       (VrBodyFollowHmdR173 + VrBodyFollowDegR173. Your INI: ON, 30.)
     - Free targeting: FREE / FREE PLUS. FREE = the game turns you to the
       enemy. FREE PLUS = it never turns your view. (VrFreeTargetingModeR133.
       GAME LOCK is still on the developer page; one click here = FREE PLUS.)
     - Hand smoothing: UNCAPPED / CAPPED. UNCAPPED when your game fps is
       above the headset refresh, CAPPED when it is below.
       (VrMotionTrialD67 4 / 2, the same as VRIK EXPERIMENTS.)
     - Body height, Joystick turning, Physical walking and dodges,
       Crouching, Comfort vignette (moved here from the old pages)
   Page 2 "Controls + hands":
     - Controller scheme: OPENXR / STEAMVR / PS VR2. It also sets the button
       icons: PS VR2 = PlayStation, the others = Xbox. PS VR2 also turns on
       face touch as the modifier (same as the Experiments row). The old
       "Button icons" row is gone (folded into this).
       (VrControllerLayoutR118 + VrPadGlyphStyle)
     - Activity gamepad, Pointing Quick Talk, Alchemy hands, Smithing hands,
       Dialogue view, Physical impalements, Gore
   Page 3 "HUD": HUD mode (NORMAL -> IMMERSIVE -> MINIMAL), Palm look, HUD
     depth, Combat direction cross, Tutorial messages, BLOCK message. The old
     "Immersive HUD" and "Minimal HUD" rows are now the one HUD mode row.
     This page moves to the HUD tab later.
   Page 4 "Enemies": Enemy combat, Fast block response. Moves to the Combat
     tab in the next step.
   Code: src/release_menu_r92.h, src/vr_menu.cpp (page title).

Tests
   - New release_menu_r192: 47/47 (every row, every value, each row once,
     24 rows on 4 pages).
   - minimal_hud_r141: 260/262. The 2 failures are the same INI CRLF checks
     that also fail on R191.
   - menu_layout_r184: 42/42 (156 rows).
   - release_body_height_r98: 7/7 (did not compile since R124, fixed).
   - root_lead_r191: OK.
   - All 109 tests that include physical_weapon.h give the same results as
     before.
   - Settings checker: 0 mismatches.
   - Syntax baselines unchanged (frik_pose_probe 38, camera_hook 119,
     vr_menu 658).
   - release_menu_r92 needs windows.h. I updated its row numbers, but it
     cannot run here.
