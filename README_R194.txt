KCD2VR R194 - combat menu: enemies, custom enemies, sensitivity
Marker: KCD2VR-R194-COMBAT-MENU-20261110
Base: R190. This zip has all R191, R192, R193 and R194 files. Copy them over
the repo (same paths), then run build_release.bat.
KCD2VR.ini is NOT in this zip, keep your own.

Home > COMBAT now has 3 tabs at the top: ENEMIES | CUSTOM | SENSITIVITY >

1) ENEMIES
   - Enemy difficulty: NORMAL -> HARDCORE -> CUSTOM (click cycles)
       NORMAL   = the beta values (your zip is R139): difficulty 43 %,
                  defence x1.75, guard timing x1.0, block raise x1.40,
                  block speed BY SKILL x2.0 (skill 0) .. x3.0 (skill 30),
                  stagger rule 1, block rate BY SKILL
       HARDCORE = your R190 values: difficulty 32 %, defence x1.70,
                  guard timing x1.45, block raise x1.55, by skill OFF,
                  stagger rule 2, block rate BY SKILL
       CUSTOM   = keeps the values you have, change them on the CUSTOM tab
   - Fast block response (as before)
   - The old "Enemy combat" row is gone. Its NORMAL was 0 % / x1.3 and its
     HARD was 50 % / x1.7, both wrong. Your game INI shows 0 % / x1.3 right
     now, so you clicked it once. That shows as CUSTOM: pick NORMAL or
     HARDCORE once.
   - New key VrEnemyPresetR194 (0 / 1 / 2). It is written only after you
     choose. Missing = found from the values. Experiments row: COMBAT,
     "Free targeting +" page, row 2.
   - The values keep their own keys (VrNpcDifficulty, VrNpcDefenseSpeed,
     VrNpcBlockSpeed, VrNpcBlockAnimSpeed, VrDirectNpcBlockBySkill,
     VrDirectNpcBlockSpeedLow / High, VrDirectStaggerRule, VrDirectNpcBlock).
     The developer MISC pages still change them. Then the label shows CUSTOM
     (never a wrong NORMAL / HARDCORE).

2) CUSTOM (custom enemies)
   - Block rate: BY SKILL (40-75 %) -> OFTEN (50-85 %) -> ALWAYS ->
     ALWAYS PERFECT -> OFF (game decides)          (VrDirectNpcBlock)
   - Difficulty slider 0 .. 100 %                  (VrNpcDifficulty)
   - Block speed: BY SKILL / SAME FOR ALL           (VrDirectNpcBlockBySkill)
   - Block speed, low skill enemy (skill 0)  x1.0 .. x3.0 (..SpeedLow)
   - Block speed, high skill enemy (skill 30) x1.0 .. x3.0 (..SpeedHigh)
   - Block speed, all enemies x1.0 .. x3.0          (VrNpcDefenseSpeed)
   - Moving the low or high slider turns BY SKILL on. Moving the all-enemies
     slider turns it off, so a slider is never without effect.
   - Any change here selects CUSTOM on ENEMIES. Choosing NORMAL or HARDCORE
     later overwrites these values.

3) SENSITIVITY >
   - Opens the same page as the developer left-side button "WEAPON / FIST
     SENSITIVITY": weapon type, intent and speed sensitivity, thrust axis,
     tip travel. Its STABS, FISTS, BLOCK and SHIELD BOX pages work too.
   - These pages are now open without developer mode.
   - BACK returns to COMBAT when you came from there (from the developer
     Home button it still goes Home).

Code
   src/enemy_preset_r194.h (new), src/release_menu_r92.h, src/vr_menu.cpp,
   src/vr_menu.h, live_settings_d39.h, live_runtime_d40.inl.
   The R191-R193 files are unchanged since R193.

Tests
   - New enemy_preset_r194: 11/11 (exact NORMAL / HARDCORE values, CUSTOM
     keeps, your INI shows CUSTOM, a developer change shows CUSTOM).
   - release_menu_r192: 54/54.
   - menu_layout_r184: 44/44 (157 rows).
   - minimal_hud_r141: 260/262 (the same 2 old INI CRLF checks).
   - root_lead_r191, release_body_height_r98, tutorial_native_r159: pass.
   - The 109 physical_weapon tests are the same as before.
   - Settings checker: 0 mismatches.
   - Syntax baselines unchanged (38 / 119 / 658).
   - The COMBAT tab and CUSTOM slider layout is checked at compile time: no
     overlap, labels clear.
