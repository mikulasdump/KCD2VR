// R192 IMMERSION AND CONTROLS rebuild (step 1 of the player menu): new rows, one saved owner each.
#include "release_menu_r92.h"
#include "ui_palm_look_r192.h"
#include <cstdio>
#include <cwchar>
#include <set>
namespace r=kcd2vr::release_menu_r92;
struct S {
 kcd2vr::live_settings_d39::Settings live_d39{};int comfort_vignette51=0,vr_turn_mode=0,alchemy_mode67=1,smithing_mode67=1,vr_pad_glyph_style=2,dialogue_camera_mode=0,cyclopean_mode=0,roomscale_mode=0,impale_hang=0,impale_hang_on45=0;
 bool activity_hands59c7f[4]{},smith_auto_hold36=true,native_crouch_r93=false,contextual_hud=false,native_activity_controls163=false,fast_block_r92=false,gesture_talk=false;
 int body_height_mode98=2;float npc_difficulty_percent=0,npc_defense_speed=1.3f;
 bool bloody_fights49=false,decapitation=false,dismemberment_d80=false,show_block_indicator_h24=false,quick_access_face_touch_r112=false;
};
static int n=0,f=0;static void ck(bool ok,const char* w){++n;if(!ok){++f;std::printf("FAIL %s\n",w);}}
static bool is(const wchar_t* a,const wchar_t* b){return std::wcscmp(a,b)==0;}
int main(){
 S s{};
 // body rotation: OFF -> 30 -> 45 -> OFF, the same VrBodyFollowHmdR173 / VrBodyFollowDegR173 fields
 s.live_d39.bodyFollowR173=false;ck(is(r::value(s,r::Row::BodyRotation),L"OFF"),"rotation OFF");
 ck(r::advance(s,r::Row::BodyRotation)&&s.live_d39.bodyFollowR173&&s.live_d39.bodyFollowDegR173==30.f&&is(r::value(s,r::Row::BodyRotation),L"30 DEG"),"-> 30");
 ck(r::advance(s,r::Row::BodyRotation)&&s.live_d39.bodyFollowR173&&s.live_d39.bodyFollowDegR173==45.f&&is(r::value(s,r::Row::BodyRotation),L"45 DEG"),"-> 45");
 ck(r::advance(s,r::Row::BodyRotation)&&!s.live_d39.bodyFollowR173,"-> OFF");
 s.live_d39.bodyFollowR173=true;s.live_d39.bodyFollowDegR173=60.f;ck(is(r::value(s,r::Row::BodyRotation),L"60 DEG")&&r::advance(s,r::Row::BodyRotation)&&!s.live_d39.bodyFollowR173,"developer 60 -> OFF");
 // free targeting FREE <-> FREE PLUS, game lock -> FREE PLUS
 s.live_d39.freeTargetingModeR133=1;ck(is(r::value(s,r::Row::FreeTargeting),L"FREE"),"FREE");
 ck(r::advance(s,r::Row::FreeTargeting)&&s.live_d39.freeTargetingModeR133==2&&s.live_d39.freeTargetingD42&&is(r::value(s,r::Row::FreeTargeting),L"FREE PLUS"),"-> FREE PLUS");
 ck(r::advance(s,r::Row::FreeTargeting)&&s.live_d39.freeTargetingModeR133==1,"-> FREE");
 s.live_d39.freeTargetingModeR133=0;s.live_d39.freeTargetingD42=false;ck(is(r::value(s,r::Row::FreeTargeting),L"GAME LOCK")&&r::advance(s,r::Row::FreeTargeting)&&s.live_d39.freeTargetingModeR133==2&&s.live_d39.freeTargetingD42,"game lock -> FREE PLUS");
 // hand smoothing UNCAPPED (4) <-> CAPPED (2)
 s.live_d39.motionTrialD67=4;ck(is(r::value(s,r::Row::HandSmoothing),L"UNCAPPED"),"UNCAPPED");
 ck(r::advance(s,r::Row::HandSmoothing)&&s.live_d39.motionTrialD67==2&&is(r::value(s,r::Row::HandSmoothing),L"CAPPED"),"-> CAPPED");
 ck(r::advance(s,r::Row::HandSmoothing)&&s.live_d39.motionTrialD67==4,"-> UNCAPPED");
 // controller scheme: layout + icons + PS VR2 face touch
 s.live_d39.controllerLayoutR118=0;s.vr_pad_glyph_style=4;
 ck(r::advance(s,r::Row::ControllerScheme)&&s.live_d39.controllerLayoutR118==1&&s.vr_pad_glyph_style==2&&!s.quick_access_face_touch_r112&&is(r::value(s,r::Row::ControllerScheme),L"STEAMVR"),"-> STEAMVR, Xbox icons");
 ck(r::advance(s,r::Row::ControllerScheme)&&s.live_d39.controllerLayoutR118==2&&s.vr_pad_glyph_style==4&&s.quick_access_face_touch_r112&&is(r::value(s,r::Row::ControllerScheme),L"PS VR2"),"-> PS VR2, PlayStation icons, face touch");
 ck(r::advance(s,r::Row::ControllerScheme)&&s.live_d39.controllerLayoutR118==0&&s.vr_pad_glyph_style==2&&!s.quick_access_face_touch_r112,"-> OPENXR");
 // palm look label in NORMAL
 s.contextual_hud=false;s.live_d39.minimalHudR141=false;ck(is(r::value(s,r::Row::PalmLook),L"ON (not in NORMAL)"),"palm look ON but not in NORMAL");
 // every row once, no Glyphs / MinimalHud / Hud rows left (folded), titles
 std::set<int> seen;bool once=true;
 for(unsigned p=0;p<r::kPages;++p){ck(r::pageTitle(p)[0]!=0,"page title");for(const auto& e:r::rows(p)){if(e.id==r::Row::None)continue;once=once&&seen.insert(int(e.id)).second;
   ck(e.id!=r::Row::Glyphs&&e.id!=r::Row::MinimalHud&&e.id!=r::Row::Hud,"folded rows gone");}}
 ck(once,"every row once");ck(seen.size()==18,"R193: 18 rows on 3 pages (HUD rows only on the HUD menu)");
 ck(!seen.count(int(r::Row::HudMode))&&!seen.count(int(r::Row::PalmLook)),"R193 no HUD rows");
 // R193 sections: IMMERSION AND CONTROLS = pages 0..1, COMBAT = page 2; < > stay inside
 ck(r::firstPage(0)==0&&r::pageCount(0)==2&&r::firstPage(1)==2&&r::pageCount(1)==2&&r::kCustomEnemiesPage==3,"sections (R194 COMBAT = ENEMIES + CUSTOM)");
 ck(r::stepPage(0,0,+1)==1&&r::stepPage(0,1,+1)==0&&r::stepPage(0,0,-1)==1&&r::stepPage(1,2,+1)==3&&r::stepPage(1,3,+1)==2,"walk inside section");
 ck(r::pageIn(1,0)==2&&r::pageIn(0,2)==0&&r::pageIn(0,1)==1,"page kept in section");
 ck(r::rows(r::firstPage(1))[0].id==r::Row::EnemyPreset&&r::rows(r::firstPage(1))[1].id==r::Row::FastBlock,"COMBAT page");
 for(const auto& e:r::rows(r::kCustomEnemiesPage))ck(e.id==r::Row::None,"CUSTOM page drawn by the menu, no rows");
 ck(r::rows(0)[0].id==r::Row::BodyRotation&&r::rows(0)[1].id==r::Row::FreeTargeting&&r::rows(0)[2].id==r::Row::HandSmoothing&&r::rows(1)[0].id==r::Row::ControllerScheme,"order");
 std::printf("release_menu_r192_test %d/%d\n",n-f,n);return f?1:0;
}
