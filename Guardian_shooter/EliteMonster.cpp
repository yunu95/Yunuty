#include "EliteMonster.h"
#include "DebugObject.h"
#include "Player.h"
#include "Ground.h"
#include "ProjectilePool.h"
#include "ThreatPool.h"
#include "DamageEffect.h"

void EliteMonster::Initialize(D2DAnimatedSprite* animatedSprite)
{
    anim = animatedSprite;
}

GameObject* EliteMonster::CreateEliteMonster(Vector3d pos)
{
    // ThreatPool 없으면 생성.
    if (ThreatPool::GetInstance() == nullptr)
    {
        GameObject* threatPool = Scene::getCurrentScene()->AddGameObject();
        threatPool->AddComponent<ThreatPool>();
    }

    auto eliteMonster = Scene::getCurrentScene()->AddGameObject();
    eliteMonster->AddComponent<BoxCollider2D>();
    eliteMonster->GetComponent<BoxCollider2D>()->SetWidth(130);
    eliteMonster->GetComponent<BoxCollider2D>()->SetHeight(300);
    //eliteMonster->GetComponent<BoxCollider2D>()->GetTransform();
    //eliteMonster->AddComponent<EliteMonster>();
    DebugObject::CreateColliderImage(eliteMonster->GetComponent<BoxCollider2D>());
    auto eliteMonsterComp = eliteMonster->AddComponent<EliteMonster>();
    eliteMonster->GetTransform()->SetWorldPosition(pos);

    GameObject* shooterImage = Scene::getCurrentScene()->AddGameObject(eliteMonster);
    auto anim1 = shooterImage->AddComponent<D2DAnimatedSprite>();
    shooterImage->GetTransform()->position = Vector3d(0, 50, 0);
    eliteMonster->GetComponent<EliteMonster>()->Initialize(anim1);
    anim1->SetHeight(360);
    anim1->SetWidth(370);

    GameObject* effect = DamageEffect::CreateDamageEffect(Vector3d::zero);
    effect->SetParent(eliteMonster);
    effect->SetSelfActive(false);
    effect->GetComponent<D2DAnimatedSprite>()->SetHeight(550);
    effect->GetComponent<D2DAnimatedSprite>()->SetWidth(250);

    effect->GetTransform()->position = Vector3d(0, 30, 0);
    eliteMonsterComp->effect = effect->GetComponent<DamageEffect>();

    return eliteMonster;
}

void EliteMonster::Start()
{
    // 초기화 구문
    /*instance = this;*/
    hp = 20;
    damage = 3.f;
    gravity = 100;
    bulletCnt = 0;
    canAttack = false;
    attackHorable = false;
    attackVerable = false;
    justLanded = false;
    justDamaged = false;
    justFallen = false;
    stateJustChanged = true;
    isSummoned = false;

    state = EliteState::FALLING;

    traceTimer.duration = 1.2;		// 이동으로 전환되는 시간
    attackDelay.duration = 0.3;
    attackDelay2.duration = 1.2;
    attackDelay2.onExpiration = [this]()
    {
        attackFinished = true;
    };
    interrupted.duration = 0.9;

    attackCoolDownTimer.duration = 2.5;
    attackCoolDownTimer.onExpiration = [this]()
    {
        attackFinished = true;
        isBombShooted = false;
    };

    changeIdle.duration = 0.55;
    changeIdle.onExpiration = [this]()
    {
        attackFinished = true;
    };

    animTimer.duration = 0.35;
    animTimer.onExpiration = [this]()
    {
        ThreatPool::GetInstance()->Retrieve(this);
    };

    damageEffect.duration = 0.4;
    damageEffect.onExpiration = [this]()
    {
        effect->GetGameObject()->SetSelfActive(false);
    };

    horizontalView = 1;

    decidePattern(attackPattern);
}

void EliteMonster::changeState(EliteState changedState)
{
    state = changedState;
    stateJustChanged = true;
}

void EliteMonster::Update()
{
    // 업데이트 구문
    traceTimer.Update();
    attackDelay.Update();
    attackDelay2.Update();
    interrupted.Update();
    attackCoolDownTimer.Update();
    changeIdle.Update();
    damageEffect.Update();

    // 적군 위치
    GetTransform()->SetWorldPosition(
        GetTransform()->GetWorldPosition()
        + speed * Time::GetDeltaTime());

    //GetTransform()->scale.x = horizontalView;
    //GetTransform()->scale.x = 1;

    // 추적하고자 하는 플레이어의 위치
    Vector3d playerPos = Player::GetPlayerPos();

    // 수평 공격 가능 범위
    if (abs(playerPos.x - GetTransform()->GetWorldPosition().x) < randHorRange &&
        abs(playerPos.y - GetTransform()->GetWorldPosition().y) < 300)
    {
        attackHorable = true;
        attackVerable = false;		// 중복 사격 방지
    }
    else
    {
        attackHorable = false;
    }

    // 수직 공격 가능 범위
    if (abs(playerPos.x - GetTransform()->GetWorldPosition().x) < randVerRange &&
        playerPos.y - GetTransform()->GetWorldPosition().y > 100)
    {
        attackVerable = true;
        attackHorable = false;		// 중복 사격 방지
    }
    else
    {
        attackVerable = false;
    }

    if (attackHorable || attackVerable)
        canAttack = true;
    else
        canAttack = false;


    /// <summary>
/// 객체의 상태가 다른 상태로 바뀔 수 있는지를 먼저 따진다.
/// </summary>\

    switch (state)
    {
    case EliteState::IDLE:
        if (canAttack == false && traceTimer.isActive == false)
        {
            changeState(EliteState::WALKING);
        }
        if (canAttack == true && traceTimer.isActive == false)
        {
            if (attackPattern == 0)
                changeState(EliteState::ATTACK);
            if (attackPattern == 1)
                changeState(EliteState::ATTACK2);
        }
        break;

    case EliteState::FALLING:
        if (justLanded == true)
        {
            changeState(EliteState::IDLE);
            justLanded = false;
        }
        break;
    case EliteState::WALKING:
        if (canAttack == true)
        {
            changeState(EliteState::IDLE);
        }
        if (justFallen == true)
        {
            changeState(EliteState::FALLING);
            justFallen = false;
        }
        break;
    case EliteState::ATTACK:
        if (attackFinished)
        {
            changeState(EliteState::IDLE);
        }
        break;
    case EliteState::ATTACK2:
        if (attackFinished)
        {
            changeState(EliteState::IDLE);
        }
        break;
    case EliteState::DEATH:
        break;
    default:
        break;
    }
    /// <summary>
    /// 각 상태에 따른 행동은 여기서 진행합니다.
    /// </summary>
    switch (state)
    {
    case EliteState::IDLE:
        if (stateJustChanged)
        {
            anim->LoadAnimationFromFile(L"sprites/enemy/eliteMonster/idle");
            speed.x = 0;
            speed.y = 0;
            traceTimer.Start();
            if (playerPos.x > GetTransform()->GetWorldPosition().x)
            {
                GetTransform()->scale.x = 1;
            }
            else if (playerPos.x < GetTransform()->GetWorldPosition().x)
            {
                GetTransform()->scale.x = -1;
            }
            decidePattern(attackPattern);
            bulletCnt = 0;
        }
        break;
    case EliteState::FALLING:
        speed += Vector3d(0, -gravity, 0) * 10 * Time::GetDeltaTime();

        if (stateJustChanged)
        {
            anim->LoadAnimationFromFile(L"sprites/enemy/eliteMonster/fall");
            speed.x = 0;
        }
        break;
    case EliteState::WALKING:
        if (stateJustChanged)
        {
            if (playerPos.x > this->GetTransform()->GetWorldPosition().x)
                speed.x = 400;
            if (playerPos.x < this->GetTransform()->GetWorldPosition().x)
                speed.x = -400;
            anim->LoadAnimationFromFile(L"sprites/enemy/eliteMonster/move");
            speed.y = 0;
            traceTimer.Start();
            randHorRange = rand() % 500 + 500;
            randVerRange = rand() % 20 + 40;
        }
        break;
    case EliteState::ATTACK:
        if (stateJustChanged)
        {
            if (attackHorable)
                anim->LoadAnimationFromFile(L"sprites/enemy/eliteMonster/attack/forward");
            else if (attackVerable)
                anim->LoadAnimationFromFile(L"sprites/enemy/eliteMonster/attack/up");
            speed.x = 0;
            speed.y = 0;
            attackFinished = false;
            bulletCnt = 0;
            if (playerPos.x > GetTransform()->GetWorldPosition().x)
                GetTransform()->scale.x = 1;
            if (playerPos.x < GetTransform()->GetWorldPosition().x)
                GetTransform()->scale.x = -1;
        }

        if (!attackDelay.isActive && !interrupted.isActive && bulletCnt < 3)
        {
            if (attackHorable)
            {
                if (playerPos.x > GetTransform()->GetWorldPosition().x)
                {
                    // 오른쪽 사격
                    ProjectilePool::GetInstance()->SummonProjectile(GetTransform()->GetWorldPosition() + Vector3d(130, 9, 0)
                        , Vector3d(500, 0, 0), ProjectileType::BULLET, damage, false);
                    GetTransform()->scale.x = 1;
                }
                else
                {
                    // 왼쪽 사격
                    ProjectilePool::GetInstance()->SummonProjectile(GetTransform()->GetWorldPosition() + Vector3d(-130, 9, 0)
                        , Vector3d(-500, 0, 0), ProjectileType::BULLET, damage, false);
                    GetTransform()->scale.x = -1;
                }
            }

            if (attackVerable)
            {
                if (playerPos.y > GetTransform()->GetWorldPosition().y)
                {
                    // 위로 사격
                    if (playerPos.x > GetTransform()->GetWorldPosition().x)
                    {
                        ProjectilePool::GetInstance()->SummonProjectile(GetTransform()->GetWorldPosition() + Vector3d(40, 50, 0)
                            , Vector3d(0, 500, 0), ProjectileType::BULLET, damage, false);
                    }
                    else
                    {
                        ProjectilePool::GetInstance()->SummonProjectile(GetTransform()->GetWorldPosition() + Vector3d(-40, 50, 0)
                            , Vector3d(0, 500, 0), ProjectileType::BULLET, damage, false);
                    }
                }

            }
            SoundSystem::PlaySoundfile("sounds/enemy/Enemy_tripleshoot.wav");
            bulletCnt++;
            attackDelay.Start();
            if (bulletCnt == 3)
            {
                attackDelay2.Start();
                anim->LoadAnimationFromFile(L"sprites/enemy/eliteMonster/idle");
            }
        }
        break;

    case EliteState::ATTACK2:
        if (stateJustChanged)
        {
            anim->LoadAnimationFromFile(L"sprites/enemy/eliteMonster/attack/bomb");
            speed.x = 0;
            speed.y = 0;
            attackCoolDownTimer.Start();
            attackFinished = false;
            isAttackReady = true;
            changeIdle.Start();
        }
        if (isAttackReady)
        {
            isAttackReady = false;
            isBombShooted = true;
            // 풀에서 폭탄 소환
            Vector3d pos = GetTransform()->GetWorldPosition();
            Vector3d shootPosR = Vector3d(pos.x + horizontalView * 45, pos.y + 165, 0);
            Vector3d shootPosL = Vector3d(pos.x - horizontalView * 45, pos.y + 165, 0);
            // 몬스터와 플레이어 사이의 거리
            double width = Player::GetPlayerPos().x - GetTransform()->GetWorldPosition().x;
            double height = Player::GetPlayerPos().y - GetTransform()->GetWorldPosition().y;
            double distance;
            if (horizontalView == -1)
                distance = width - height;
            else
                distance = width + height;
            // 폭탄 던지는 각도 45도로 고정
            // 폭탄 쏠 때의 힘의 제곱은 distance * gravity / sin(2*theta)
            double force;
            if (distance < 0)
            {
                if (horizontalView == -1)
                    force = -sqrt(abs(distance) * gravity);
                else
                    force = sqrt(abs(distance) * gravity);
            }
            else
            {
                if (horizontalView == -1)
                    force = -sqrt(abs(distance) * gravity);
                else
                    force = sqrt(abs(distance) * gravity);
            }
            if (playerPos.x > GetTransform()->GetWorldPosition().x)
            {
                ProjectilePool::GetInstance()->SummonProjectile(shootPosR, Vector3d(force * cos(45.0) * 2.85, abs(force * sin(45.0)) * 2.85, 0), ProjectileType::BOMB, damage, false);
                ProjectilePool::GetInstance()->SummonProjectile(shootPosR, Vector3d(force * cos(25.0) * 2.85, abs(force * sin(25.0)) * 2.85, 0), ProjectileType::BOMB, damage, false);
                ProjectilePool::GetInstance()->SummonProjectile(shootPosR, Vector3d(force * cos(50.0) * 2.85, abs(force * sin(60.0)) * 2.85, 0), ProjectileType::BOMB, damage, false);
            }
            else
            {
                ProjectilePool::GetInstance()->SummonProjectile(shootPosL, -1 * Vector3d(force * cos(45.0) * 2.85, abs(force * sin(45.0)) * 2.85, 0), ProjectileType::BOMB, damage, false);
                ProjectilePool::GetInstance()->SummonProjectile(shootPosL, -1 * Vector3d(force * cos(25.0) * 2.85, abs(force * sin(25.0)) * 2.85, 0), ProjectileType::BOMB, damage, false);
                ProjectilePool::GetInstance()->SummonProjectile(shootPosL, -1 * Vector3d(force * cos(50.0) * 2.85, abs(force * sin(60.0)) * 2.85, 0), ProjectileType::BOMB, damage, false);
            }
        }
        break;
    case EliteState::DEATH:
        animTimer.Update();
        if (stateJustChanged)
        {
            speed.x = 0;
            speed.y = 0;
            anim->LoadAnimationFromFile(L"sprites/enemy/eliteMonster/death");
            SoundSystem::PlaySoundfile("sounds/enemy/Enemy_dyingexplode.wav");
            animTimer.Start();
        }
        break;
    default:
        break;
    }

    if (isSummoned)
    {
        stateJustChanged = true;
        isSummoned = false;
    }
    else
    {
        stateJustChanged = false;
    }
}


void EliteMonster::OnCollisionEnter2D(const Collision2D& collision)
{
    auto box = collision.m_OtherCollider->GetGameObject()->GetComponent<Ground>();
    if (box != nullptr)
    {
        justLanded = true;
    }
}

void EliteMonster::OnCollisionExit2D(const Collision2D& collision)
{
    auto box = collision.m_OtherCollider->GetGameObject()->GetComponent<Ground>();
    if (box != nullptr)
    {
        justFallen = true;
    }
}

void EliteMonster::Damage(double damage)
{
    damageEffect.Start();
    Threat::Damage(damage);
    if (effect)
        effect->GetGameObject()->SetSelfActive(true);
}

void EliteMonster::OnDeath()
{
    changeState(EliteState::DEATH);
}

int EliteMonster::decidePattern(int& pattern)
{
    pattern = rand() % 2;
    return pattern;
}



