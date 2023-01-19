#pragma once
#include "YunutyEngine.h"

class RA2UnitGraphic : public D2DAnimatedSprite
{
public:
    void SetFacingAngle(double angle); 
    void SetUnitSprites(wstring spriteRootFolderName);
    RA2UnitGraphic();
    void PlayBirth();
    void PlayDie();
    void PlayIdle();
    void PlayAttack();
    void PlayAttack_fragile();
    void PlayMove();
    void PlayMove_fragile();
    void PlayStand();
    void PlayStand_fragile();
private:
    // spriteRootFolder는 곧 유닛명에 대응되며, 아래의 static 멤버들은 유닛명을 키값으로 캐싱된다.
    static unordered_map<wstring, const SpriteAnim*> birth_Anims;
    static unordered_map<wstring, vector<const SpriteAnim*>> die_Anims;
    static unordered_map<wstring, vector<const SpriteAnim*>> idle_Anims;
    static unordered_map<wstring, vector<const SpriteAnim*>> attack_Anims;
    static unordered_map<wstring, vector<const SpriteAnim*>> attack_fragile_Anims;
    static unordered_map<wstring, vector<const SpriteAnim*>> move_Anims;
    static unordered_map<wstring, vector<const SpriteAnim*>> move_fragile_Anims;
    static unordered_map<wstring, vector<const SpriteAnim*>> stand_Anims;
    static unordered_map<wstring, vector<const SpriteAnim*>> stand_fragile_Anims;
    static vector<const SpriteAnim*> LoadDirectionalAnims(wstring root);
    static vector<const SpriteAnim*> LoadVariableAnims(wstring root);

    double facingAngle = 0;
    wstring spriteRootFolderName;
    void PlayAnim(unordered_map<wstring, const SpriteAnim*>);
    void PlayRandomAnim(unordered_map<wstring, vector<const SpriteAnim*>>& animMap);
    void PlayDirectionalAnim(unordered_map<wstring, vector<const SpriteAnim*>>& animMap);
};
