#include "Laser.h"
#include "GameObject.h"
#include "Player.h"
#include "DebugObject.h"


GameObject* Laser::CreateLaser(Vector3d pos)
{
	auto laser = Scene::getCurrentScene()->AddGameObject();
	laser->AddComponent<BoxCollider2D>();
	laser->GetComponent<BoxCollider2D>()->SetWidth(50);
	laser->GetComponent<BoxCollider2D>()->SetHeight(5000);
	DebugObject::CreateColliderImage(laser->GetComponent<BoxCollider2D>());
	laser->AddComponent<Laser>();

	laser->GetTransform()->SetWorldPosition(pos);
	auto anim2 = laser->AddComponent<D2DAnimatedSprite>();
	anim2->SetHeight(5000);
	anim2->SetWidth(100);
	laser->GetComponent<Laser>()->Initialize(anim2);
	laser->SetSelfActive(false);

	return laser;
}

void Laser::Initialize(D2DAnimatedSprite* upperAnimatedSprite)
{
	this->animatedSprite = upperAnimatedSprite;
	upperAnimatedSprite->LoadAnimationFromFile(L"sprites/projectile/laser/forward");
}
void Laser::Start()
{
	GetTransform()->scale.x = -1;
}
void Laser::Update()
{

}

void Laser::OnCollisionEnter2D(const Collision2D& collision)
{
	//auto player = collision.m_OtherCollider->GetGameObject()->GetComponent<Player>();
	//if (player != nullptr)
	//{
	//	player->Damage(SpaceShip::damage);
	//}
}

void Laser::OnCollisionStay2D(const Collision2D& collision)
{
	auto player = collision.m_OtherCollider->GetGameObject()->GetComponent<Player>();
	if (player != nullptr)
	{
		player->Damage();
	}
}

