#pragma once
// R176 stone throwing (Miki 9 Nov: "can I still throw stones with left grip by crouching? ... yes, add stone throwing").
// the game's own distraction throw: crouched, RB (xi_shoulderr) press = distract_init (the aim), RB release =
// decoy_throw, A = decoy_abort (defaultProfile.xml actionmaps "crouch" / "distract"). no VR button sends RB in plain play.
// now: crouched (the game's "crouch" action map is enabled, read by the view lens), an EMPTY left hand raised to about
// head height (no more than 20 cm below the headset, so the knife pouch at the hip / chest and grabbing low stay as they
// are) + left Grip = RB held; let the Grip go = RB released = the game throws. A still cancels (the game's own).
// decided at the start of each Grip press, ends with the release or when the press is blocked; no latch.
#include <atomic>
#include <cstdint>
namespace kcd2vr::stone_throw_r176 {
inline constexpr const char* kKey="VrStoneThrowR176";
inline std::atomic<bool> enabled{true};
inline std::atomic<std::uint64_t> crouchMapMs{0},distractMapMs{0}; // view lens: last frame each map was read enabled
constexpr std::uint64_t kFreshMs=300;
constexpr float kPress=.55f,kRelease=.35f;
inline bool fresh(std::uint64_t tick,std::uint64_t now) noexcept {return tick&&now>=tick&&now-tick<=kFreshMs;}
struct State {bool down=false,own=false;std::uint64_t throws=0;};
// crouched: the crouch (or distract) map is enabled now. pose: empty left hand raised. blocked: menu / chooser / activity.
inline bool step(State& s,bool on,bool crouched,bool pose,float grip,bool blocked) noexcept {
    const bool down=s.down?grip>kRelease:grip>kPress;
    if(down&&!s.down){s.own=on&&crouched&&pose&&!blocked;if(s.own)++s.throws;}
    s.down=down;
    if(!down||blocked||!on){s.own=false;return false;}
    return s.own;
}
}
