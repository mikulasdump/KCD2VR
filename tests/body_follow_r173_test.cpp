// R173 body follows the headset: pure rules (src/vrik/body_follow_r173.h).
#include "vrik/body_follow_r173.h"
#include <cstdio>
#include <cmath>
namespace bf=kcd2vr::body_follow_r173;
static unsigned total=0,fail=0;
static void ck(bool ok,const char* what){++total;if(!ok){++fail;std::printf("FAIL %s\n",what);}}
static double rad(double d){return d*bf::kPi/180;}
// a tiny game: rx turns the camera yaw (Cry + = left; rx < 0 = left) by gameSign * rx * 120 deg/s
struct Game {double yaw=0;double sign=1;void apply(float rx,double dt){yaw=bf::wrap(yaw-sign*double(rx)*rad(120)*dt);}};
static bf::Input base(double cam,double head,std::uint64_t now){
    bf::Input in{};in.freeLookRad=30*bf::kPi/180; // the cases below were written for a 30 deg edge
in.haveRoot=true;in.rootYaw=cam;in.on=true;in.context=true;in.applyView=true;in.headValid=true;in.cameraYaw=cam;in.headYaw=head;in.now=now;return in;}
// visible yaw = camera + applied + head; must not move while only Henry turns
static double visible(double cam,double applied,double head){return bf::wrap(cam+applied+head);}
int main(){
    {   // inside the free look: nothing
        bf::State s{};auto o=bf::step(s,base(0,rad(20),10));ck(o.rx==0&&o.applied==0,"20 deg: no turn");
        o=bf::step(s,base(0,rad(-29),20));ck(o.rx==0,"-29 deg: no turn");
    }
    {   // past it: Henry turns left after the head, the view stays, ends at the edge
        bf::State s{};Game g{};std::uint64_t now=1000;const double head=rad(90);
        auto o=bf::step(s,base(g.yaw,head,now));ck(o.rx<0,"head 90 left -> rx < 0 (left)");
        const double v0=visible(g.yaw,o.applied,head);double maxDrift=0;
        for(int i=0;i<400;++i){g.apply(o.rx,.011);now+=11;o=bf::step(s,base(g.yaw,head,now));
            maxDrift=std::max(maxDrift,std::fabs(bf::wrap(visible(g.yaw,o.applied,head)-v0)));}
        ck(maxDrift<rad(.5),"the view did not turn while Henry turned");
        ck(std::fabs(std::fabs(o.relative)-rad(30))<rad(1.5),"Henry stops at the 30 deg edge");
        ck(o.rx==0,"no rx at the edge");
        ck(std::fabs(g.yaw-rad(60))<rad(1.5),"Henry turned 60 deg");
        ck(std::fabs(o.appliedBody-o.applied)<1e-9,"body frame took the same yaw out (root turned with the camera)");
    }
    {   // a 180 deg physical turn: Henry ends within 30 deg of the head
        bf::State s{};Game g{};std::uint64_t now=1;const double head=rad(179);
        auto o=bf::step(s,base(g.yaw,head,now));
        for(int i=0;i<600;++i){g.apply(o.rx,.011);now+=11;o=bf::step(s,base(g.yaw,head,now));}
        ck(std::fabs(o.relative)<=rad(31),"180: Henry faces within 30 deg of the head");
    }
    {   // a machine where rx turns the other way: corrected within a few degrees, no runaway
        bf::State s{};Game g{};g.sign=-1;std::uint64_t now=1;const double head=rad(-80);
        auto o=bf::step(s,base(g.yaw,head,now));double worst=0;
        for(int i=0;i<600;++i){g.apply(o.rx,.011);now+=11;o=bf::step(s,base(g.yaw,head,now));worst=std::max(worst,std::fabs(o.relative));}
        ck(s.flips==1,"one sign flip");ck(worst<rad(86),"wrong first guess costs < 6 deg");
        ck(std::fabs(std::fabs(o.relative)-rad(30))<rad(1.5),"then it settles at the edge");
    }
    {   // the player's stick turn is never absorbed (the view turns as before) and pauses the follow
        bf::State s{};std::uint64_t now=1;auto in=base(0,rad(60),now);auto o=bf::step(s,in);ck(o.rx!=0,"driving");
        in=base(rad(10),rad(60),now+11);in.userTurn=true;o=bf::step(s,in);ck(o.rx==0&&!o.absorbed&&s.f==0,"stick turn: not absorbed, no rx");
    }
    {   // locked fight / R133 offset: pause; context off: pause; dialogue: offset not shown but kept
        bf::State s{};std::uint64_t now=1;auto in=base(0,rad(60),now);in.pause=true;auto o=bf::step(s,in);ck(o.rx==0,"pause: no rx");
        in=base(0,rad(60),now);in.context=false;o=bf::step(s,in);ck(o.rx==0,"no context: no rx");
        bf::State t{};Game g{};now=1;auto p=base(0,rad(70),now);auto q=bf::step(t,p);
        for(int i=0;i<50;++i){g.apply(q.rx,.011);now+=11;q=bf::step(t,base(g.yaw,rad(70),now));}
        ck(t.f!=0,"some yaw is held");
        p=base(g.yaw,rad(70),now+11);p.applyView=false;q=bf::step(t,p);ck(q.applied==0&&q.rx==0&&t.f!=0,"dialogue: shown 0, kept, no rx");
        p=base(g.yaw,rad(70),now+22);q=bf::step(t,p);ck(q.applied==t.f,"after: shown again");
    }
    {   // a teleport / scripted turn right after our rx is not absorbed; reset and OFF clear
        bf::State s{};std::uint64_t now=1;auto o=bf::step(s,base(0,rad(60),now));(void)o;
        o=bf::step(s,base(rad(40),rad(60),now+11));ck(!o.absorbed&&s.f==0,"40 deg jump not absorbed");
        s.f=.3;auto in=base(0,0,now+22);in.reset=true;o=bf::step(s,in);ck(s.f==0&&o.applied==0&&s.resets==1,"reset clears");
        s.f=.3;in=base(0,0,now+33);in.on=false;o=bf::step(s,in);ck(s.f==0,"OFF clears");
    }
    {   // rx is proportional (smooth) and capped
        bf::State s{};auto o=bf::step(s,base(0,rad(31),1));ck(o.rx<0&&o.rx>=-bf::kMinRx-1e-6f,"just past: minimum rx");
        bf::State t{};o=bf::step(t,base(0,rad(170),1));ck(o.rx==-1.f,"far past: full rx");
        bf::State u{};auto in=base(0,rad(50),1);in.freeLookRad=rad(60);o=bf::step(u,in);ck(o.rx==0,"60 deg free look: 50 is inside");
    }
    {   // R174 body yaw follows the view: root lagging the camera does not move the hands
        bf::BodyRef r{};ck(bf::bodyYaw(r,0,rad(3),true)==0&&r.have,"no hold: body 0, reference taken");
        // camera turned 20 (view holds -20), root not yet: body must stay where the view says (-20 + 20 = 0 against the root)
        ck(std::fabs(bf::bodyYaw(r,rad(-20),rad(3+20),true)-0)<1e-9,"root lag: body 0 while the root has not turned");
        // root catches up: rel back to 3, body -20 like the view
        ck(std::fabs(bf::bodyYaw(r,rad(-20),rad(3),true)-rad(-20))<1e-9,"root turned: body -20");
        ck(std::fabs(bf::bodyYaw(r,rad(-20),0,false)-rad(-20))<1e-9,"no root read: keep last");
        ck(bf::bodyYaw(r,0,rad(5),true)==0,"hold gone: body 0");
        bf::BodyRef q{};ck(std::fabs(bf::bodyYaw(q,rad(10),rad(1),true)-rad(10))<1e-9,"no reference yet: body = view");
    }
    {   // R177 eye position hold: a root turn under a hold does not swing the eye, then it glides back
        bf::OrbitHold o{};const double rx0=0,ry0=.12; // eye 12 cm in front of the root (Cry +Y forward at yaw 0)
        bf::orbitStep(o,true,rx0,ry0,0,1000);ck(o.px==0&&o.py==0,"first call: nothing");
        const double a=rad(20),ex=-std::sin(a)*.12,ey=std::cos(a)*.12; // root turned 20 deg left: the eye swung
        bf::orbitStep(o,true,ex,ey,a,1000);
        ck(std::fabs((ex+o.px)-rx0)<1e-9&&std::fabs((ey+o.py)-ry0)<1e-9,"held turn: eye + p = where it was");
        double before=std::hypot(o.px,o.py);for(int i=1;i<=9;++i){bf::orbitStep(o,false,ex,ey,a,1000+std::uint64_t(i)*100);}
        ck(std::hypot(o.px,o.py)<before*.4&&std::hypot(o.px,o.py)>before*.3,"0.9 s later: glided to 37 %");
        for(int i=0;i<40;++i){bf::orbitStep(o,false,ex,ey,a,1900+std::uint64_t(i+1)*250);}
        ck(std::hypot(o.px,o.py)<1e-4,"back in Henry's eyes");
        bf::OrbitHold q{};bf::orbitStep(q,false,0,.12,0,1000);bf::orbitStep(q,false,ex,ey,a,1011);ck(q.px==0&&q.py==0,"no hold (own stick turn): no offset");
        bf::OrbitHold j{};bf::orbitStep(j,true,0,.12,0,1000);bf::orbitStep(j,true,0,.12,rad(40),1011);ck(j.px==0&&j.py==0,"a 40 deg jump is not absorbed");
    }
    ck(bf::clampDeg(5)==10&&bf::clampDeg(200)==90&&bf::clampDeg(NAN)==45,"clampDeg");
    ck(!bf::enabled.load()&&bf::freeLookDeg.load()==45.f,"defaults OFF, 45 deg (R177)");
    std::printf("body_follow_r173_test: %u/%u passed\n",total-fail,total);
    return fail?1:0;
}
