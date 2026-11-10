#pragma once
// R180 weapon placement lead (Miki 9 Nov: "the handle slides against Henry's fingers with delay, as I move the weapon -
// so the hand and the weapon are not on the same frame"). the mod computes hand and weapon from ONE pose sample (log:
// grip closure 0 mm, rebaseR98=0, grace=0), and the same-cycle placement trial (weapon written at pose publication) did
// not help, so the lag is in the game's drawing: the skinned hand shows a newer pose cycle than the held weapon's world
// transform. the weapon (display only, never collision or hits) is now placed ahead along its own motion of the last
// cycle by `cycles` (VrWeaponLeadR180: 0 = off, 0.5 .. 2): pose + cycles x (pose - previous pose), position and rotation.
// capped (10 cm, 30 deg), reset on a gap > 60 ms, a new weapon or a still weapon. pure: no native reads or writes.
#include "pose_math.h"
#include <algorithm>
#include <atomic>
#include <cmath>
#include <cstdint>
namespace kcd2vr::weapon_lead_r180 {
namespace pc=kcd2vr::pose_core;
inline constexpr const char* kKey="VrWeaponLeadR180";
inline std::atomic<float> cycles{1.f};
inline float clampCycles(float c) noexcept {return std::isfinite(c)?std::clamp(c,0.f,2.f):1.f;}
constexpr double kMaxLeadM=.10,kMaxLeadRad=.5236;
constexpr std::uint64_t kMaxGapMs=60;
// R182 (Miki 9 Nov: "the weapon was in the hand by 1.0, but not smooth movement like before"): R180 led by the raw motion
// of ONE cycle. cycles are not equally long and the hand sample is noisy, so that step jumped from frame to frame. now
// the weapon's speed (linear + angular, per second, from the tracking time of each sample) is smoothed (kSmoothS) and the
// lead is that speed x the smoothed cycle length x cycles. a held still weapon has no speed = no lead.
constexpr double kSmoothS=.030;           // velocity smoothing time constant (30 ms, about 3 cycles)
struct State {pc::Quat q{};pc::Vec3 p{};std::uint64_t sample=0,key=0,tick=0,ns=0;bool have=false;
              pc::Vec3 v{},w{};double cycleS=0;}; // smoothed linear (/s), angular (rad/s) speed and cycle length (s)
// q,p: this sample's weapon pose (model space). timeNs: the sample's tracking time. returns the led pose in q,p.
inline void lead(State& s,float c,std::uint64_t key,std::uint64_t sample,std::uint64_t timeNs,std::uint64_t tick,pc::Quat& q,pc::Vec3& p) noexcept {
    c=clampCycles(c);
    const bool fin=std::isfinite(q.x)&&std::isfinite(q.y)&&std::isfinite(q.z)&&std::isfinite(q.w)&&
                   std::isfinite(p.x)&&std::isfinite(p.y)&&std::isfinite(p.z);
    if(!fin||!key||!sample){s=State{};return;}
    if(!s.have||s.key!=key||tick<s.tick||tick-s.tick>kMaxGapMs){s=State{};s.q=q;s.p=p;s.sample=sample;s.key=key;s.tick=tick;s.ns=timeNs;s.have=true;return;}
    if(sample!=s.sample){ // a new cycle: its speed since the last one
        double dt=(timeNs>s.ns&&s.ns)?double(timeNs-s.ns)*1e-9:double(tick>s.tick?tick-s.tick:0)*1e-3;
        if(dt>1e-4&&dt<=kMaxGapMs*1e-3){
            const pc::Vec3 v=(p-s.p)*(1./dt),w=pc::rotation_vector(pc::multiply(q,pc::conjugate(s.q)))*(1./dt);
            const double k=1.-std::exp(-dt/kSmoothS);
            if(s.cycleS<=0){s.v=v;s.w=w;s.cycleS=dt;}
            else{s.v=s.v+(v-s.v)*k;s.w=s.w+(w-s.w)*k;s.cycleS+=(dt-s.cycleS)*k;}
        }
        s.q=q;s.p=p;s.sample=sample;s.tick=tick;s.ns=timeNs;
    }
    if(c<=0.f||s.cycleS<=0)return;
    const double t=double(c)*s.cycleS;
    pc::Vec3 dp=s.v*t,dr=s.w*t;
    const double lp=std::sqrt(dp.x*dp.x+dp.y*dp.y+dp.z*dp.z),lr=std::sqrt(dr.x*dr.x+dr.y*dr.y+dr.z*dr.z);
    if(!(std::isfinite(lp)&&std::isfinite(lr)))return;
    if(lp>kMaxLeadM)dp=dp*(kMaxLeadM/lp);
    if(lr>kMaxLeadRad)dr=dr*(kMaxLeadRad/lr);
    p=p+dp;q=pc::multiply(pc::from_rotation_vector(dr),q);
}
}
