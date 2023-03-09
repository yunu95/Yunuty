#include "Guardian_shooter.h"
#include "EnemyMarker.h"

using namespace gs_map;

EnemyMarker* EnemyMarker::selectedEnemyMarker = nullptr;
vector<EnemyMarker*> EnemyMarker::enemyMarkers;

EnemyMarker::EnemyMarker()
{
    this->markerIndex = enemyMarkers.size(); enemyMarkers.push_back(this);
}
EnemyMarker::~EnemyMarker()
{
    size_t size = enemyMarkers.size();
    if (size == 0 || this->markerIndex < 0)
    {
        return;
    }
    for (int i = this->markerIndex; i < size - 1; i++)
    {
        enemyMarkers[i] = enemyMarkers[i + 1];
        enemyMarkers[i]->markerIndex--;
    }
    enemyMarkers.resize(size - 1);
    if (enemyWave)
        enemyWave->HandleMarkerDeletion(this);
}

void EnemyMarker::Save(wstring mapRoot)
{
    fstream file;
    const wstring filename = L"enemyMarkers.txt";
    file.open(mapRoot + L'\\' + filename, fstream::out | fstream::trunc);
    //if (file.fail())
    //	_wassert(_CRT_WIDE("map/맵 이름/ 폴더를 먼저 만들어야 저장이 가능합니다."), _CRT_WIDE(__FILE__), (unsigned)(__LINE__));

    for (int i = 0; i < enemyMarkers.size(); i++)
    {
        auto each = enemyMarkers[i];
        file << (int)each->enemyType << " ";
        file << each->GetTransform()->GetWorldPosition().x << ' ';
        file << each->GetTransform()->GetWorldPosition().y << ' ';
        file << i << ' ';
        file << "\n";   // 첫 줄 생략 구조 변경
    }
    file.close();
}

void EnemyMarker::Load(wstring mapRoot)
{
    fstream file;
    for (auto each : enemyMarkers)
    {
        each->markerIndex = -1;
        EnemyMarker::DeleteEnemyMarker(each);
    }
    EnemyMarker::enemyMarkers.clear();
    const wstring filename = L"enemyMarkers.txt";
    file.open(mapRoot + L'\\' + filename);
    if (file.fail())
        return;

    vector < tuple<int, double, double, int >> infos;
    string line;
    while (getline(file, line))
    {
        stringstream sstream(line);
        int enemyType;
        double x;
        double y;
        int index;
        sstream >> enemyType;
        sstream >> x;
        sstream >> y;
        sstream >> index;
        infos.push_back(tie(enemyType, x, y, index));
    }

    for (const auto& each : infos)
    {
        int enemyType;
        double x;
        double y;
        int index;
        tie(enemyType, x, y, index) = each;
        CreateEnemyMarker(Vector2d(x, y), (EnemyType)enemyType)->markerIndex = index;
    }
    file.close();
}

EnemyMarker* EnemyMarker::CreateEnemyMarker(Vector2d location, EnemyType enemyType, EnemyWave* wave)
{
    auto obj = MapTool::GetInstance()->GetEditorUnitsLayer()->AddGameObject();
    obj->GetTransform()->SetWorldPosition(location);
    auto enemyMarker = obj->AddComponent<EnemyMarker>();
    auto imageObj = Scene::getCurrentScene()->AddGameObject(obj);
    enemyMarker->markerSprite = imageObj->AddComponent<D2DAnimatedSprite>();
    obj->AddComponent<CircleCollider2D>()->SetRadius(1);
    DebugObject::CreateDebugRectImage(imageObj, 100, 100, D2D1::ColorF::Orange);
    switch (enemyType)
    {
    case EnemyType::CommonEnemy:
        enemyMarker->markerSprite->LoadAnimationFromFile(L"sprites/enemy/shooter/idle");
        enemyMarker->enemyType = enemyType;
        break;
    case EnemyType::StrongEnemy:
        enemyMarker->markerSprite->LoadAnimationFromFile(L"sprites/enemy/shooter1/idle");
        enemyMarker->enemyType = enemyType;
        break;
    case EnemyType::Shooter:
        enemyMarker->markerSprite->LoadAnimationFromFile(L"sprites/enemy/shooter2/idle");
        enemyMarker->enemyType = enemyType;
        break;
    case EnemyType::Bomber:
        enemyMarker->markerSprite->LoadAnimationFromFile(L"sprites/enemy/shooter_curve/idle");
        enemyMarker->enemyType = enemyType;
        break;
    case EnemyType::Laser_V:
        enemyMarker->markerSprite->LoadAnimationFromFile(L"sprites/enemy/laserEnemy/idle");
        enemyMarker->enemyType = enemyType;
        break;
    case EnemyType::Laser_H:
        enemyMarker->markerSprite->LoadAnimationFromFile(L"sprites/enemy/shooter3/idle");
        enemyMarker->enemyType = enemyType;
        break;
    case EnemyType::Elite:
        enemyMarker->markerSprite->LoadAnimationFromFile(L"sprites/enemy/eliteMonster/idle");
        enemyMarker->enemyType = enemyType;
        break;
    case EnemyType::Boss:
        enemyMarker->markerSprite->LoadAnimationFromFile(L"sprites/spaceship/idle");
        enemyMarker->enemyType = enemyType;
        break;
    default:
        break;
    }

#ifdef _MAPTOOL
    auto dragBtn = Button::AddDraggableButton(obj, 50);
    dragBtn->onClick = [=]()
    {
        if (MapTool::GetInstance()->GetBrushLayer() != MapTool::BrushLayer::Unit)
            return;
        EnemyMarker::selectedEnemyMarker = enemyMarker;
        if (D2DInput::isKeyDown(KeyCode::Alt) && EnemyWave::selectedWave)
            EnemyWave::selectedWave->ToggleMarkerEssential(EnemyMarker::selectedEnemyMarker);
    };
    dragBtn->onDrag = [=]()
    {
        if (MapTool::GetInstance()->GetBrushLayer() != MapTool::BrushLayer::Unit)
            return;
        dragBtn->GetTransform()->SetWorldPosition(D2DInput::getMouseWorldPosition());
    };
#endif
    if (wave)
        wave->InsertMarker(enemyMarker);

    return enemyMarker;
}

void EnemyMarker::Update()
{

}
void EnemyMarker::OnCollisionEnter2D(const Collision2D& collision)
{
    // 맵툴 상태가 아니며, 접촉시 소환 플래그가 켜져 있으며, 이미 소환된 경우가 아닌 경우 적 개체를 현재 위치에 소환한다.
    // 그리고 충돌 대상이 카메라라는 것도 있어야 한다.
    if (!MapTool::GetInstance()->isEditingMap() && summonOnTouch && !alreadySummoned && !enemyWave && collision.m_OtherCollider == GlobalReference::cameraRectCollider)
    {
        Invoke();
    }
}

void EnemyMarker::OnCollisionStay2D(const Collision2D& collision)
{
    // 시작부터 카메라 위치에 Marker가 있는 경우
    // 굳이 해당 함수를 작성하지 않고 카메라 밖에서 유닛을 배치하거나,
    // 시작 화면 내부에 위치할 Enemy Unit 만 하드코딩으로 배치하는 등의 방법을 사용하는 것이 좋아 보임
    if (!MapTool::GetInstance()->isEditingMap() && summonOnTouch && !alreadySummoned && !enemyWave && collision.m_OtherCollider == GlobalReference::cameraRectCollider)
    {
        Invoke();
    }
}

// opacity가 0이하면 완전히 사라진다.
void EnemyMarker::SetOpacity(double opacity)
{
    if (opacity <= 0)
    {
        GetGameObject()->SetSelfActive(false);
    }
    else
    {
        if (!GetGameObject()->GetSelfActive())
            DebugObject::CreatePopUpCircle(GetTransform()->GetWorldPosition(), 70, 0.4, D2D1::ColorF::Red);
        GetGameObject()->SetSelfActive(true);
        if (markerSprite)
            markerSprite->color.a = opacity;
    }
}

void EnemyMarker::Invoke()
{
    alreadySummoned = true;
    GameObject* gameObj = nullptr;
    switch (enemyType)
    {
    case EnemyType::CommonEnemy:
        gameObj = ShooterOneShot::CreateShooterOneShot(GetTransform()->GetWorldPosition());
        break;
    case EnemyType::StrongEnemy:
        gameObj = Shooter1::CreateShooter1(GetTransform()->GetWorldPosition());
        break;
    case EnemyType::Shooter:
        gameObj = Shooter2::CreateShooter2(GetTransform()->GetWorldPosition());
        break;
    case EnemyType::Bomber:
        gameObj = Bomber::CreateBomber(GetTransform()->GetWorldPosition());
        break;
    case EnemyType::Laser_V:
        gameObj = SpaceShip::CreateSpaceShip(GetTransform()->GetWorldPosition());
        break;
    case EnemyType::Laser_H:
        gameObj = Shooter3::CreateShooter3(GetTransform()->GetWorldPosition());
        break;
    case EnemyType::Elite:
        gameObj = EliteMonster::CreateEliteMonster(GetTransform()->GetWorldPosition());
        break;
    case EnemyType::Boss:
        SoundSystem::StopMusic();
        SoundSystem::PlayMusic("sounds/boss/Boss_main.mp3");
        gameObj = Boss::CreateBoss(GetTransform()->GetWorldPosition());
        break;
    default:
        break;
    }
    DebugObject::CreatePopUpCircle(GetTransform()->GetWorldPosition());
    auto threatComp = gameObj->GetComponent<Threat>();
    gameObj->SetParent(MapTool::GetInstance()->GetEnemyLayer());
    threatComp->birthMother = this;
    threatComp->belongingWave = this->enemyWave;
}
void EnemyMarker::OnRestart()
{
    alreadySummoned = false;
    markerSprite->GetGameObject()->SetSelfActive(false);
    if (enemyType == EnemyType::Boss)
    {
        auto gameObj = Boss::CreateBoss(GetTransform()->GetWorldPosition());
        gameObj->SetSelfActive(false);
    }
}

void EnemyMarker::OnEngagingMapEditMode()
{
    markerSprite->GetGameObject()->SetSelfActive(true);
}
void EnemyMarker::OnDisengagingMapEditMode()
{
    markerSprite->GetGameObject()->SetSelfActive(false);
}

void EnemyMarker::DeleteEnemyMarker(EnemyMarker* marker)
{
    Scene::getCurrentScene()->DestroyGameObject(marker->GetGameObject());
    if (selectedEnemyMarker == marker)
        selectedEnemyMarker = nullptr;
}

ifstream EnemyMarker::fin = ifstream();
ofstream EnemyMarker::fout = ofstream();

