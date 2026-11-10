// R189 a contact keeps its committed attack for 150 ms after the last forward sample; a stab is kept 250 ms
#include "vrik/stab_contact_r189.h"
#include <cstdio>
namespace sc=kcd2vr::stab_contact_r189;
static unsigned total=0,fail=0;
static void ck(bool ok,const char* what){++total;if(!ok){++fail;std::printf("FAIL %s\n",what);}}
int main(){
    ck(sc::recentForward(1000,50,1000,50),"same sample: as before");
    ck(sc::recentForward(1000,50,1058,55)&&sc::recentForward(1000,50,1141,60)&&sc::recentForward(1000,50,1150,61),"58 / 141 / 150 ms later (the lost stabs of the R188 log): kept");
    ck(!sc::recentForward(1000,50,1151,61),"151 ms: a new contact, not this attack");
    ck(!sc::recentForward(0,0,10,1)&&!sc::recentForward(1000,50,999,49),"no forward sample / clock back: no");
    ck(sc::keepThrust(1,1000,1000)&&sc::keepThrust(1,1000,1250)&&!sc::keepThrust(1,1000,1251),"stab kept 250 ms through a backward frame");
    ck(!sc::keepThrust(0,1000,1100)&&!sc::keepThrust(2,1000,1100)&&!sc::keepThrust(1,0,10),"cut / pommel / no commit: old rule");
    std::printf("stab_contact_r189_test: %u/%u passed\n",total-fail,total);return fail?1:0;
}
