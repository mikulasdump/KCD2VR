#pragma once
// R178 weapon feel (Miki 9 Nov: "there must be a better option - for the weapon to not drift around the handle, only with
// the tip through the drag. so slightly rotating yes, but not moving laterally inside the grip... can you also make the
// better strength calculation - so flick of the wrist has lower power then move of the whole hand").
//
// 1) HANDLE PIVOT. the held weapon lags the controller in rotation only (tip weight, R59.16C10R14 / D69 rigid grip), its
//    grip point is pinned to the WRIST (primary.p). the wrist sits about 3 cm off the handle centre line (C55 GRIP_CLOSURE:
//    localWrist (0.024,-0.066,0.016)), so every degree of lag swung the handle around the wrist: it slid out or in along
//    itself, inside your fist. now the lag turns around the point of the handle centre line next to the wrist (where the
//    fist closes): handle stays where your real hand is, only the tip trails. hand and weapon stay one rigid piece (the
//    hand is still drawn from the weapon, closure 0), the wrist moves a few mm with the lag instead.
// 2) SWING POWER. the hit energy came from the struck point's speed only, so a quick wrist flick (tip fast, hand almost
//    still) hit as hard as a whole arm swing. now the hand's own speed at the impact against the struck point's speed
//    scales it: hand / point >= 0.30 (arm swing, thrust) = full, <= 0.08 (pure flick) = 45 % of the energy, smooth between.
//    unknown hand speed = unchanged. the game's RPG / armour / perks still decide the damage after that.
// pure: no native reads or writes.
#include <algorithm>
#include <atomic>
#include <cmath>
namespace kcd2vr::weapon_feel_r178 {
inline constexpr const char* kPivotKey="VrWeaponHandlePivotR178";
inline constexpr const char* kPowerKey="VrSwingPowerArmR178";
inline std::atomic<bool> handlePivot{false},armPower{true}; // R180: handle pivot retired (OFF, no row, the INI key is ignored)

struct V {double x=0,y=0,z=0;};
// weapon-local: grip = the wrist point, tip = the far end of the bounds' centre line (PhysFillProfile). the centre line is
// parallel to the longest bounds axis, which is the dominant component of tip - grip. returns false = no usable handle.
inline bool handlePoint(V grip,V tip,V& out) noexcept {
    const double d[3]{tip.x-grip.x,tip.y-grip.y,tip.z-grip.z};
    if(!(std::isfinite(d[0])&&std::isfinite(d[1])&&std::isfinite(d[2])))return false;
    int k=0;for(int i=1;i<3;++i)if(std::fabs(d[i])>std::fabs(d[k]))k=i;
    if(std::fabs(d[k])<.10)return false;                     // shorter than 10 cm: not a handle
    out=tip;(k==0?out.x:k==1?out.y:out.z)=(k==0?grip.x:k==1?grip.y:grip.z); // same height on the centre line as the wrist
    const double r=std::sqrt((out.x-grip.x)*(out.x-grip.x)+(out.y-grip.y)*(out.y-grip.y)+(out.z-grip.z)*(out.z-grip.z));
    return r>=.003&&r<=.08;                                   // 3 mm .. 8 cm off the line, else the profile is not a fist grip
}
constexpr double kMaxShiftM=.03;                              // never more than 3 cm (a 60 deg lag at 3 cm)

constexpr double kFlickRatio=.08,kArmRatio=.30,kFlickEnergy=.45;
// handSpeed: the wrist's speed, pointSpeed: the struck point's speed, same units and frame. < 0 = unknown.
inline double energyScale(double handSpeed,double pointSpeed) noexcept {
    if(!std::isfinite(handSpeed)||!std::isfinite(pointSpeed)||handSpeed<0||pointSpeed<=1e-6)return 1.;
    const double t=std::clamp((handSpeed/pointSpeed-kFlickRatio)/(kArmRatio-kFlickRatio),0.,1.);
    return kFlickEnergy+(1.-kFlickEnergy)*t;
}
}
