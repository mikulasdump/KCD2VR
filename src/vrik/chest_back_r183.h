#pragma once
// R183 chest back (Miki 9 Nov: "we have it for longer time - the chest moving unnaturally. if I look down ... the chest is
// just sticking out, too much, if we can move it like 3-5 cm back"). the camera is placed from the calibrated head anchor
// (tracked_frame51 eye), the VR body hangs from the solved head target (pose_tracking_goals). the trunk now hangs this
// much further BACK (horizontal, along the calibrated body forward) than the camera: chest, shoulders and belt move back
// under the view, the view and the controller hands do not move. 0 = old. Henry's head mesh is hidden in first person.
// R185 (Miki 9 Nov: "now the shoulders are not moving forward (but without moving the head forward) - the whole body feels
// not mine ... it is not a new thing, only as a bugfix"): the VR trunk never came forward when you reach - the shoulders
// stayed where the head put them, so the arms ran out of length and the hands stopped short of the controllers. now the
// trunk offset follows how far your hands are in front of your head (horizontal, along the body forward):
//   hands near the body (<= 30 cm in front): the R183 chest back (default 4 cm back)
//   reaching (>= 60 cm in front): the trunk comes kReachForwardCm (5 cm) FORWARD - shoulders lead the arms like a real reach
//   smooth in between. the head target moves, the view and the hands never do.
#include <algorithm>
#include <atomic>
#include <cmath>
namespace kcd2vr::chest_back_r183 {
inline constexpr const char* kKey="VrChestBackCmR183";
inline std::atomic<float> cm{4.f};
inline float clampCm(float c) noexcept {return std::isfinite(c)?std::clamp(c,0.f,10.f):4.f;}
constexpr float kReachNearM=.30f,kReachFarM=.60f,kReachForwardCm=5.f;
// reach: the farthest tracked hand in front of the head target (metres, horizontal); < 0 / NaN = no hand = near.
// returns the trunk offset in cm, + = back, - = forward.
inline float trunkCm(float backCm,float reach) noexcept {
    const float b=clampCm(backCm);
    if(!std::isfinite(reach))return b;
    float k=std::clamp((reach-kReachNearM)/(kReachFarM-kReachNearM),0.f,1.f);k=k*k*(3.f-2.f*k); // smoothstep
    return b*(1.f-k)-kReachForwardCm*k;
}
struct V {float x=0,y=0,z=0;};
// forward: horizontal body forward in model space (any length). centimetres: + back, - forward. returns the shift to ADD (m).
inline V shift(float fx,float fy,float centimetres) noexcept {
    const float c=std::isfinite(centimetres)?std::clamp(centimetres,-10.f,10.f):0.f;const float n=std::sqrt(fx*fx+fy*fy);
    if(c==0.f||!std::isfinite(n)||n<1e-4f)return {};
    return {-fx/n*c*.01f,-fy/n*c*.01f,0.f};
}
}
