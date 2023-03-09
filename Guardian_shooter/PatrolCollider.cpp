#include "PatrolCollider.h"
#include "Shooter2.h"
#include "DebugObject.h"

GameObject* PatrolCollider::CreatePatrolbox()
{
	auto patrolBox = Scene::getCurrentScene()->AddGameObject();
	patrolBox->AddComponent<BoxCollider2D>();
	patrolBox->GetComponent<BoxCollider2D>()->SetHeight(100);
	patrolBox->GetComponent<BoxCollider2D>()->SetWidth(100);
	//patrolBox->AddComponent<PatrolCollider>();


	DebugObject::CreateDebugRectImage(patrolBox, 100, 100, D2D1::ColorF::Green);

	return patrolBox;
}

void PatrolCollider::OnCollisionEnter2D(const Collision2D& collision)
{
	auto shooter2 = collision.m_OtherCollider->GetGameObject()->GetComponent<Shooter2>();
	if (shooter2 != nullptr)
	{
		shooter2->SetDirection();
	}
}
