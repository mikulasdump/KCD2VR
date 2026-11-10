#pragma once
// R175 crossbow / gun: the right trigger alone shoots (Miki 9 Nov: "the crossbow (and rifle then too, probably) does not
// work - you need to hold LT+RT to shoot. If we can make only RT to shoot"). the game fires a crossbow / gun only while
// it aims (LT held). with a crossbow or a firearm in hand and the left trigger NOT pressed, a right trigger press now
// sends LT (aim) for the whole press, and RT only after kLeadMs (the game must be in the aim first). a short tap that
// ends before kLeadMs still fires: RT is sent for kTapMs at kLeadMs. after the shot LT stays kTailMs so dropping the aim
// in the same frame cannot cancel it. a press that starts while you hold LT yourself is passed through unchanged (your
// own aim). decided at the start of each press, no latch.
#include <atomic>
#include <cstdint>
namespace kcd2vr::ranged_rt_fire_r175 {
inline constexpr const char* kKey="VrRangedRtFireR175";
inline std::atomic<bool> enabled{true};
constexpr std::uint64_t kLeadMs=250,kTapMs=80,kTailMs=150;
constexpr float kPress=.55f,kRelease=.35f,kOwnLt=.35f;
struct State {bool down=false,own=false,tap=false;std::uint64_t since=0,upAt=0,presses=0;};
struct Out {bool own=false;float lt=0,rt=0;};
inline bool after(std::uint64_t now,std::uint64_t t,std::uint64_t ms) noexcept {return now>=t&&now-t>=ms;}
// on: the switch. ranged: a crossbow / firearm is in Henry's hand (fresh). blocked: menu / dialogue / chooser / activity.
inline Out step(State& s,bool on,bool ranged,float rt,float lt,bool blocked,std::uint64_t now) noexcept {
    Out o{};o.lt=lt;o.rt=rt;
    const bool down=s.down?rt>kRelease:rt>kPress;
    if(!on||!ranged||blocked){s.down=down;s.own=s.tap=false;s.upAt=0;return o;}
    if(down&&!s.down){s.own=lt<kOwnLt;s.tap=false;s.since=now;s.upAt=0;if(s.own)++s.presses;} // a new press
    if(!down&&s.down&&s.own){
        if(!after(now,s.since,kLeadMs))s.tap=true;   // released before the aim was ready: fire at kLeadMs anyway
        else s.upAt=now?now:1;
    }
    s.down=down;
    if(!s.own)return o;
    o.own=true;o.lt=1.f;o.rt=0.f;
    if(down){if(after(now,s.since,kLeadMs))o.rt=rt;return o;}
    if(s.tap){
        if(!after(now,s.since,kLeadMs))return o;                       // aiming
        if(!after(now,s.since,kLeadMs+kTapMs)){o.rt=1.f;return o;}        // the shot
        if(!after(now,s.since,kLeadMs+kTapMs+kTailMs))return o;           // keep the aim
    }else if(s.upAt&&!after(now,s.upAt,kTailMs))return o;
    s.own=s.tap=false;s.upAt=0;o.own=false;o.lt=lt;o.rt=rt;return o;
}
}
