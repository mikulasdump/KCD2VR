// R194 enemy difficulty preset (COMBAT > ENEMIES / CUSTOM): NORMAL = beta values, HARDCORE = R190 values, CUSTOM keeps.
#include "enemy_preset_r194.h"
#include "release_menu_r92.h"
#include <cstdio>
#include <cwchar>
namespace ep=kcd2vr::enemy_preset_r194;namespace r=kcd2vr::release_menu_r92;
struct S {
 kcd2vr::live_settings_d39::Settings live_d39{};int comfort_vignette51=0,vr_turn_mode=0,alchemy_mode67=1,smithing_mode67=1,vr_pad_glyph_style=2,dialogue_camera_mode=0,cyclopean_mode=0,roomscale_mode=0,impale_hang=0,impale_hang_on45=0;
 bool activity_hands59c7f[4]{},smith_auto_hold36=true,native_crouch_r93=false,contextual_hud=false,native_activity_controls163=false,fast_block_r92=false,gesture_talk=false;
 int body_height_mode98=2;bool bloody_fights49=false,decapitation=false,dismemberment_d80=false,show_block_indicator_h24=false;
 // the C39 / C44 / D34 fields as in VrMenuState (direct/host.inl), with his game INI of 10 Nov
 float npc_difficulty_percent=0.f,npc_defense_speed=1.3f,exp_npc_block_speed18=1.45f,npc_block_anim_speed109=1.55f;
 bool exp_direct_block_timing39=false;float exp_direct_block_lo39=2.f,exp_direct_block_hi39=3.f;int exp_stagger_rule44=2,npc_block39=0;
};
static int n=0,f=0;static void ck(bool ok,const char* w){++n;if(!ok){++f;std::printf("FAIL %s\n",w);}}
static bool is(const wchar_t* a,const wchar_t* b){return std::wcscmp(a,b)==0;}
int main(){
 static_assert(ep::HasFields<S>::value,"fields");
 S s{};ck(s.live_d39.enemyPresetR194==-1,"not saved yet");
 ck(is(r::value(s,r::Row::EnemyPreset),L"CUSTOM"),"his INI (0 %, x1.3 after the old row) = CUSTOM");
 ck(r::advance(s,r::Row::EnemyPreset)&&s.live_d39.enemyPresetR194==ep::Normal&&is(r::value(s,r::Row::EnemyPreset),L"NORMAL"),"-> NORMAL");
 ck(s.npc_difficulty_percent==43.f&&s.npc_defense_speed==1.75f&&s.exp_npc_block_speed18==1.f&&s.npc_block_anim_speed109==1.4f&&s.exp_direct_block_timing39&&
    s.exp_direct_block_lo39==2.f&&s.exp_direct_block_hi39==3.f&&s.exp_stagger_rule44==1&&s.npc_block39==0,"NORMAL = beta R139 values");
 ck(r::advance(s,r::Row::EnemyPreset)&&s.live_d39.enemyPresetR194==ep::Hardcore&&is(r::value(s,r::Row::EnemyPreset),L"HARDCORE"),"-> HARDCORE");
 ck(s.npc_difficulty_percent==32.f&&s.npc_defense_speed==1.7f&&s.exp_npc_block_speed18==1.45f&&s.npc_block_anim_speed109==1.55f&&!s.exp_direct_block_timing39&&
    s.exp_stagger_rule44==2,"HARDCORE = R190 values");
 ck(r::advance(s,r::Row::EnemyPreset)&&s.live_d39.enemyPresetR194==ep::Custom&&is(r::value(s,r::Row::EnemyPreset),L"CUSTOM")&&s.npc_difficulty_percent==32.f,"-> CUSTOM keeps the values");
 ck(r::advance(s,r::Row::EnemyPreset)&&s.live_d39.enemyPresetR194==ep::Normal,"CUSTOM -> NORMAL");
 // a developer page change while NORMAL is saved: shown as CUSTOM, next = HARDCORE? no: CUSTOM -> NORMAL
 s.npc_difficulty_percent=50.f;ck(is(r::value(s,r::Row::EnemyPreset),L"CUSTOM"),"changed values = CUSTOM label");
 ck(r::advance(s,r::Row::EnemyPreset)&&s.live_d39.enemyPresetR194==ep::Normal&&s.npc_difficulty_percent==43.f,"shown CUSTOM -> NORMAL");
 // never saved, values = HARDCORE: found
 S h{};ep::write(h,ep::kHardcore);ck(ep::current(h,-1)==ep::Hardcore,"found from values");
 std::printf("enemy_preset_r194_test %d/%d\n",n-f,n);return f?1:0;
}
