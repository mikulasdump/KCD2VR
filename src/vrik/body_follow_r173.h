#pragma once
// R173 body follows the headset (Miki 9 Nov, and Pico 4 players: "if I turn around 180 degrees, I have to push the
// joystick backwards to move forward"). the view turns with the head as before. when the head is more than the free
// look angle (default 30 deg) away from where Henry faces, Henry turns after it with the game's own right stick turn,
// smooth, until he is back at the edge of the free look. the same yaw goes out of the view and out of the VR body
// frame (the R133 FREE PLUS hold, proven in fights), so the world and your hands do not turn - only Henry under you.
// stick forward then stays within the free look angle of where you look.
//
// never in a locked fight (the game turns Henry to the opponent, R133 FREE PLUS holds that), never while R133 still
// bleeds an offset back, never while you turn with the stick, only in plain tracked gameplay (no menu, dialogue,
// cutscene, scripted scene, native body, activity / dialogue eye, horse). recenter / load / pose epoch = reset to 0.
// pure: no native reads, no writes. the camera thread runs step(), the pad thread sends rx, the body frame reads applied.
#include <algorithm>
#include <atomic>
#include <cmath>
#include <cstdint>
namespace kcd2vr::body_follow_r173 {
inline constexpr const char* kKey="VrBodyFollowHmdR173";
inline constexpr const char* kDegKey="VrBodyFollowDegR173";
inline std::atomic<bool> enabled{false};            // not headset proven yet: OFF until Miki says it works
inline std::atomic<float> freeLookDeg{45.f}; // R177: 45 (30 swung the view too often)
inline float clampDeg(float d) noexcept {return std::isfinite(d)?std::clamp(d,10.f,90.f):45.f;}

constexpr double kPi=3.141592653589793;
constexpr double kJumpRad=.35;          // > 20 deg camera change in one call = teleport / scripted, never absorbed
constexpr double kStopRad=.0087;        // 0.5 deg past the edge: done
constexpr double kGain=3.4;             // rx per rad past the edge (10 deg -> 0.59)
constexpr float kMinRx=.12f,kMaxRx=1.f;
constexpr std::uint64_t kDriveMs=150;   // a camera turn this soon after our rx is ours and is absorbed
constexpr double kSignCheckRad=.0524;   // 3 deg of absorbed turn decide whether the stick sign is right
constexpr std::uint64_t kMaxFlips=6;
inline double wrap(double a) noexcept {return std::isfinite(a)?std::remainder(a,2*kPi):0;}

struct State {
    double f=0;                 // the yaw taken out of the view and the body frame (rad, Cry yaw, + = left)
    double lastCamera=0;bool haveCamera=false;
    double fb=0,lastRoot=0;bool haveRoot=false; // the same for the VR body frame, from Henry's root yaw (R133 keeps both)
    std::uint64_t driveMs=0;    // last tick we asked for a turn
    int sign=1,lastDir=0;double signAcc=0; // lastDir: +1 we asked for a left turn, -1 right
    double turnedDeg=0;std::uint64_t driveFrames=0,flips=0,resets=0;
};
struct Input {
    bool on=false,reset=false;
    bool context=false;         // plain tracked gameplay (worker + camera checks)
    bool applyView=false;       // the offset may be shown now (not dialogue / activity eye / horse / native camera)
    bool pause=false;           // locked fight, R133 offset or bleed, R133 own turn
    bool userTurn=false;        // the right stick is turning right now
    bool headValid=false;
    double cameraYaw=0;         // native camera yaw = where Henry faces (Cry yaw)
    bool haveRoot=false;double rootYaw=0; // Henry's root yaw (fresh), the body frame reference
    double headYaw=0;           // recentered local HMD yaw (camera_hook hmd_local_yaw, Cry yaw)
    double freeLookRad=45*kPi/180;
    std::uint64_t now=0;
};
struct Output {float rx=0;double applied=0,appliedBody=0;bool absorbed=false;double relative=0;};
inline Output step(State& s,const Input& in) noexcept {
    Output o{};
    if(!in.on||in.reset){const bool had=s.f!=0;const auto resets=s.resets+(had?1:0);s=State{};s.resets=resets;return o;}
    const double dc=s.haveCamera?wrap(in.cameraYaw-s.lastCamera):0;
    s.lastCamera=in.cameraYaw;s.haveCamera=true;
    const double dr=(s.haveRoot&&in.haveRoot)?wrap(in.rootYaw-s.lastRoot):0;
    if(in.haveRoot){s.lastRoot=in.rootYaw;s.haveRoot=true;} // a frame without a root read: the next delta covers the gap
    const bool ours=s.driveMs&&in.now>=s.driveMs&&in.now-s.driveMs<=kDriveMs;
    if(ours&&in.context&&!in.userTurn&&!in.pause&&std::fabs(dc)<=kJumpRad&&dc!=0){
        s.f=wrap(s.f-dc);s.turnedDeg+=std::fabs(dc)*180/kPi;o.absorbed=true;
        if(std::fabs(dr)<=kJumpRad)s.fb=wrap(s.fb-dr);
        // right sign: a left request (lastDir +1) must give a camera yaw increase (Cry + = left)
        s.signAcc+=dc*double(s.lastDir);
        // checked every 3 deg, so a wrong guess (or a game turn in the same frames) is corrected within 3 deg; more than
        // kMaxFlips wrong turns = this machine's stick does not turn Henry the way we think: stop asking (until reset)
        if(std::fabs(s.signAcc)>=kSignCheckRad){if(s.signAcc<0){s.sign=-s.sign;++s.flips;}s.signAcc=0;}
    }
    o.applied=in.applyView?s.f:0;o.appliedBody=in.applyView?s.fb:0;
    const double relative=wrap(s.f+in.headYaw);o.relative=relative;
    if(!in.context||in.pause||in.userTurn||!in.headValid||!in.applyView||s.flips>kMaxFlips){s.signAcc=0;return o;}
    const double past=std::fabs(relative)-in.freeLookRad;
    if(past<=kStopRad){return o;}
    const float mag=float(std::clamp(past*kGain,double(kMinRx),double(kMaxRx)));
    // relative > 0: the head is left of Henry -> turn left = rx < 0 (the R133 bleed convention)
    s.lastDir=relative>0?1:-1;
    o.rx=(relative>0?-1.f:1.f)*float(s.sign)*mag;
    s.driveMs=in.now?in.now:1;++s.driveFrames;
    return o;
}

// R174 (Miki 9 Nov: "the hands are moving with that - instead of staying where the controller is. the same with free
// combat - our hands position should not drift with the body but stay relative to hmd"). the body frame yaw is no longer
// absorbed on its own from root turns (R173 only took root turns in the frames the camera also turned, within 150 ms of
// our rx; a root that turns after the camera - the game's turn lag - moved the hands with Henry). it now follows the view:
// body yaw = the yaw held out of the view + how far the root has turned against the camera since the view hold began.
// so head and hands keep the same relation they had without any hold, whatever lags. view hold 0 = body 0 (native).
struct BodyRef {double c0=0;bool have=false;double last=0;};
// view: total yaw held out of the view this call (R133 + R173), rel: camera yaw - root yaw (wrapped), haveRel: both fresh
inline double bodyYaw(BodyRef& r,double view,double rel,bool haveRel) noexcept {
    if(!haveRel)return r.last;                       // no root this call: keep the last (the next call covers the gap)
    if(std::fabs(view)<1e-4){r.c0=rel;r.have=true;r.last=0;return 0;}
    r.last=r.have?wrap(view+wrap(rel-r.c0)):view;
    return r.last;
}
inline std::atomic<float> bodyTotal{0.f};           // camera thread -> body frame: the one yaw for head/hands/weapon
// R180: RETIRED, not called any more (Miki 9 Nov: "by turning now the chest is coming forward, head stays back - my neck
// clips"): holding the eye back also held the VR head anchor back while Henry's chest turned forward with the root. the
// free look edge is 45 deg (R177), which alone made the view natural again. the pure rule stays for its test only.
// R177 eye position hold (Miki 9 Nov: "the HMD look of 30 is problematic - the view shifts more, it does not look quite
// natural; by 45 it is ok"). Henry turns around his root, his eyes sit in front of it, so every turn under you swung the
// view sideways by a few cm (2 x 12 cm x sin(turn/2)). while a hold is active (R133 / R173 body yaw not 0, or R173
// turning) that swing is taken back into a world offset p: the view (camera) and the VR body anchor (head / hands /
// weapon) stay where they were. p then glides back to Henry's real eyes (time constant kGlideS), so after a turn you end
// up in his head again, without a jump. horizontal only; no hold = p glides to 0 and stays 0.
struct OrbitHold {double px=0,py=0,relX=0,relY=0,lastYaw=0;bool have=false;std::uint64_t lastMs=0;};
constexpr double kGlideS=.9;              // seconds: the offset is 37 % after this, 5 % after 3x
constexpr double kMaxOrbitM=.40;          // a larger offset is not a turn swing (teleport / mount): dropped
// held: a body hold is active now. relX/relY: eye - root (world, metres, horizontal). rootYaw: Cry yaw. returns p.
inline void orbitStep(OrbitHold& o,bool held,double relX,double relY,double rootYaw,std::uint64_t now) noexcept {
    if(!std::isfinite(relX)||!std::isfinite(relY)||!std::isfinite(rootYaw)){o=OrbitHold{};return;}
    const double dt=o.lastMs&&now>=o.lastMs?double(now-o.lastMs)*.001:0.;
    if(o.have&&held){
        const double dr=wrap(rootYaw-o.lastYaw);
        if(std::fabs(dr)<=kJumpRad){ // the eye swing this root turn made: R(dr)*rel - rel, taken back
            const double c=std::cos(dr),s=std::sin(dr);
            const double sx=c*o.relX-s*o.relY-o.relX,sy=s*o.relX+c*o.relY-o.relY;
            o.px-=sx;o.py-=sy;
        }
    }
    if(dt>0){const double k=std::exp(-std::min(dt,.25)/kGlideS);o.px*=k;o.py*=k;}
    if(std::hypot(o.px,o.py)>kMaxOrbitM){o.px=o.py=0;}
    o.relX=relX;o.relY=relY;o.lastYaw=rootYaw;o.have=true;o.lastMs=now?now:1;
}
inline std::atomic<float> orbitX{0.f},orbitY{0.f},orbitZ{0.f}; // tracked camera -> VR body anchor (MODEL space, already / scale)
// ---- cross-thread publication ----
inline std::atomic<std::uint64_t> contextMs{0};     // worker: plain tracked gameplay (NativeFreeViewContextD42) and on foot
inline std::atomic<float> applied{0.f},appliedBody{0.f}; // camera thread: view / -> body frame (frik_pose_probe, weapon_tracking)
inline std::atomic<float> rx{0.f};inline std::atomic<std::uint64_t> rxMs{0};
inline std::atomic<float> turnedDeg{0.f},relativeDeg{0.f};
inline std::atomic<std::uint64_t> flips{0},resets{0},driveFrames{0};
inline bool contextFresh(std::uint64_t now) noexcept {const auto t=contextMs.load(std::memory_order_acquire);return t&&now>=t&&now-t<=100;}
inline float rxNow(std::uint64_t now) noexcept {
    const auto t=rxMs.load(std::memory_order_acquire);
    return t&&now>=t&&now-t<=60?rx.load(std::memory_order_acquire):0.f;
}
}
