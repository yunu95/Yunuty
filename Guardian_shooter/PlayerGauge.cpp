#include "PlayerGauge.h"

PlayerGauge* PlayerGauge::CreateGaugeBox(Vector3d pos)
{
	auto gameObject = Scene::getCurrentScene()->AddGameObject();
	gameObject->AddComponent<D2DRectangle>();
	gameObject->GetComponent<D2DRectangle>()->color = D2D1::ColorF::Yellow;
	gameObject->GetComponent<D2DRectangle>()->filled = true;
	gameObject->GetComponent<D2DRectangle>()->height = 40;
	gameObject->GetComponent<D2DRectangle>()->width = 40;
	gameObject->GetTransform()->SetWorldPosition(pos);
	auto gaugeBox = gameObject->AddComponent<PlayerGauge>();
	//BulletPool::instance->Retrieve(gaugeBox->GetComponent<PlayerGauge>());

	return gaugeBox;
}
