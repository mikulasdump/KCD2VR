// R184 host test: Experiments tabs re-sorted (Miki 9 Nov: "move the settings to corresponding tabs ... I am loosing sight.
// could not find chest settings"). one page table: every setting has exactly one row, a tab walks only its own pages.
#include "subsystems/experimental/live_settings_d39.h"
#include <cstdio>
#include <cwchar>
#include <set>
using namespace kcd2vr;namespace live=live_settings_d39;
static unsigned total=0,fail=0;
static void ck(bool ok,const char* what){++total;if(!ok){++fail;std::printf("FAIL %s\n",what);}}
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
int main(){
    std::set<unsigned> seen;unsigned rows=0;bool once=true;
    for(unsigned p=0;p<live::kPageCount;++p)for(const auto& r:live::rows(p))if(r.id!=live::Id::None){++rows;once=once&&seen.insert(unsigned(r.id)).second;}
    ck(once,"every setting has exactly ONE row (duplicates removed)");ck(rows==157,"157 settings, none lost (R194 enemy preset)");
    for(unsigned g=0;g<unsigned(live::Group::Count);++g)ck(live::group(g)==live::Group(g)&&live::pageNumber(g)==1,"a tab opens its page 1");
    // walk every tab: next until the end, prev back to page 1, never leaving the tab
    for(unsigned g=0;g<unsigned(live::Group::Count);++g){
        VrMenuState s{};unsigned sel=g;const unsigned n=live::pagesIn(live::Group(g));bool ok=true;
        auto click=[&](live::Rect r){return live::dispatch(sel,r.l+2,r.t+2,s,[](const VrMenuState&){return true;},[]{return true;});};
        for(unsigned k=2;k<=n;++k){ok=ok&&click(live::kMotionNextD69)==live::Click::Changed&&live::group(sel)==live::Group(g)&&live::pageNumber(sel)==k;}
        if(n>1){ok=ok&&click(live::kMotionNextD69)==live::Click::Changed&&live::pageNumber(sel)==n;} // stays on the last page
        for(unsigned k=n;k>1;--k){ok=ok&&click(live::kMotionPrevD69)==live::Click::Changed&&live::pageNumber(sel)==k-1;}
        ok=ok&&sel==g;ck(ok,"tab walks its own pages and back");
    }
    ck(live::pagesIn(live::Group::Combat)==5,"COMBAT has 5 pages (enemies, free targeting +, combos, directions, clinch + impale)");
    ck(live::rows(unsigned(live::Group::MotionD67))[0].id==live::Id::ChestBackR183,"VRIK+ page 1 row 1 = chest back");
    ck(live::group(live::pageOf(live::Id::BodyFollowR173))==live::Group::MotionD67,"body follows headset on VRIK+");
    ck(live::group(live::pageOf(live::Id::NpcBlockR177))==live::Group::Combat&&live::group(live::pageOf(live::Id::FreeTargetingD42))==live::Group::Combat,"enemy settings on COMBAT");
    ck(live::group(live::pageOf(live::Id::BodyLeadR191))==live::Group::Hands,"R191 body lead on VRIK");
    {ck(live::group(live::pageOf(live::Id::EnemyPresetR194))==live::Group::Combat,"R194 enemy preset on COMBAT");VrMenuState b{};
     ck(live::advance(b,live::Id::EnemyPresetR194)&&b.live_d39.enemyPresetR194>=0,"R194 row never refused (test view: number only)");}
    {VrMenuState b{};ck(b.live_d39.bodyLeadR191==1.f,"R191 default 1 step");live::advance(b,live::Id::BodyLeadR191);ck(b.live_d39.bodyLeadR191==1.5f,"1 -> 1.5");
     live::advance(b,live::Id::BodyLeadR191);live::advance(b,live::Id::BodyLeadR191);ck(b.live_d39.bodyLeadR191==-1.f,"2 -> -1 (never refused)");
     live::advance(b,live::Id::BodyLeadR191);live::advance(b,live::Id::BodyLeadR191);ck(b.live_d39.bodyLeadR191==0.f,"-> OFF");
     wchar_t tx[200]{};live::label(b,live::rows(live::pageOf(live::Id::BodyLeadR191))[7],tx,200);ck(std::wcsstr(tx,L"OFF")!=nullptr,"label OFF");}
    ck(live::group(live::pageOf(live::Id::WeaponLeadR180))==live::Group::Npc&&live::group(live::pageOf(live::Id::MeleeGripDepthR168))==live::Group::Npc,"weapon in hand on WEAPONS");
    ck(live::group(live::pageOf(live::Id::SceneryBigPropsR158))==live::Group::Objects&&live::group(live::pageOf(live::Id::InstantInteractionsD72))==live::Group::Objects,"props + interactions on OBJECTS");
    ck(live::group(live::pageOf(live::Id::RangedRtFireR175))==live::Group::RepairsD65&&live::group(live::pageOf(live::Id::StoneThrowR176))==live::Group::RepairsD65,"bow, crossbow, gun, stones on RANGED");
    ck(live::group(live::pageOf(live::Id::MinimalHudR141))==live::Group::View,"HUD on VIEW");
    ck(std::wcscmp(live::kTabsText[unsigned(live::Group::Npc)],L"WEAPONS")==0&&std::wcscmp(live::kTabsText[unsigned(live::Group::RepairsD65)],L"RANGED")==0,"tab names");
    ck(live::pageOf(live::Id::HudWalkoffD70)==live::kPageCount&&live::pageOf(live::Id::ChestDagger)==live::kPageCount,"INI-only / removed ids have no row");
    std::printf("menu_layout_r184_test: %u/%u passed\n",total-fail,total);return fail?1:0;
}
