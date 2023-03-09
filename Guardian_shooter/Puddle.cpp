#include "Puddle.h"
#include "DebugObject.h"
#include "Player.h"


void Puddle::Start()
{
	damage = 1.f;
}

void Puddle::Initialize(D2DAnimatedSprite* upperAnimatedSprite, D2DAnimatedSprite* lowerAnimatedSprite)
{
	this->upperAnimatedSprite = upperAnimatedSprite;
	upperAnimatedSprite->LoadAnimationFromFile(L"sprites/obstacle/spark");
	this->lowerAnimatedSprite = lowerAnimatedSprite;
	lowerAnimatedSprite->LoadAnimationFromFile(L"sprites/obstacle/puddle");
}

GameObject* Puddle::CreatePuddle(Vector3d pos)
{
	auto puddle = Scene::getCurrentScene()->AddGameObject();

	puddle->AddComponent<BoxCollider2D>();
	puddle->GetComponent<BoxCollider2D>()->SetWidth(2300);
	puddle->GetComponent<BoxCollider2D>()->SetHeight(10);

	puddle->AddComponent<Puddle>();

	puddle->GetTransform()->SetWorldPosition(pos);
	auto anim1 = puddle->AddComponent<D2DAnimatedSprite>();
	anim1->SetHeight(128);
	anim1->SetWidth(128);
	auto anim2 = puddle->AddComponent<D2DAnimatedSprite>();
	anim2->SetHeight(256);
	anim2->SetWidth(128);
	puddle->GetComponent<Puddle>()->Initialize(anim1, anim2);

	return puddle;
}

void Puddle::Update()
{

}

void Puddle::OnCollisionEnter2D(const Collision2D& collision)
{

}

void Puddle::OnCollisionStay2D(const Collision2D& collision)
{
	auto player = collision.m_OtherCollider->GetGameObject()->GetComponent<Player>();
	if (player != nullptr)
	{
		player->Damage();
	}
}