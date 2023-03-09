#include "Guardian_shooter.h"
#include "Threat.h"
#include "DebugObject.h"

unordered_set<Threat*> Threat::threats;

Threat::Threat()
{
    threats.insert(this);
};
Threat::~Threat()
{
    threats.erase(this);
};

void Threat::RemoveAllThreats()
{
    for (auto each : threats)
    {
        Scene::getCurrentScene()->DestroyGameObject(each->GetGameObject());
    }
}

void Threat::Damage(double damage)
{
	SoundSystem::PlaySoundfile("sounds/enemy/Enemy_damaged.wav");
    if (invincible)
        return;
    // 체력 깎기
    hp -= damage;
    if (!Player::GetInstance()->isDroneSummoned)
    {
		Player::GetInstance()->AddCombo(1);
		Player::GetInstance()->SetGauge(1);
    }

    DebugObject::CreatePopUpCircle(GetTransform()->GetWorldPosition(), 25, 0.5, D2D1::ColorF::Blue);
    // 체력 0보다 같거나 작으면 사망
    if (hp <= 0)
    {
        if (belongingWave)
            belongingWave->HandleDeathEvent(this);
        OnDeath();
        return;
    }
}
