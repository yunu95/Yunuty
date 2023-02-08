#include "BulletPool.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Player.h"
#include "BulletPool.h"
#include "GlobalReferences.h"

void Bullet::Update()
{
	GetTransform()->SetWorldPosition(GetTransform()->GetWorldPosition() + speed * Time::GetDeltaTime());
}

void Bullet::OnCollisionEnter2D(const Collision2D& collision)
{
	auto enemy = collision.m_OtherCollider->GetGameObject()->GetComponent<Enemy>();
	if (enemy != nullptr && isPlayerBullet == true)
	{
		enemy->Damage();
		BulletPool::instance->Retrieve(this);
	}

	auto player = collision.m_OtherCollider->GetGameObject()->GetComponent<Player>();
	if (player != nullptr && isPlayerBullet == false)
	{
		player->Damage(damage);
		BulletPool::instance->Retrieve(this);
	}


}


void Bullet::OnCollisionExit2D(const Collision2D& collision)
{
	if (collision.m_OtherCollider == GlobalReference::cameraRectCollider)
	{
		BulletPool::instance->Retrieve(this);
	}
}

void CreateBullet(Scene* scene, Vector3d speed)
{
	auto bullet = scene->AddGameObject();
	bullet->AddComponent<D2DRectangle>();
	bullet->GetComponent<D2DRectangle>()->color = D2D1::ColorF::Gray;
	bullet->GetComponent<D2DRectangle>()->filled = true;
	bullet->GetComponent<D2DRectangle>()->height = 20;
	bullet->GetComponent<D2DRectangle>()->width = 40;
	bullet->AddComponent<BoxCollider2D>()->SetHeight(20);
	bullet->GetComponent<BoxCollider2D>()->SetWidth(40);
	bullet->AddComponent<Bullet>()->speed = speed;
	BulletPool::instance->Retrieve(bullet->GetComponent<Bullet>());
}




