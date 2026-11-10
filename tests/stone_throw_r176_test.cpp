// R176 stone throwing: crouched + raised empty left hand + left Grip = the game's RB distraction throw
#include "subsystems/throw_drop/stone_throw_r176.h"
#include <cstdio>
namespace st=kcd2vr::stone_throw_r176;
static unsigned total=0,fail=0;
static void ck(bool ok,const char* what){++total;if(!ok){++fail;std::printf("FAIL %s\n",what);}}
int main(){
    {st::State s{};ck(st::step(s,true,true,true,1,false),"crouched + raised + grip = RB");
     ck(st::step(s,true,true,true,.4f,false),".4 keeps RB (hysteresis)");
     ck(!st::step(s,true,true,true,.2f,false),"release = RB up = throw");ck(s.throws==1,"one throw");}
    {st::State s{};ck(!st::step(s,true,false,true,1,false),"standing: no RB (RB is Quick Speak there)");
     ck(!st::step(s,true,true,true,1,false),"crouching during the same press: still not ours");
     st::step(s,true,true,true,0,false);ck(st::step(s,true,true,true,1,false),"next press while crouched: RB");}
    {st::State s{};ck(!st::step(s,true,true,false,1,false),"hand low (knife pouch / grab): no RB");
     st::step(s,true,true,true,0,false);ck(st::step(s,true,true,true,1,false),"raised next time: RB");
     ck(st::step(s,true,true,false,1,false),"lowering the hand mid press keeps the aim (the press decided)");}
    {st::State s{};ck(!st::step(s,false,true,true,1,false),"OFF");
     st::step(s,true,true,true,0,false);ck(!st::step(s,true,true,true,1,true),"menu");
     st::step(s,true,true,true,0,false);ck(st::step(s,true,true,true,1,false),"ours");ck(!st::step(s,true,true,true,1,true),"menu mid press: RB up");
     ck(!st::step(s,true,true,true,1,false),"and it stays the menu's press");}
    {ck(st::fresh(1000,1300)&&!st::fresh(1000,1301)&&!st::fresh(0,10),"fresh 300 ms");}
    ck(st::enabled.load(),"default ON");
    std::printf("stone_throw_r176_test: %u/%u passed\n",total-fail,total);
    return fail?1:0;
}
