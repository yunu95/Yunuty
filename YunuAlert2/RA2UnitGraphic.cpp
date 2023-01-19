#include "YunutyEngine.h"
#include "RA2UnitGraphic.h"

unordered_map<wstring, const SpriteAnim*> RA2UnitGraphic::birth_Anims;
unordered_map<wstring, vector<const SpriteAnim*>> RA2UnitGraphic::die_Anims;
unordered_map<wstring, vector<const SpriteAnim*>> RA2UnitGraphic::idle_Anims;
unordered_map<wstring, vector<const SpriteAnim*>> RA2UnitGraphic::attack_Anims;
unordered_map<wstring, vector<const SpriteAnim*>> RA2UnitGraphic::attack_fragile_Anims;
unordered_map<wstring, vector<const SpriteAnim*>> RA2UnitGraphic::move_Anims;
unordered_map<wstring, vector<const SpriteAnim*>> RA2UnitGraphic::move_fragile_Anims;
unordered_map<wstring, vector<const SpriteAnim*>> RA2UnitGraphic::stand_Anims;
unordered_map<wstring, vector<const SpriteAnim*>> RA2UnitGraphic::stand_fragile_Anims;

RA2UnitGraphic::RA2UnitGraphic()
{
    SetIsRepeating(true);
}
void RA2UnitGraphic::SetFacingAngle(double angle)
{
    facingAngle = angle;
}
void RA2UnitGraphic::SetUnitSprites(wstring spriteRootFolder)
{
    spriteRootFolderName = spriteRootFolder;
    if (stand_Anims.find(spriteRootFolder) != stand_Anims.end())
        return;

    auto birth_AnimRoot = spriteRootFolder + L"/birth";
    auto die_AnimRoot = spriteRootFolder + L"/die";
    auto idle_AnimRoot = spriteRootFolder + L"/idle";
    auto attack_AnimRoot = spriteRootFolder + L"/attack";
    auto attack_fragile_AnimRoot = spriteRootFolder + L"/attack_fragile";
    auto move_AnimRoot = spriteRootFolder + L"/move";
    auto move_fragile_AnimRoot = spriteRootFolder + L"/move_fragile";
    auto stand_AnimRoot = spriteRootFolder + L"/stand";
    auto stand_fragile_AnimRoot = spriteRootFolder + L"/stand_fragile";

    birth_Anims[spriteRootFolder] = LoadAnimation(birth_AnimRoot);
    die_Anims[spriteRootFolder] = LoadVariableAnims(die_AnimRoot);
    idle_Anims[spriteRootFolder] = LoadVariableAnims(idle_AnimRoot);
    attack_Anims[spriteRootFolder] = LoadDirectionalAnims(attack_AnimRoot);
    attack_fragile_Anims[spriteRootFolder] = LoadDirectionalAnims(attack_fragile_AnimRoot);
    move_Anims[spriteRootFolder] = LoadDirectionalAnims(move_AnimRoot);
    move_fragile_Anims[spriteRootFolder] = LoadDirectionalAnims(move_fragile_AnimRoot);
    stand_Anims[spriteRootFolder] = LoadDirectionalAnims(stand_AnimRoot);
    stand_fragile_Anims[spriteRootFolder] = LoadDirectionalAnims(stand_fragile_AnimRoot);
}
void RA2UnitGraphic::PlayBirth() { SetIsRepeating(false); PlayAnim(birth_Anims); }
void RA2UnitGraphic::PlayDie() { SetIsRepeating(false); PlayRandomAnim(die_Anims); }
void RA2UnitGraphic::PlayIdle() { SetIsRepeating(false); PlayRandomAnim(idle_Anims); }
void RA2UnitGraphic::PlayAttack() { SetIsRepeating(false); PlayDirectionalAnim(attack_Anims); }
void RA2UnitGraphic::PlayAttack_fragile() { SetIsRepeating(false); PlayDirectionalAnim(attack_fragile_Anims); }
void RA2UnitGraphic::PlayMove() { SetIsRepeating(true); PlayDirectionalAnim(move_Anims); }
void RA2UnitGraphic::PlayMove_fragile() { SetIsRepeating(true); PlayDirectionalAnim(move_fragile_Anims); }
void RA2UnitGraphic::PlayStand() { SetIsRepeating(true); PlayDirectionalAnim(stand_Anims); }
void RA2UnitGraphic::PlayStand_fragile() { SetIsRepeating(true); PlayDirectionalAnim(stand_fragile_Anims); }

vector<const SpriteAnim*> RA2UnitGraphic::LoadDirectionalAnims(wstring root)
{
    vector<const SpriteAnim*> ret;
    ret.push_back(LoadAnimation(root + L"/deg000"));
    ret.push_back(LoadAnimation(root + L"/deg045"));
    ret.push_back(LoadAnimation(root + L"/deg090"));
    ret.push_back(LoadAnimation(root + L"/deg135"));
    ret.push_back(LoadAnimation(root + L"/deg180"));
    ret.push_back(LoadAnimation(root + L"/deg225"));
    ret.push_back(LoadAnimation(root + L"/deg270"));
    ret.push_back(LoadAnimation(root + L"/deg315"));
    return ret;
}
vector<const SpriteAnim*> RA2UnitGraphic::LoadVariableAnims(wstring root)
{
    vector<const SpriteAnim*> ret;

    HANDLE dir;
    WIN32_FIND_DATA file_data;

    if ((dir = FindFirstFile((root + L"/*").c_str(), &file_data)) == INVALID_HANDLE_VALUE)
        return vector<const SpriteAnim*>(); /* No files found */

    do {
        const wstring folder = file_data.cFileName;
        const wstring full_file_name = root + L"/" + folder;
        const bool is_directory = (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

        if (folder[0] == '.')
            continue;

        if (!is_directory)
            continue;

        ret.push_back(LoadAnimation(full_file_name));
    } while (FindNextFile(dir, &file_data));

    FindClose(dir);
    return ret;
}
void RA2UnitGraphic::PlayAnim(unordered_map<wstring, const SpriteAnim*> animMap)
{
    SetAnimation(animMap[spriteRootFolderName]);
    Play();
}
void RA2UnitGraphic::PlayRandomAnim(unordered_map<wstring, vector<const SpriteAnim*>>& animMap)
{
    auto& anims = animMap[spriteRootFolderName];
    SetAnimation(anims[YunutyMath::Random::GetRandomInt(0, anims.size() - 1)]);
    Play();
}
void RA2UnitGraphic::PlayDirectionalAnim(unordered_map<wstring, vector<const SpriteAnim*>>& animMap)
{
    auto& anims = animMap[spriteRootFolderName];
    if (anims.size() < 8)
        return;

    if (GetAnimSprites() == animMap[spriteRootFolderName][(int((facingAngle + 22.5 + 360) / 45) % 8)] && GetIsRepeating())
        return;

    SetAnimation(animMap[spriteRootFolderName][(int((facingAngle + 22.5 + 360) / 45) % 8)]);
    Play();
}
