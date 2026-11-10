// R167 host test: left trigger -> LB at the look / Follow prompts (lt_prompt_lb_r167.h)
// g++ -std=c++20 -I src tests/lt_prompt_lb_r167_test.cpp -o lt_prompt_lb_r167_test && ./lt_prompt_lb_r167_test
#include "subsystems/hud_ui_controls/runtime/lt_prompt_lb_r167.h"
#include <cstdio>
namespace lp=kcd2vr::lt_prompt_lb_r167;
static unsigned total=0,fail=0;
static void ck(bool b,const char* s){++total;if(!b){++fail;std::printf("FAIL %s\n",s);}}
int main(){
    ck(lp::fresh(1000,1000)&&lp::fresh(1000,1300)&&!lp::fresh(1000,1301)&&!lp::fresh(0,5)&&!lp::fresh(1000,999),"300 ms freshness, never backwards");
    {lp::Route r{};ck(!lp::step(r,.2f,true,false),"light touch is no press");
     ck(lp::step(r,.8f,true,false),"press at the prompt = LB");
     ck(lp::step(r,.5f,false,false),"prompt map gone mid-press (look started): LB stays held");
     ck(lp::step(r,.4f,false,true),"hysteresis: still held above 0.35, decision kept");
     ck(!lp::step(r,.2f,true,false)&&!r.down&&!r.pass,"release ends it");}
    {lp::Route r{};ck(!lp::step(r,.9f,false,false),"press without prompt = normal LT");
     ck(!lp::step(r,.9f,true,false),"prompt appearing mid-press does not switch the press (no half-way change)");
     lp::step(r,0.f,true,false);ck(lp::step(r,.9f,true,false),"next press at the prompt passes (no latch)");}
    {lp::Route r{};ck(!lp::step(r,.9f,true,true),"blocked (VR block / menu) = normal LT even at the prompt");}
    std::printf("%s %u/%u\n",fail?"FAILED":"PASS",total-fail,total);return fail?1:0;
}
