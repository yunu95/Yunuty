#include "DamageEffect.h"

void DamageEffect::Initialize(D2DAnimatedSprite* animatedSprite)
{
	anim = animatedSprite;
}

void DamageEffect::Start()
{
	anim->LoadAnimationFromFile(L"sprites/obstacle/spark");
}

void DamageEffect::Update()
{
}

GameObject* DamageEffect::CreateDamageEffect(Vector3d pos)
{
	auto effect = Scene::getCurrentScene()->AddGameObject();
	effect->AddComponent<DamageEffect>();

	auto anim1 = effect->AddComponent<D2DAnimatedSprite>();
	anim1->SetHeight(256);
	anim1->SetWidth(128);

	effect->GetComponent<DamageEffect>()->Initialize(anim1);
	effect->SetSelfActive(false);

	return effect;
}
