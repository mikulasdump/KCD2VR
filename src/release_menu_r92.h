#pragma once
#include "vrik/crafting_menu_h18.h"
#include "vrik/native_crouch_r93.h"
#include "combat_menu49.h"
#include "controller_layout_r118.h" // R192 controller scheme
#include "subsystems/experimental/live_settings_d39.h" // R192 HasFaceTouchR118
#include "enemy_preset_r194.h" // R194
#include <array>
#include <algorithm>
#include <cmath>
#include <cwchar>
namespace kcd2vr::release_menu_r92 {
enum class Row { Vignette,Turn,Alchemy,Smith,Crouch,Hud,Glyphs,Dialogue,Enemies,ActivityPad,Roomscale,FastBlock,PointingTalk,Impale,BodyHeight,Gore,BlockIndicator,HudInfinity,CombatCross,MinimalHud,Tutorials,
 BodyRotation,FreeTargeting,HandSmoothing,ControllerScheme,HudMode,PalmLook,EnemyPreset,None }; // R192: body rotation .. palm look, R194 preset
struct Entry {Row id;const wchar_t* title;const wchar_t* detail;};
// R192 (Miki 10 Nov: menu rebuild for players, step 1 = IMMERSION AND CONTROLS): pages with a title each.
// the rows are the same saved keys as the developer pages (one owner per setting, nothing doubled):
//  body rotation = VrBodyFollowHmdR173 + VrBodyFollowDegR173, free targeting = VrFreeTargetingModeR133,
//  hand smoothing = VrMotionTrialD67 (2 capped / 4 uncapped), controller scheme = VrControllerLayoutR118 + VrPadGlyphStyle
//  (PS VR2 = PlayStation icons, the others Xbox; the old "Button icons" row is folded into it), HUD mode = VrContextualHud /
//  VrMinimalHudR141 (as VR HUD page), palm look = VrPalmLookR192. R193: the HUD rows live on the HUD menu only.
inline constexpr std::array<Entry,8> pages[4]{{{
 {Row::BodyRotation,L"HMD based body rotation",L"Look further aside than this and Henry turns after your head. OFF: only the stick."},
 {Row::FreeTargeting,L"Free targeting",L"FREE: game turns you to the enemy. FREE PLUS: never turns your view."},
 {Row::HandSmoothing,L"Hand smoothing",L"UNCAPPED: game fps above headset Hz. CAPPED: fps below headset Hz."},
 {Row::BodyHeight,L"Body height",L"HMD, native, or HMD with native height on horse and in crouch."},
 {Row::Turn,L"Joystick turning",L"Choose fixed-angle snap turns or smooth rotation."},
 {Row::Roomscale,L"Physical walking and dodges",L"Choose room movement, headset dodges, both, or neither."},
 {Row::Crouch,L"Crouching",L"Physical head movement or the native button; walking stays smooth."},
 {Row::Vignette,L"Comfort vignette",L"Darken peripheral vision while moving."}
}},{{
 {Row::ControllerScheme,L"Controller scheme",L"OPENXR: Menu = inventory. STEAMVR / PS VR2: X hold = inventory. PS VR2 icons."},
 {Row::ActivityPad,L"Activity gamepad controls",L"Session only: normal controller actions in activities."},
 {Row::PointingTalk,L"Index-finger pointing Quick Talk",L"Point while looking at a person or dog; keep pointing to choose. X stays available."},
 {Row::Alchemy,L"Alchemy hands",L"Semi follows native actions; 6DOF uses tracked hands."},
 {Row::Smith,L"Smithing hands",L"Semi follows native actions; 6DOF uses tracked tools."},
 {Row::Dialogue,L"Dialogue view",L"Cinematic native cameras or immersive first person."},
 {Row::Impale,L"Physical impalements",L"Let eligible corpses follow the blade; pull out to release."},
 {Row::Gore,L"Gore",L"Off, blood x2, or blood x2 + decapitation/dismemberment on kills (needs the gore pak)."}
}},{{
 {Row::EnemyPreset,L"Enemy difficulty",L"NORMAL: beta values. HARDCORE: faster guards. CUSTOM: your CUSTOM tab values."},
 {Row::FastBlock,L"Fast block response",L"Enter your tuned frontal guard after 0.10s; native outcome."},
 {Row::None,L"",L""},{Row::None,L"",L""},{Row::None,L"",L""},{Row::None,L"",L""},{Row::None,L"",L""},{Row::None,L"",L""}
}},{{ // R194 COMBAT > CUSTOM: block rate, difficulty and block speed sliders (drawn by vr_menu.cpp, no rows)
 {Row::None,L"",L""},{Row::None,L"",L""},{Row::None,L"",L""},{Row::None,L"",L""},{Row::None,L"",L""},{Row::None,L"",L""},{Row::None,L"",L""},{Row::None,L"",L""}
}}};
inline constexpr unsigned kPages=4;
inline constexpr const wchar_t* kPageTitles[kPages]{L"Body + movement",L"Controls + hands",L"Enemies",L"Custom enemies"};
inline constexpr unsigned kCustomEnemiesPage=3; // R194
// R193 (Miki 10 Nov: "we should have only 5 items on the right page - HUD, VR config, Graphics, Immersion and controls and
// combat"): the pages belong to two Home buttons. IMMERSION AND CONTROLS = pages 0..1, COMBAT = page 2 (the Combat
// step adds its pages here). the HUD rows of R192 page 3 are gone: every one of them is on the HUD menu.
enum class Section:unsigned {Immersion=0,Combat=1,Count};
inline constexpr unsigned kSectionFirst[2]{0,2},kSectionCount[2]{2,2}; // R194: COMBAT = Enemies + Custom (tabs, plus SENSITIVITY)
inline constexpr const wchar_t* kSectionTitle[2]{L"IMMERSION AND CONTROLS",L"COMBAT"};
constexpr unsigned sectionIndex(unsigned section) noexcept {return section<unsigned(Section::Count)?section:0;}
constexpr unsigned firstPage(unsigned section) noexcept {return kSectionFirst[sectionIndex(section)];}
constexpr unsigned pageCount(unsigned section) noexcept {return kSectionCount[sectionIndex(section)];}
// the page shown: page (any number) kept inside the section, < > wrap inside it
constexpr unsigned pageIn(unsigned section,unsigned page) noexcept {
    return page>=firstPage(section)&&page<firstPage(section)+pageCount(section)?page:firstPage(section);}
constexpr unsigned stepPage(unsigned section,unsigned page,int dir) noexcept {
    const unsigned f=firstPage(section),n=pageCount(section),i=pageIn(section,page)-f;
    return f+(dir<0?(i+n-1)%n:(i+1)%n);}
inline constexpr const auto& rows(unsigned page) noexcept{return pages[page<kPages?page:0];}
inline constexpr const wchar_t* pageTitle(unsigned page) noexcept{return kPageTitles[page<kPages?page:0];}
// R192 HMD based body rotation: OFF -> 30 deg -> 45 deg -> OFF (another free look angle from the developer page: next = 45 or OFF)
template<class S> void nextBodyRotation(S& s) noexcept {
 auto& d=s.live_d39;
 if(!d.bodyFollowR173){d.bodyFollowR173=true;d.bodyFollowDegR173=30.f;}
 else if(d.bodyFollowDegR173<37.5f)d.bodyFollowDegR173=45.f;
 else d.bodyFollowR173=false;
}
template<class S> bool advance(S& s,Row row) noexcept {
 switch(row){
 case Row::Vignette:s.comfort_vignette51=(std::clamp(s.comfort_vignette51,0,3)+1)%4;break;
 case Row::Turn:s.vr_turn_mode=s.vr_turn_mode==0?1:0;break;
 case Row::Alchemy:crafting_menu_h18::select(s,minigame_hands45::Alchemy,s.alchemy_mode67==3?1:3);break;
 case Row::Smith:crafting_menu_h18::select(s,minigame_hands45::Smithing,s.smithing_mode67==3?1:3);break;
 case Row::Crouch:native_crouch_r93::select(s);break;
 case Row::BodyHeight:s.body_height_mode98=native_crouch_r93::next_body_height_r98(s.body_height_mode98);break;
 case Row::Hud:s.contextual_hud=!s.contextual_hud;break;
 case Row::Glyphs:s.vr_pad_glyph_style=s.vr_pad_glyph_style==4?2:4;break;
 case Row::BodyRotation:nextBodyRotation(s);break; // R192
 case Row::FreeTargeting:s.live_d39.freeTargetingModeR133=s.live_d39.freeTargetingModeR133==2?1:2;s.live_d39.freeTargetingD42=true;break; // R192: FREE <-> FREE PLUS
 case Row::HandSmoothing:s.live_d39.motionTrialD67=s.live_d39.motionTrialD67==4?2:4;break; // R192: UNCAPPED <-> CAPPED
 case Row::ControllerScheme:s.live_d39.controllerLayoutR118=controller_layout_r118::Next(s.live_d39.controllerLayoutR118); // R192: layout + icons
  if constexpr(live_settings_d39::HasFaceTouchR118<S>::value)s.quick_access_face_touch_r112=controller_layout_r118::FaceTouchFor(s.live_d39.controllerLayoutR118);
  s.vr_pad_glyph_style=controller_layout_r118::Clamp(s.live_d39.controllerLayoutR118)==controller_layout_r118::Psvr2?4:2;break;
 case Row::HudMode:{const int mode=s.live_d39.minimalHudR141?1:s.contextual_hud?0:2;const int next=mode==2?0:mode==0?1:2; // R192: NORMAL -> IMMERSIVE -> MINIMAL
  s.contextual_hud=next==0;s.live_d39.minimalHudR141=next==1;break;}
 case Row::PalmLook:s.live_d39.palmLookR192=!s.live_d39.palmLookR192;break; // R192: VrPalmLookR192
 case Row::EnemyPreset:if constexpr(enemy_preset_r194::HasFields<S>::value){(void)enemy_preset_r194::next(s,s.live_d39.enemyPresetR194);break;}else return false; // R194
 case Row::Dialogue:s.dialogue_camera_mode=s.dialogue_camera_mode==2?0:2;s.cyclopean_mode=0;break;
 case Row::Enemies:{const bool hardcore=std::fabs(s.npc_difficulty_percent-50.f)<.01f&&std::fabs(s.npc_defense_speed-1.7f)<.001f;s.npc_difficulty_percent=hardcore?0.f:50.f;s.npc_defense_speed=hardcore?1.3f:1.7f;break;}
 case Row::ActivityPad:s.native_activity_controls163=!s.native_activity_controls163;break;
 case Row::Roomscale:s.roomscale_mode=(std::clamp(s.roomscale_mode,0,3)+1)%4;s.live_d39.nativeRoomscaleD76=s.roomscale_mode==1||s.roomscale_mode==2;break;
 case Row::FastBlock:s.fast_block_r92=!s.fast_block_r92;break;
 case Row::BlockIndicator:s.show_block_indicator_h24=!s.show_block_indicator_h24;break; // R124: same VrShowBlockIndicator key as COMBAT
 case Row::HudInfinity:s.live_d39.hudInfinityR125=!s.live_d39.hudInfinityR125;break; // R125: VrHudInfinityR125
 case Row::CombatCross:s.live_d39.combatCrossR125=!s.live_d39.combatCrossR125;break; // R125: VrCombatCrossR125
 case Row::MinimalHud:s.live_d39.minimalHudR141=!s.live_d39.minimalHudR141;break; // R141: VrMinimalHudR141
 case Row::Tutorials:s.live_d39.hideTutorialsR141=!s.live_d39.hideTutorialsR141;break; // R141: VrHideTutorialsR141
 case Row::PointingTalk:s.gesture_talk=!s.gesture_talk;break;
 case Row::Impale:s.impale_hang=combat_menu49::impaleToggle(s.impale_hang,s.impale_hang_on45);break;
 case Row::Gore:{const int next=((s.decapitation||s.dismemberment_d80?2:s.bloody_fights49?1:0)+1)%3;s.bloody_fights49=next>=1;s.decapitation=next==2;s.dismemberment_d80=next==2;break;} // R98: OFF -> BLOOD x2 (VrBloodyFights) -> + VrDecapitation + VrDismemberment
 default:return false;
 }return true;
}
template<class S> const wchar_t* value(const S& s,Row row) noexcept {
 switch(row){
 case Row::Vignette:return s.comfort_vignette51==0?L"OFF":s.comfort_vignette51==1?L"LOW":s.comfort_vignette51==2?L"MEDIUM":L"STRONG";
 case Row::Turn:return s.vr_turn_mode==0?L"SNAP":L"SMOOTH";
 case Row::Alchemy:return crafting_menu_h18::label(s.alchemy_mode67);
 case Row::Smith:return crafting_menu_h18::label(s.smithing_mode67);
 case Row::Crouch:return native_crouch_r93::label(s);
 case Row::BodyHeight:return native_crouch_r93::body_height_label_r98(s.body_height_mode98);
 case Row::Hud:return s.contextual_hud?L"ON":L"OFF";
 case Row::Glyphs:return s.vr_pad_glyph_style==4?L"PLAYSTATION":L"XBOX";
 case Row::BodyRotation:{const auto& d=s.live_d39;if(!d.bodyFollowR173)return L"OFF";
  return d.bodyFollowDegR173<22.5f?L"15 DEG":d.bodyFollowDegR173<37.5f?L"30 DEG":d.bodyFollowDegR173<52.5f?L"45 DEG":L"60 DEG";}
 case Row::FreeTargeting:return s.live_d39.freeTargetingModeR133<=0?L"GAME LOCK":s.live_d39.freeTargetingModeR133==2?L"FREE PLUS":L"FREE";
 case Row::HandSmoothing:return s.live_d39.motionTrialD67==4?L"UNCAPPED":s.live_d39.motionTrialD67==0?L"BASELINE":L"CAPPED";
 case Row::ControllerScheme:return controller_layout_r118::Name(s.live_d39.controllerLayoutR118);
 case Row::HudMode:return s.live_d39.minimalHudR141?L"MINIMAL":s.contextual_hud?L"IMMERSIVE":L"NORMAL";
 case Row::EnemyPreset:if constexpr(enemy_preset_r194::HasFields<S>::value)return enemy_preset_r194::label(enemy_preset_r194::current(s,s.live_d39.enemyPresetR194));else return L""; // R194
 case Row::PalmLook:return !s.live_d39.palmLookR192?L"OFF":(s.contextual_hud||s.live_d39.minimalHudR141)?L"ON":L"ON (not in NORMAL)";
 case Row::Dialogue:return s.dialogue_camera_mode==2?L"IMMERSIVE":L"CINEMATIC";
 case Row::Enemies:return std::fabs(s.npc_difficulty_percent-50.f)<.01f&&std::fabs(s.npc_defense_speed-1.7f)<.001f?L"HARDCORE":std::fabs(s.npc_difficulty_percent)<.01f&&std::fabs(s.npc_defense_speed-1.3f)<.001f?L"NORMAL":L"CUSTOM";
 case Row::ActivityPad:return s.native_activity_controls163?L"ON":L"OFF";
 case Row::Roomscale:return s.roomscale_mode==1?L"WALK":s.roomscale_mode==2?L"WALK + DODGE":s.roomscale_mode==3?L"DODGE":L"OFF";
 case Row::FastBlock:return s.fast_block_r92?L"ON":L"OFF";
 case Row::BlockIndicator:return s.show_block_indicator_h24?L"SHOWN":L"HIDDEN";
 case Row::HudInfinity:return s.live_d39.hudInfinityR125?L"INF":L"DISTANCE";
 case Row::CombatCross:return s.live_d39.combatCrossR125?L"SHOWN":L"HIDDEN";
 case Row::MinimalHud:return s.live_d39.minimalHudR141?L"ON":L"OFF";
 case Row::Tutorials:return s.live_d39.hideTutorialsR141?L"HIDDEN":L"SHOWN";
 case Row::PointingTalk:return s.gesture_talk?L"ON":L"OFF";
 case Row::Impale:return s.impale_hang?L"ON":L"OFF";
 case Row::Gore:return s.decapitation&&s.dismemberment_d80?(s.bloody_fights49?L"BLOOD x2 + HEADS + LIMBS":L"HEADS + LIMBS"):s.decapitation?L"HEADS ONLY":s.dismemberment_d80?L"LIMBS ONLY":s.bloody_fights49?L"BLOOD x2":L"OFF";
 default:return L"";
 }
}
}
