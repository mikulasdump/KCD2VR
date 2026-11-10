// R168 host test: "Weapon in hand: depth" (VrMeleeGripDepthR168, melee_grip_r123.h) + its Experiments row (OBJECTS page 3 row 8)
// g++ -std=c++20 -I src -I src/vrik tests/melee_grip_depth_r168_test.cpp -o melee_grip_depth_r168_test && ./melee_grip_depth_r168_test
#include "vrik/melee_grip_r123.h"
#include "subsystems/experimental/live_settings_d39.h"
#include <cstdio>
#include <cwchar>
#include <limits>
#include <set>
using namespace kcd2vr;
namespace M=melee_grip_r123;namespace live=live_settings_d39;
struct VrMenuState {
    live::Settings live_d39{};bool quick_access_face_touch_r112=false;
    bool vrik_body_follow=true,smooth_move51=false,precise_contacts44=true,exp_wall_slide=false,grab_hand_stays43=true;
    int roomscale_mode=1,vr_turn_mode=1,body_height_mode98=2;
    bool activity_hands59c7f[4]{true,true,true,false};
    int alchemy_mode67=1,smithing_mode67=1,smith_anvil_mode_a3=1;
    bool craft_trial_h41=false,smith_auto_hold36=true,user_shoulder_holsters=true,chest_dagger54=true;
    int weapon_release_mode_t1=0;bool throwing_supply_t2=false,gesture_talk=false,native_kill_nf=false;
    int exp_hide_stowed_bow=0,physical_f9_rung61=0,physical_f9_next_h6=1;float clinch_distance32=.55f;
    int comfort_vignette51=0,vr_pad_glyph_style=2,dialogue_camera_mode=0,cyclopean_mode=0,impale_hang=0,impale_hang_on45=3;
    bool contextual_hud=true,fast_block_r92=false,show_block_indicator_h24=false,native_activity_controls163=false;
    bool bloody_fights49=false,decapitation=false,dismemberment_d80=false,hmd_body_height54=false,native_crouch_r93=false;
    float npc_difficulty_percent=0,npc_defense_speed=1.3f;
};
static unsigned total=0,fail=0;
static void ck(bool b,const char* s){++total;if(!b){++fail;std::printf("FAIL %s\n",s);}}
static bool near(double a,double b,double e=1e-6){return a>b-e&&a<b+e;}
int main(){
    ck(near(M::depthScale(.08,0.f),1.0),"0 cm = authored socket unchanged");
    ck(near(M::depthScale(.08,2.f),.06/.08),"+2 cm: wrist 2 cm closer to the weapon = weapon 2 cm deeper in the hand");
    ck(near(M::depthScale(.08,-2.f),.10/.08),"-2 cm: 2 cm further out");
    ck(near(M::depthScale(.03,4.f),M::kMinSpanM/.03),"never closer than 1 cm");
    ck(near(M::depthScale(.08,9.f),.04/.08)&&near(M::depthScale(.08,-9.f),.10/.08),"clamped -2..4");
    ck(near(M::depthScale(0.,2.f),1.0)&&M::clampDepth(std::numeric_limits<float>::quiet_NaN())==0.f,"zero span / NaN safe");
    VrMenuState s{};ck(s.live_d39.meleeGripDepthR168==0.f,"default 0");
    const auto pageR184=live::pageOf(live::Id::MeleeGripDepthR168);bool rowR184=false;
    for(const auto& r:live::rows(pageR184))if(r.id==live::Id::MeleeGripDepthR168)rowR184=!r.readOnly&&!r.session;
    ck(live::group(pageR184)==live::Group::Npc&&rowR184,"R184: WEAPONS tab (weapon in hand), saved, never read-only");
    std::set<unsigned> ids;for(unsigned n=0;n<live::kPageCount;++n)for(const auto& row:live::rows(n))ids.insert(unsigned(row.id));
    ck(ids.count(unsigned(live::Id::GripStowTrial))==1,"the stow-button row still exists");
    float seen[16]{};unsigned n=0;
    for(unsigned k=0;k<13;++k){live::advance(s,live::Id::MeleeGripDepthR168);seen[n++]=s.live_d39.meleeGripDepthR168;}
    ck(near(seen[0],.5)&&near(seen[3],2.0)&&near(seen[7],4.0)&&near(seen[8],-2.0)&&near(seen[12],0.0),"cycle 0 > 0.5 .. 4 > -2 .. 0 in 0.5 cm steps");
    wchar_t text[200]{};live::advance(s,live::Id::MeleeGripDepthR168);live::advance(s,live::Id::MeleeGripDepthR168);live::advance(s,live::Id::MeleeGripDepthR168);live::advance(s,live::Id::MeleeGripDepthR168);
    {const auto& rowsR184=live::rows(pageR184);for(const auto& r:rowsR184)if(r.id==live::Id::MeleeGripDepthR168)live::label(s,r,text,200);};ck(std::wcsstr(text,L"+2.0 cm")!=nullptr,"label +2.0 cm");
    std::printf("melee_grip_depth_r168_test: %u/%u passed\n",total-fail,total);return fail?1:0;
}
