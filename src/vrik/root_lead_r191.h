#pragma once
// R191 body lead (Miki 10 Nov: "by forward movement the hands move forward, by backward movement they move backward a
// little - the weapons stay on the controller ... they don't stay the same position to the HMD"). his Shift+F2 log
// (D68 ATTACHMENT, R189): between the pose and the weapon placement Henry's root moves exactly one cycle of walking
// (3.6 cm at 3.3 m/s, 0.7 cm at 0.6 m/s), and the next pose starts from that placement root (producer = last current).
// the camera and the held weapon use that placement root, the skinned body is drawn one root step further along it:
// hands (and the chest) ahead of the eye while walking forward, behind while walking back, and turned on with a joystick
// turn. the camera eye and the drawn weapon are now placed on the root predicted `cycles` steps ahead (last step x
// cycles: position and yaw), so eye, weapon and body share one root. VrBodyLeadR191: -1 .. 2 (0 = off, default 1).
// standing still = no step = no change. a step over 25 cm or 20 deg (teleport, mount, cut scene) = no lead.
// display only: hits, contact and aim are untouched. pure: no native reads or writes.
#include "pose_math.h"
#include <algorithm>
#include <atomic>
#include <cmath>
#include <cstdint>
#include <mutex>
namespace kcd2vr::root_lead_r191 {
namespace pc=kcd2vr::pose_core;
inline constexpr const char* kKey="VrBodyLeadR191";
inline std::atomic<float> cycles{1.f};
inline float clampCycles(float c) noexcept {return std::isfinite(c)?std::clamp(c,-1.f,2.f):1.f;}
constexpr double kMaxStepM=.25,kMaxStepRad=.3491;
// R193 (Miki 10 Nov, R191 test: "weapons are better by forward movement, by rotation [joystick] the weapons are moving
// medially or laterally ... and there is some jitter in the hands, like 1 frame here and there, springing to another
// position"). two changes:
//  1) position only. the turn part of the lead put the weapon on Henry's next yaw, the view stays on the current one:
//     at a stick turn (2.1 deg per frame in his log) the weapon swung 1-2 cm sideways. the turn is no longer led.
//  2) the step is smoothed over about three frames (kSmooth per new frame). his frame times are 10-15 ms with 4 % at
//     15-65 ms; the raw last step doubled on such a frame and the eye / weapon sprang ahead for one frame (his R191
//     log: hand-to-eye frame-to-frame change p98 12 mm with the raw lead, 5 mm without). a new frame = a new producer
//     pose sample (key); the camera (called twice per frame) and both hands read the same smoothed step.
constexpr double kSmooth=.35;
struct Lead {bool on=false;pc::Vec3 root{},step{};pc::Quat turn{};};
struct State {std::mutex m;std::uint64_t key=0;pc::Vec3 smooth{};bool have=false;};
// producer: the root the pose was made with, current: the root now. both world, unit quaternions.
// key: the pose sample of this producer root (a new key = a new frame).
inline Lead make(State& st,std::uint64_t key,pc::Quat qProd,pc::Vec3 tProd,pc::Quat qCur,pc::Vec3 tCur,float c) noexcept {
    Lead l{};c=clampCycles(c);
    std::lock_guard<std::mutex> lock(st.m);
    if(c==0.f||!pc::finite(tProd)||!pc::finite(tCur)){st.have=false;return l;}
    const pc::Vec3 d=tCur-tProd;
    const auto dq=pc::unit(pc::multiply(qCur,pc::conjugate(qProd)));
    if(!dq){st.have=false;return l;}
    const pc::Vec3 rv=pc::rotation_vector(*dq);
    if(!pc::finite(rv)||pc::length(d)>kMaxStepM||pc::length(rv)>kMaxStepRad){st.have=false;return l;}
    if(!st.have){st.smooth=d;st.key=key;st.have=true;}
    else if(key!=st.key){st.smooth=st.smooth+(d-st.smooth)*kSmooth;st.key=key;} // new frame
    l.on=true;l.root=tCur;l.step=st.smooth*double(c);
    return l;
}
inline pc::Vec3 point(const Lead& l,pc::Vec3 p) noexcept {return l.on?p+l.step:p;}
inline pc::Quat orient(const Lead&,pc::Quat q) noexcept {return q;} // R193: no turn lead
}
