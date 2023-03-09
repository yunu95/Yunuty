#include "Guardian_shooter.h"
#include "SpaceShip.h"
#include "Timer.h"
#include "Player.h"
#include "DebugObject.h"
#include "Laser.h"
#include "ThreatPool.h"
#include "DamageEffect.h"

void SpaceShip::Initialize(D2DAnimatedSprite* upperAnimatedSprite)
{
    this->animatedSprite = upperAnimatedSprite;
    animatedSprite->LoadAnimationFromFile(L"sprites/enemy/laserEnemy/idle");
}

GameObject* SpaceShip::CreateSpaceShip(Vector3d pos)
{
    // ThreatPool 없으면 생성.
    if (ThreatPool::GetInstance() == nullptr)
    {
        GameObject* threatPool = Scene::getCurrentScene()->AddGameObject();
        threatPool->AddComponent<ThreatPool>();
    }

    auto spaceship = Scene::getCurrentScene()->AddGameObject();
    spaceship->AddComponent<BoxCollider2D>();
    spaceship->GetComponent<BoxCollider2D>()->SetWidth(100);
    spaceship->GetComponent<BoxCollider2D>()->SetHeight(50);
    DebugObject::CreateColliderImage(spaceship->GetComponent<BoxCollider2D>());
    auto spaceshipComp = spaceship->AddComponent<SpaceShip>();
    spaceship->GetTransform()->SetWorldPosition(pos);

    GameObject* laser = Laser::CreateLaser(Vector3d::zero);
    laser->SetParent(spaceship);
    laser->GetTransform()->position = Vector3d(3, -2500, 0);

    GameObject* spaceShipImage = Scene::getCurrentScene()->AddGameObject(spaceship);
    auto anim = spaceShipImage->AddComponent<D2DAnimatedSprite>();
    spaceshipComp->laser = laser->GetComponent<Laser>();
    spaceShipImage->GetTransform()->position = Vector3d(-7, 0, 0);

    //DebugObject::CreateDebugRectImage(spaceship, 100, 50);
    spaceship->GetComponent<SpaceShip>()->Initialize(anim);
    anim->SetHeight(300);
    anim->SetWidth(300);

    GameObject* effect = DamageEffect::CreateDamageEffect(Vector3d::zero);
    effect->SetParent(spaceship);
    effect->SetSelfActive(false);
    effect->GetComponent<D2DAnimatedSprite>()->SetHeight(200);
    effect->GetTransform()->position = Vector3d(7, 0, 0);

    spaceshipComp->effect = effect->GetComponent<DamageEffect>();

    return spaceship;
}

void SpaceShip::Start()
{
    //chargeStartTimer.Start();
    chargeStartTimer.duration = 2;

    attackStartTimer.duration = 0.8;
    attackStartTimer.onExpiration = [this]()
    {
        laser->GetGameObject()->SetSelfActive(true);
    };

    laserLastTimer.duration = 1.0;
    laserLastTimer.onExpiration = [this]()
    {
        laser->GetGameObject()->SetSelfActive(false);
    };

    animTimer.duration = 0.35;
    animTimer.onExpiration = [this]()
    {
        ThreatPool::GetInstance()->Retrieve(this);
    };

    damageEffect.duration = 0.7;
    damageEffect.onExpiration = [this]()
    {
        effect->GetGameObject()->SetSelfActive(false);
    };
    state = SpaceShipState::IDLE;
}

void SpaceShip::changeState(SpaceShipState tochangeState)
{
    justChanged = true;
    state = tochangeState;
}

void SpaceShip::Update()
{
    chargeStartTimer.Update();
    attackStartTimer.Update();
    laserLastTimer.Update();
    damageEffect.Update();

    switch (state)
    {
    case SpaceShipState::IDLE:
        if (!chargeStartTimer.isActive)
            changeState(SpaceShipState::CHARGE);
        break;
    case SpaceShipState::CHARGE:
        if (!attackStartTimer.isActive)
            changeState(SpaceShipState::ATTACK);
        break;
    case SpaceShipState::ATTACK:
        if (!laserLastTimer.isActive)
            changeState(SpaceShipState::IDLE);
        break;
    default:
        break;
    }

    switch (state)
    {
    case SpaceShipState::IDLE:
        if (justChanged)
        {
            // 애니메이션 적용
            // 차지 타이머 시작
            animatedSprite->LoadAnimationFromFile(L"sprites/enemy/laserEnemy/idle");
            chargeStartTimer.Start();
        }

        break;
    case SpaceShipState::CHARGE:
        if (justChanged)
        {
            // 애니메이션 적용
            // 어택 타이머 시작
            animatedSprite->LoadAnimationFromFile(L"sprites/enemy/laserEnemy/charge");
            SoundSystem::PlaySoundfile("sounds/enemy/Enemy_Lazercharge.wav");
            attackStartTimer.Start();
        }
        break;
    case SpaceShipState::ATTACK:
        if (justChanged)
        {
            // 애니메이션 적용
            // laserlast 타이머 시작
            animatedSprite->LoadAnimationFromFile(L"sprites/enemy/laserEnemy/attack");
            SoundSystem::PlaySoundfile("sounds/enemy/Enemy_Lazershoot.wav");
            laserLastTimer.Start();
        }
        break;
    case  SpaceShipState::Death:
        animTimer.Update();
        if (justChanged)
        {
            animatedSprite->LoadAnimationFromFile(L"sprites/enemy/laserEnemy/death");
            SoundSystem::PlaySoundfile("sounds/enemy/Enemy_dyingexplode.wav");
            animTimer.Start();
        }
    default:
        break;
    }

    justChanged = false;

}

void SpaceShip::Damage(double damage)
{
    damageEffect.Start();
    Threat::Damage(damage);
    effect->GetGameObject()->SetSelfActive(true);
}

void SpaceShip::OnDeath()
{
    changeState(SpaceShipState::Death);
}

void SpaceShip::OnCollisionExit2D(const Collision2D& collision)
{
    //auto cam = collision.m_OtherCollider->GetGameObject()->GetComponent<GSCamera>();
    if (collision.m_OtherCollider == GlobalReference::cameraRectCollider) ThreatPool::GetInstance()->Retrieve(this);
}
