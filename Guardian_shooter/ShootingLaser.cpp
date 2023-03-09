#include "ShootingLaser.h"
#include "GameObject.h"
#include "Player.h"
#include "DebugObject.h"

void ShootingLaser::Start()
{
	damage = 3.f;
}

GameObject* ShootingLaser::CreateLaser(Vector3d pos)
{
	auto laser = Scene::getCurrentScene()->AddGameObject();

	laser->AddComponent<BoxCollider2D>();
	laser->GetComponent<BoxCollider2D>()->SetWidth(2300);
	laser->GetComponent<BoxCollider2D>()->SetHeight(10);
	DebugObject::CreateColliderImage(laser->GetComponent<BoxCollider2D>());



	laser->AddComponent<ShootingLaser>();

	laser->GetTransform()->SetWorldPosition(pos);
	auto anim = laser->AddComponent<D2DAnimatedSprite>();
	anim->SetHeight(10);
	anim->SetWidth(5000);
	laser->GetComponent<ShootingLaser>()->Initialize(anim);
	/*anim->GetTransform()->SetWorldPosition(pos - Vector3d(10000, 0, 0));*/
	laser->SetSelfActive(false);

	return laser;
}

void ShootingLaser::Initialize(D2DAnimatedSprite* animatedSprite)
{
	this->animatedSprite = animatedSprite;
	animatedSprite->LoadAnimationFromFile(L"sprites/projectile/laser/forward");
}

void ShootingLaser::Update()
{

}

void ShootingLaser::OnCollisionEnter2D(const Collision2D& collision)
{
	//auto player = collision.m_OtherCollider->GetGameObject()->GetComponent<Player>();
	//if (player != nullptr)
	//{
	//	player->Damage(damage);
	//}
}

void ShootingLaser::OnCollisionStay2D(const Collision2D& collision)
{
	auto player = collision.m_OtherCollider->GetGameObject()->GetComponent<Player>();
	if (player != nullptr)
	{
		player->Damage();
	}
}