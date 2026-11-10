#pragma once
// R194 enemy difficulty for players (Miki 10 Nov: "Enemy difficulty: Normal (prerelease) / Hardcore (current) / Custom
// tab" ... "normal is normal" + the beta zip = R139, and "custom NPC difficulty should have a tab where you can choose the
// block rate and difficulty slider and block change from low skill to high skill (1.0-3.0 slider)").
//   NORMAL   = the beta (R139) starter values: difficulty 43 %, defence x1.75, guard timing x1.0, block raise x1.40,
//              block speed BY SKILL x2.0 (skill 0) .. x3.0 (skill 30), stagger rule 1, block rate BY SKILL.
//   HARDCORE = his R190 values (R190 starter INI): 32 %, x1.70, guard timing x1.45, block raise x1.55, by skill OFF
//              (x2.0 / x3.0 kept), stagger rule 2, block rate BY SKILL.
//   CUSTOM   = whatever is set: the COMBAT > CUSTOM tab (block rate, difficulty, low / high skill speed, all speed) or
//              the developer pages. choosing CUSTOM keeps the current values; any custom change selects CUSTOM.
// VrEnemyPresetR194: 0 NORMAL, 1 HARDCORE, 2 CUSTOM, missing = found from the values (no match = CUSTOM). the values
// themselves keep their own keys (VrNpcDifficulty, VrNpcDefenseSpeed, VrNpcBlockSpeed, VrNpcBlockAnimSpeed,
// VrDirectNpcBlockBySkill, VrDirectNpcBlockSpeedLow / High, VrDirectStaggerRule, VrDirectNpcBlock). pure.
#include <algorithm>
#include <atomic>
#include <cmath>
#include <type_traits>
#include <utility>
namespace kcd2vr::enemy_preset_r194 {
inline constexpr const char* kKey="VrEnemyPresetR194";
enum Preset:int {Normal=0,Hardcore=1,Custom=2,Count=3};
inline std::atomic<int> stored{-1}; // -1 = not saved yet: found from the values
struct Values {float difficulty,defense,guardTiming,blockRaise;bool bySkill;float lo,hi;int stagger,blockRate;};
inline constexpr Values kNormal{43.f,1.75f,1.f,1.4f,true,2.f,3.f,1,0};
inline constexpr Values kHardcore{32.f,1.7f,1.45f,1.55f,false,2.f,3.f,2,0};
constexpr const Values& values(int p) noexcept {return p==Hardcore?kHardcore:kNormal;}
constexpr int clampStored(int p) noexcept {return p>=0&&p<Count?p:-1;}
template<class S> Values read(const S& s) noexcept {
    return {s.npc_difficulty_percent,s.npc_defense_speed,s.exp_npc_block_speed18,s.npc_block_anim_speed109,s.exp_direct_block_timing39,
            s.exp_direct_block_lo39,s.exp_direct_block_hi39,s.exp_stagger_rule44,s.npc_block39};
}
inline bool same(const Values& a,const Values& b) noexcept {
    auto f=[](float x,float y){return std::fabs(x-y)<.006f;};
    return f(a.difficulty,b.difficulty)&&f(a.defense,b.defense)&&f(a.guardTiming,b.guardTiming)&&f(a.blockRaise,b.blockRaise)&&
           a.bySkill==b.bySkill&&f(a.lo,b.lo)&&f(a.hi,b.hi)&&a.stagger==b.stagger&&a.blockRate==b.blockRate;
}
template<class S> int detect(const S& s) noexcept {const auto v=read(s);return same(v,kNormal)?Normal:same(v,kHardcore)?Hardcore:Custom;}
// the preset shown: CUSTOM when chosen; NORMAL / HARDCORE only while the values still are that preset (a developer page
// change shows CUSTOM, no stale label); never saved = the one the values match
template<class S> int current(const S& s,int savedChoice) noexcept {
    const int c=clampStored(savedChoice);
    if(c==Custom)return Custom;
    if(c>=0)return same(read(s),values(c))?c:Custom;
    return detect(s);
}
template<class S> void write(S& s,const Values& v) noexcept {
    s.npc_difficulty_percent=v.difficulty;s.npc_defense_speed=v.defense;s.exp_npc_block_speed18=v.guardTiming;s.npc_block_anim_speed109=v.blockRaise;
    s.exp_direct_block_timing39=v.bySkill;s.exp_direct_block_lo39=v.lo;s.exp_direct_block_hi39=v.hi;s.exp_stagger_rule44=v.stagger;s.npc_block39=v.blockRate;
}
// NORMAL -> HARDCORE -> CUSTOM -> NORMAL. returns the new choice (saved in savedChoice)
template<class S> int next(S& s,int& savedChoice) noexcept {
    const int n=(current(s,savedChoice)+1)%Count;
    if(n!=Custom)write(s,values(n));
    savedChoice=n;return n;
}
constexpr const wchar_t* label(int p) noexcept {return p==Normal?L"NORMAL":p==Hardcore?L"HARDCORE":L"CUSTOM";}
// the fields exist (VrMenuState); portable test views without them only cycle the number
template<class S,class=void> struct HasFields:std::false_type{};
template<class S> struct HasFields<S,std::void_t<decltype(std::declval<S&>().exp_direct_block_lo39),decltype(std::declval<S&>().npc_block39),
    decltype(std::declval<S&>().exp_stagger_rule44),decltype(std::declval<S&>().npc_block_anim_speed109)>>:std::true_type{};
}
