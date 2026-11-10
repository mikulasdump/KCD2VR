KCD2VR R190 - your current setup becomes the default
Marker: KCD2VR-R190-HARDCORE-DEFAULTS-20261110
Base: R189. Copy the files over the repo (same paths), then run build_release.bat.

This zip contains the repo's starter KCD2VR.ini. That is the INI a NEW
install gets from the release package. Do NOT copy it into your game folder,
your own INI already has all these values. (Read from your game folder,
C:\Steam\...\Win64MasterMasterSteamPGO\KCD2VR.ini, 10 Nov 11:04.)

Starter INI values changed to yours:
   Enemies (hardcore):
      VrNpcDifficulty 43 -> 32, VrNpcDefenseSpeed 1.75 -> 1.7
      VrNpcBlockSpeed 1 -> 1.45, VrNpcBlockAnimSpeed 1.4 -> 1.55
      VrDirectNpcBlockBySkill true -> false, VrDirectStaggerRule 1 -> 2
      VrDirectCounterStabStrength 1.5 -> 1.3, VrComboRecoveryHoldMs 1000 -> 1500
   Block:
      VrGuardEnterMaxSpeed 2.5 -> 3.90719, VrGuardExitSpeed 3.5 -> 4.2
      VrGuardAfterAttackMs 400 -> 150, VrGuardMaxBodyDistanceD56 0.57 -> 0.56
      VrGuardPlaneHalfWidthH41 0.4356 -> 0.3877, VrGuardPlaneHalfHeightH41 0.1778 -> 0.1576
      VrGuardBoxDepthD61 0.5 -> 0.39, VrFistBlockSensitivityH29 1.25 -> 1.35
      VrPunchSensitivityC54 1 -> 1.05
   VRIK:
      VrikChestNaturalTwist 1 -> 0, VrBodyFollowHmdR173 false -> true
      VrBodyFollowDegR173 45 -> 30, VrHeadRaiseMaxCm 18 -> 12
      VrRoomscaleLeanHeightD70 0.12 -> 0.1, VrElbowOutwardD67 0.2 -> 0.35
      VrSupportReleaseD67 false -> true, VrArcherStanceH41Trial false -> true
      VrChestBackCmR183 4 -> 10, VrFreeTargetingModeR133 1 -> 2 (FREE PLUS)
      VrMeleeGripDepthR168 0 -> 0.5, VrHandLeadMsD69 11 -> 33
docs/SETTINGS.md: the "shipped" values updated to match.
Settings checker: 0 mismatches.
