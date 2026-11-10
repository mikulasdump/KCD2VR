KCD2VR R181 - bellows keep LT, VRIK comes on sooner
Marker: KCD2VR-R181-STATION-LT-FAST-START-20261109
Base: R180. Copy the files over the repo (same paths), then run build_release.bat.

1) Alchemy bellows keep the left trigger
   - R167 turns a left trigger press into LB whenever the game offers its
     "hold to look" or Follow prompt. At a station the game can offer that
     prompt too, so the bellows press went to the look.
   - Now R167 never takes LT while you are at a station or activity
     (alchemy, smithing and the rest).
   - File: src/mod.cpp, the R167 block.
   - The prompt still shows "LB". That is the game's own button picture and
     the mod does not change it. Outside stations LT still does the look.

2) VRIK comes on sooner after a load
   - The world must now be up 0.5 s before VRIK switches on (was 1.5 s).
   - File: src/vrik/start_r111.h (kSettleMs).
   - Your last log: load done 21:31:33.8, VRIK on 21:31:35.4 (by itself,
     first try). From this build VRIK should come on about 1 s sooner.
   - The camera recenter came 10 s later (21:31:45.2). It waits until your
     headset sits steady at your runtime center. I did not change that wait,
     see below.

Tests: start_vrik_r111 PASS, R167 scope checked.
