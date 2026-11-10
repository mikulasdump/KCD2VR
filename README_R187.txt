KCD2VR R187 - "look there" back on X
Marker: KCD2VR-R187-LOOK-BACK-ON-X-20261110
Base: R186. Copy the files over the repo (same paths), then run build_release.bat.

Back to how it was before R167
   - Miki: "look there should be back to X ... we have lost a lot from
     taking away the LT".
   - Removed the R167 left trigger -> LB route from src/mod.cpp (the block
     before the R175 crossbow block, and its include). The left trigger is
     always the left trigger again: block, bellows, aim, everything the game
     puts on LT.
   - The X routes were never changed by R167 or later (checked with git:
     no change since R167 in lb_prompt_x_r153.h, native_vr_pad_premap and
     native_input_look_lab):
       hold X = look / watch prompt (R153 / R156)
       X tap  = Follow (R147, the "X tap = Follow" row on GEAR page 2)
   - lt_prompt_lb_r167.h stays in the repo (the view lens still reads the
     Follow map with it). Nothing sends LB from LT any more.
   - Settings: none. R167 had no INI key.
