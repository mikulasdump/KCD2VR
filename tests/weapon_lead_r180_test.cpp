// R180/R182 weapon drawn ahead with the hand (src/vrik/weapon_lead_r180.h)
#include "vrik/weapon_lead_r180.h"
#include <cstdio>
#include <cmath>
namespace wl=kcd2vr::weapon_lead_r180;namespace pc=kcd2vr::pose_core;
static unsigned total=0,fail=0;
static void ck(bool ok,const char* what){++total;if(!ok){++fail;std::printf("FAIL %s\n",what);}}
static void step(wl::State& s,float c,std::uint64_t key,std::uint64_t n,std::uint64_t ms,pc::Quat& q,pc::Vec3& p){wl::lead(s,c,key,n,ms*1000000ull,ms,q,p);}
int main(){
    {   // 1 cm per 11 ms cycle along x: 1 cycle ahead = +1 cm
        wl::State s{};pc::Quat q{};pc::Vec3 p{0,0,0};step(s,1,7,1,1000,q,p);ck(p.x==0,"first sample: as is");
        p={.01,0,0};q={};step(s,1,7,2,1011,q,p);ck(std::fabs(p.x-.02)<1e-9,"1 cycle ahead");
        p={.01,0,0};q={};step(s,1,7,2,1012,q,p);ck(std::fabs(p.x-.02)<1e-9,"same sample again: same lead");
        p={.02,0,0};q={};step(s,.5f,7,3,1022,q,p);ck(std::fabs(p.x-.025)<1e-9,"0.5 cycle");
        p={.03,0,0};q={};step(s,0,7,4,1033,q,p);ck(p.x==.03,"0 = off");
    }
    {   // R182: a noisy step is smoothed (no frame to frame jump)
        wl::State s{};pc::Quat q{};pc::Vec3 p{};double x=0;std::uint64_t t=1000;step(s,1,7,1,t,q,p);
        for(int i=2;i<40;++i){x+=.01;t+=11;p={x,0,0};q={};step(s,1,7,std::uint64_t(i),t,q,p);}
        const double steady=p.x-x;ck(std::fabs(steady-.01)<1e-4,"steady motion: 1 cycle lead");
        x+=.03;t+=11;p={x,0,0};q={};step(s,1,7,40,t,q,p); // one 3 cm jump (noise)
        ck(p.x-x<.02,"a single noisy 3 cm step leads by less than 2 cm (smoothed)");
        for(int i=41;i<80;++i){t+=11;p={x,0,0};q={};step(s,1,7,std::uint64_t(i),t,q,p);}
        ck(std::fabs(p.x-x)<1e-4,"held still: the lead fades to 0");
    }
    {   // rotation: 5 deg per cycle about z -> 10 deg shown
        wl::State s{};pc::Quat q{};pc::Vec3 p{};step(s,1,7,1,1000,q,p);
        q=pc::from_rotation_vector({0,0,5*3.14159265358979/180});p={};step(s,1,7,2,1011,q,p);
        ck(std::fabs(pc::length(pc::rotation_vector(q))-10*3.14159265358979/180)<1e-6,"rotation led");
    }
    {   // caps, gap reset, new weapon
        wl::State s{};pc::Quat q{};pc::Vec3 p{};step(s,2,7,1,1000,q,p);p={.5,0,0};q={};step(s,2,7,2,1011,q,p);ck(std::fabs(p.x-.6)<1e-9,"capped at 10 cm");
        p={.6,0,0};q={};step(s,1,7,3,1200,q,p);ck(p.x==.6,"100 ms gap: reset");
        p={.7,0,0};q={};step(s,1,9,4,1211,q,p);ck(p.x==.7,"another weapon: reset");
        p={NAN,0,0};step(s,1,9,5,1222,q,p);ck(!s.have,"nan: reset");
    }
    ck(wl::cycles.load()==1.f&&wl::clampCycles(5)==2.f&&wl::clampCycles(-1)==0.f,"default 1, clamp 0..2");
    std::printf("weapon_lead_r180_test: %u/%u passed\n",total-fail,total);return fail?1:0;
}
