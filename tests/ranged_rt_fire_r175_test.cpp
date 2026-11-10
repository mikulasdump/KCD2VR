// R175 crossbow / gun: RT alone shoots (src/subsystems/ranged_reload/ranged_rt_fire_r175.h)
#include "subsystems/ranged_reload/ranged_rt_fire_r175.h"
#include <cstdio>
namespace rf=kcd2vr::ranged_rt_fire_r175;
static unsigned total=0,fail=0;
static void ck(bool ok,const char* what){++total;if(!ok){++fail;std::printf("FAIL %s\n",what);}}
int main(){
    {   // hold RT: LT at once, RT after 250 ms, LT 150 ms after release, then native
        rf::State s{};auto o=rf::step(s,true,true,1,0,false,1000);ck(o.own&&o.lt==1&&o.rt==0,"press: aim, no fire yet");
        o=rf::step(s,true,true,1,0,false,1200);ck(o.lt==1&&o.rt==0,"200 ms: still aiming");
        o=rf::step(s,true,true,1,0,false,1250);ck(o.lt==1&&o.rt==1,"250 ms: fire");
        o=rf::step(s,true,true,0,0,false,1400);ck(o.own&&o.lt==1&&o.rt==0,"release: aim kept");
        o=rf::step(s,true,true,0,0,false,1549);ck(o.lt==1,"tail 149 ms");
        o=rf::step(s,true,true,0,0,false,1551);ck(!o.own&&o.lt==0&&o.rt==0,"tail over: native");
        ck(s.presses==1,"one press");
    }
    {   // short tap: fires at 250 ms for 80 ms, then keeps the aim 150 ms
        rf::State s{};rf::step(s,true,true,1,0,false,1000);auto o=rf::step(s,true,true,0,0,false,1050);
        ck(o.own&&o.lt==1&&o.rt==0,"tap released at 50 ms: aiming");
        o=rf::step(s,true,true,0,0,false,1260);ck(o.rt==1&&o.lt==1,"tap: the shot at 260 ms");
        o=rf::step(s,true,true,0,0,false,1340);ck(o.rt==0&&o.lt==1,"tap: shot over, aim kept");
        o=rf::step(s,true,true,0,0,false,1490);ck(!o.own,"tap: native after the tail");
    }
    {   // own LT held: unchanged
        rf::State s{};auto o=rf::step(s,true,true,1,1,false,1000);ck(!o.own&&o.lt==1&&o.rt==1,"own aim: passthrough");
        o=rf::step(s,true,true,1,0,false,1100);ck(!o.own&&o.rt==1&&o.lt==0,"LT released mid press: still the player's press");
    }
    {   // OFF / no ranged weapon / blocked: native
        rf::State s{};auto o=rf::step(s,false,true,1,0,false,1000);ck(!o.own&&o.rt==1&&o.lt==0,"OFF");
        o=rf::step(s,true,false,1,0,false,1100);ck(!o.own&&o.rt==1,"no crossbow");
        o=rf::step(s,true,true,1,0,true,1200);ck(!o.own&&o.rt==1,"menu");
        o=rf::step(s,true,true,1,0,false,1300);ck(!o.own,"a press that started while blocked stays native");
        o=rf::step(s,true,true,0,0,false,1400);o=rf::step(s,true,true,1,0,false,1500);ck(o.own&&o.lt==1,"the next press is ours");
        o=rf::step(s,true,true,1,0,true,1600);ck(!o.own&&o.lt==0,"menu opens mid press: let go at once");
    }
    {   // hysteresis: .5 does not start a press, .4 does not end one
        rf::State s{};auto o=rf::step(s,true,true,.5f,0,false,1000);ck(!o.own,".5 is no press");
        o=rf::step(s,true,true,.6f,0,false,1010);ck(o.own,".6 starts");o=rf::step(s,true,true,.4f,0,false,1300);ck(o.own&&o.rt==.4f,".4 keeps it (fires with the raw value)");
    }
    ck(rf::enabled.load(),"default ON");
    std::printf("ranged_rt_fire_r175_test: %u/%u passed\n",total-fail,total);
    return fail?1:0;
}
