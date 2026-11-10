#pragma once
// R192 palm look (Miki 10 Nov: "palm look should be working ONLY in immersive or minimal HUD, not in normal HUD - now it
// disables the normal HUD too, like switches it to immersive"). before, the palm glance WAS the Immersive HUD switch
// (VrContextualHud): ticking "palm look" turned a NORMAL HUD into the immersive one (compass / health hidden until you
// look at a palm). now the HUD mode (NORMAL / IMMERSIVE / MINIMAL) and the palm look are two settings:
//   NORMAL     = full HUD, the palm look does nothing.
//   IMMERSIVE  = palm look ON: compass / health hidden, an open palm glance shows them (as before).
//   MINIMAL    = palm look ON: as R141. palm look OFF: compass / health stay hidden (no glance).
// IMMERSIVE with palm look OFF has nothing to hide, it shows the full HUD like NORMAL. VrPalmLookR192, default ON
// (= the behaviour before R192 in IMMERSIVE / MINIMAL). pure.
#include <atomic>
namespace kcd2vr::ui_palm_look_r192 {
inline constexpr const char* kKey="VrPalmLookR192";
inline std::atomic<bool> enabled{true};
constexpr bool Active(bool immersiveHud,bool minimalHud,bool palmLook) noexcept {return palmLook&&(immersiveHud||minimalHud);}
}
