// R191/R193 root_lead_r191.h: eye / weapon on the root predicted N steps ahead (position only, smoothed step).
#include "vrik/root_lead_r191.h"
#include <cstdio>
#include <cmath>
namespace rl=kcd2vr::root_lead_r191;namespace pc=kcd2vr::pose_core;
static int fails=0,total=0;
static void ck(bool b,const char* m){++total;if(!b){++fails;std::printf("FAIL %s\n",m);}}
static bool near(pc::Vec3 a,pc::Vec3 b,double e=1e-9){return pc::length(a-b)<e;}
int main(){
    const pc::Quat id{};
    {rl::State s;auto l=rl::make(s,1,id,{0,0,0},id,{0,.036,0},1.f); // first frame: the step itself
     ck(l.on&&near(rl::point(l,{0,.036,1}),{0,.072,1}),"one step forward");}
    {rl::State s;auto l=rl::make(s,1,id,{0,0,0},id,{0,.036,0},.5f);ck(near(rl::point(l,{0,.036,1}),{0,.054,1}),"half step");}
    {rl::State s;auto l=rl::make(s,1,id,{0,0,0},id,{0,.036,0},-1.f);ck(near(rl::point(l,{0,.036,1}),{0,0,1}),"minus one step");}
    {rl::State s;auto l=rl::make(s,1,id,{0,0,0},id,{0,.036,0},0.f);ck(!l.on&&near(rl::point(l,{1,2,3}),{1,2,3}),"0 = off");}
    {rl::State s;auto l=rl::make(s,1,id,{0,0,0},id,{0,.3,0},1.f);ck(!l.on,"teleport step ignored");}
    // R193: a hitch frame (double step) moves the lead only 35 % of the difference, not a whole extra step
    {rl::State s;for(std::uint64_t k=1;k<=20;++k)(void)rl::make(s,k,id,{0,0,0},id,{0,.02,0},1.f);
     auto l=rl::make(s,21,id,{0,0,0},id,{0,.04,0},1.f);ck(std::fabs(l.step.y-(.02+.02*rl::kSmooth))<1e-9,"hitch frame smoothed");
     // the same frame again (second camera call / other hand): no second update
     l=rl::make(s,21,id,{0,0,0},id,{0,.04,0},1.f);ck(std::fabs(l.step.y-(.02+.02*rl::kSmooth))<1e-9,"same key = same step");
     // standing still: the step decays to 0 within a few frames
     for(std::uint64_t k=22;k<40;++k)l=rl::make(s,k,id,{5,5,5},id,{5,5,5},1.f);ck(pc::length(l.step)<1e-4,"standing still -> no lead");}
    // R193: no turn lead (stick turn 2 deg a frame): a point 20 cm in front of the root stays put
    {rl::State s;const double a=2*3.14159265358979/180;const pc::Quat q2=pc::from_rotation_vector({0,0,a});
     auto l=rl::make(s,1,id,{0,0,0},q2,{0,0,0},1.f);ck(l.on&&near(rl::point(l,{0,.2,0}),{0,.2,0})&&pc::angle(rl::orient(l,q2),q2)<1e-12,"no turn lead");
     l=rl::make(s,2,id,{0,0,0},pc::from_rotation_vector({0,0,.5}),{0,0,0},1.f);ck(!l.on,"cut scene turn ignored");}
    ck(rl::clampCycles(5.f)==2.f&&rl::clampCycles(-3.f)==-1.f&&rl::clampCycles(NAN)==1.f,"clamp");
    std::printf(fails?"root_lead_r191_test FAILED %d/%d\n":"root_lead_r191_test OK %d/%d\n",total-fails,total);return fails?1:0;
}
