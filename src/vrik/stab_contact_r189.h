#pragma once
// R189 stab contact identity (Miki 10 Nov: "some of my stabs were not registering (ca. 20%) - maybe too low of energy?").
// not the energy: in his R188 log 3 of 13 committed stabs touched the enemy only 58..141 ms AFTER the commit, and every one
// of those contacts was recorded as kind=UNKNOWN / gesture 0 (GEOM_IMPACT ... queued=0) - a contact without its attack, so
// it was never sent to the game. the committed gesture kept its identity for a contact only in the exact sample its blade
// was last seen moving forward (forwardTickH8 == this tick), and a stab was retired on the first backward frame - the
// blade meets the body, the hand stops or pulls back, and the attack identity is gone in that same frame.
// now: (1) a contact up to kContactGraceMs after the last forward sample of the committed attack still belongs to it;
// (2) a committed STAB is not retired by a backward frame during its first kThrustKeepMs (the recognizer's own rearm /
// new candidate / quiet rules still end it). one contact per attack stays one hit (EXACT_CONTACT_ALREADY_QUEUED).
#include <cstdint>
namespace kcd2vr::stab_contact_r189 {
constexpr std::uint64_t kContactGraceMs=150;
constexpr std::uint64_t kThrustKeepMs=250;
constexpr bool recentForward(std::uint64_t fwdTick,std::uint64_t fwdSample,std::uint64_t tick,std::uint64_t sample) noexcept {
    return fwdTick&&((fwdTick==tick&&fwdSample==sample)||(tick>=fwdTick&&tick-fwdTick<=kContactGraceMs));
}
// kind 1 = thrust (CommittedGestureH26). true = keep the committed stab through this backward frame
constexpr bool keepThrust(int kind,std::uint64_t commitTick,std::uint64_t tick) noexcept {
    return kind==1&&commitTick&&tick>=commitTick&&tick-commitTick<=kThrustKeepMs;
}
}
