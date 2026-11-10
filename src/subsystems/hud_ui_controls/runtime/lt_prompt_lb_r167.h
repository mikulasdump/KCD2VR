#pragma once
#include <atomic>
#include <cstdint>
// R167 (Miki 9 Nov: "what if we switch it to LT? the same for follow? maybe X is blocked because of the quicktalk"):
// the hold to look / watch prompt (Rosa at the window) and the Follow prompt are LB on the pad, and no VR button sends
// LB. R153 / R156 hold X through for the look prompt, R147 turns an X tap into LB for Follow - but X is also Quick Speak
// (chat_init / chat_init_with_focus on xi_x in the companion profile) and 0.5 s X = torch, and in all his R163-R166 logs
// the X route never ran once (no "R153 LB_PROMPT_X" row, torch rows instead at 00:36:06-00:36:21).
// so a second button with nothing else on it in that moment: the LEFT TRIGGER. a left trigger press that starts while the
// game enables the look prompt map (infinite_hold_interaction_init, read by the view lens, R156) or the Follow prompt map
// (focus_follow_init_interactor, same reader) is sent to the game as LB for that whole press (held = the look keeps going,
// release = it ends), and that press sends no LT. decided once at the start of the press, ends with the release: no latch.
// not while the mod holds LT itself (VR block / special), not in the VR menu, a system menu or the quick talk chooser.
// the X routes stay as they are.
namespace kcd2vr::lt_prompt_lb_r167 {
inline std::atomic<std::uint64_t> followMapEnabledMs{0}; // view lens: last frame the Follow prompt map was read enabled
constexpr std::uint64_t kFreshMs=300;   // same freshness as the R156 look map tick
constexpr float kPress=.55f,kRelease=.35f;
inline bool fresh(std::uint64_t tick,std::uint64_t now) noexcept {return tick&&now>=tick&&now-tick<=kFreshMs;}
struct Route {bool down=false,pass=false;};
// one step per pad frame. returns true = this frame the left trigger is sent as LB (and LT = 0).
inline bool step(Route& r,float trigger,bool promptNow,bool blocked) noexcept {
    const bool down=r.down?trigger>kRelease:trigger>kPress;
    if(!down){r={};return false;}
    if(!r.down){r.down=true;r.pass=promptNow&&!blocked;}
    return r.pass;
}
}
