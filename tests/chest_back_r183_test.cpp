// R183/R185 trunk behind the view, forward on a reach (src/vrik/chest_back_r183.h)
#include "vrik/chest_back_r183.h"
#include <cstdio>
#include <cmath>
namespace cb=kcd2vr::chest_back_r183;
static unsigned total=0,fail=0;
static void ck(bool ok,const char* what){++total;if(!ok){++fail;std::printf("FAIL %s\n",what);}}
int main(){
    auto s=cb::shift(0,1,4);ck(std::fabs(s.x)<1e-7&&std::fabs(s.y+.04f)<1e-6&&s.z==0,"facing +y: 4 cm back = -y");
    s=cb::shift(3,0,5);ck(std::fabs(s.x+.05f)<1e-6&&std::fabs(s.y)<1e-7,"any length forward is normalized");
    s=cb::shift(0,1,-6);ck(std::fabs(s.y-.06f)<1e-6,"negative = forward");
    s=cb::shift(0,1,0);ck(s.x==0&&s.y==0,"0 = nothing");
    s=cb::shift(0,0,4);ck(s.x==0&&s.y==0,"no forward: nothing");
    s=cb::shift(0,1,50);ck(std::fabs(s.y+.10f)<1e-6,"clamped to 10 cm");
    // R185 reach
    ck(cb::trunkCm(4,.10f)==4.f&&cb::trunkCm(4,.30f)==4.f,"hands near the body: chest back 4 cm");
    ck(cb::trunkCm(4,.60f)==-5.f&&cb::trunkCm(4,.90f)==-5.f,"full reach: trunk 5 cm forward");
    const float mid=cb::trunkCm(4,.45f);ck(std::fabs(mid+.5f)<1e-5,"half way: smooth (-0.5 cm)");
    bool mono=true;float last=99;for(int i=0;i<=40;++i){const float v=cb::trunkCm(4,.2f+.015f*i);mono=mono&&v<=last+1e-6f;last=v;}ck(mono,"never jumps back while reaching further");
    ck(cb::trunkCm(4,-1.f)==4.f&&cb::trunkCm(4,NAN)==4.f,"no hand: chest back");
    ck(cb::trunkCm(0,.60f)==-5.f,"chest back OFF still reaches forward (the bug fix is not tied to the option)");
    ck(cb::cm.load()==4.f&&cb::clampCm(NAN)==4.f,"default 4 cm");
    std::printf("chest_back_r183_test: %u/%u passed\n",total-fail,total);return fail?1:0;
}
