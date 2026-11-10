// R178 weapon feel: handle pivot point + arm swing power (src/vrik/weapon_feel_r178.h)
#include "vrik/weapon_feel_r178.h"
#include <cstdio>
#include <cmath>
namespace wf=kcd2vr::weapon_feel_r178;
static unsigned total=0,fail=0;
static void ck(bool ok,const char* what){++total;if(!ok){++fail;std::printf("FAIL %s\n",what);}}
int main(){
    {   // the C55 sword: wrist (0.0239,-0.0661,0.0161), blade along +y, tip on the centre line
        wf::V h{};ck(wf::handlePoint({.0239,-.0661,.0161},{0,.85,0},h),"sword: handle found");
        ck(std::fabs(h.x)<1e-9&&std::fabs(h.y+.0661)<1e-9&&std::fabs(h.z)<1e-9,"handle point = centre line at the wrist's height");
        ck(wf::handlePoint({.0239,.0661,.0161},{0,-.85,0},h)&&std::fabs(h.y-.0661)<1e-9,"blade along -y");
        ck(wf::handlePoint({.9,.02,.01},{0,0,0},h)&&std::fabs(h.x-.9)<1e-9&&h.y==0&&h.z==0,"blade along x");
        ck(!wf::handlePoint({0,0,0},{0,.05,0},h),"5 cm: not a handle");
        ck(!wf::handlePoint({0,0,0},{0,.8,0},h),"wrist exactly on the line: nothing to correct");
        ck(!wf::handlePoint({.2,0,0},{0,.8,0},h),"20 cm off the line: not a fist grip");
        ck(!wf::handlePoint({NAN,0,0},{0,.8,0},h),"nan");
    }
    {   // energy: flick 45 %, arm full, smooth, unknown unchanged
        ck(std::fabs(wf::energyScale(.2,10)-.45)<1e-9,"flick (hand 2 %): 45 %");
        ck(std::fabs(wf::energyScale(.8,10)-.45)<1e-9,"8 %: 45 %");
        ck(wf::energyScale(3,10)==1.,"30 %: full");ck(wf::energyScale(7,7)==1.,"thrust (hand = point): full");
        const double mid=wf::energyScale(1.9,10);ck(mid>.7&&mid<.75,"19 %: in between (~72 %)");
        ck(wf::energyScale(-1,10)==1.&&wf::energyScale(1,0)==1.&&wf::energyScale(NAN,5)==1.,"unknown: unchanged");
    }
    ck(!wf::handlePivot.load()&&wf::armPower.load(),"R180: pivot retired OFF, arm power ON");
    std::printf("weapon_feel_r178_test: %u/%u passed\n",total-fail,total);
    return fail?1:0;
}
