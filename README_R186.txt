KCD2VR R186 - FREE PLUS turn limit (no crossed arms after an attack from behind)
Marker: KCD2VR-R186-FREE-PLUS-TURN-LIMIT-20261110
Base: R185. Copy the files over the repo (same paths), then run build_release.bat.
No KCD2VR.ini in this zip. Keep your own. The new key works without it
(default 90 deg).

FREE PLUS: turn limit (COMBAT page 2 "Free targeting +", VrFreePlusTurnLimitR186, default 90 deg)
   - Miki: "if I am attacked from behind in free mode + the body turns
     around, when I turn there my arms are crossed ... we need some
     restraints".
   - Why: FREE PLUS took EVERY game turn out of the view. The game turned
     Henry 180 deg toward the attacker behind you, your view stayed, and so
     Henry faced away from you. Your hands are in front of YOU, which was
     behind HIM, so his arms reached back and crossed. When you turned
     around, he was already turned, and it got worse.
   - Now the game's turn is taken out of the view only while Henry stays
     within the limit of where your head looks (head yaw + view offset). The
     part past the limit is shown: your view turns with Henry, like the
     native game. Henry never faces more than the limit away from you.
       - Turn your head toward him: the hold works again.
       - A game turn back toward you: always held.
       - Your own stick turns in a fight (R153): never limited.
   - Values: OFF (old) / 60 / 75 / 90 / 110 / 135 deg. Live.
   - Files: src/subsystems/experimental/free_targeting_plus_r133.h
              (limitedAbsorb, Input headYaw),
            src/camera_hook.cpp (passes the HMD yaw into the hold),
            settings files. COMBAT now has 5 pages, page 2 is
            "Free targeting +".

Tests: free_targeting_plus_r133 265 checks, 0 failures. New cases:
- an attack from behind holds only 90 deg
- turning your head toward him lets the hold work again
- a turn back toward you is held in full
- own stick turns pass the limit
- OFF absorbs everything as before
- the edge math
menu_layout 36/36, tutorial 84/84, training 117/117.
Settings checker: 0 mismatches.
Syntax checks: same error counts as the baseline (camera_hook 119,
vr_menu 658, frik_pose_probe 38).
