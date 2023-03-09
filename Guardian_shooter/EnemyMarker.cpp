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
    //	_wassert(_CRT_WIDE("map/�� �̸�/ ������ ���� ������ ������ �����մϴ�."), _CRT_WIDE(__FILE__), (unsigned)(__LINE__));

    for (int i = 0; i < enemyMarkers.size(); i++)
    {
        auto each = enemyMarkers[i];
        file << (int)each->enemyType << " ";
        file << each->GetTransform()->GetWorldPosition().x << ' ';
        file << each->GetTransform()->GetWorldPosition().y << ' ';
        file << i << ' ';
        file << "\n";   // ù �� ���� ���� ����
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
    // ���� ���°� �ƴϸ�, ���˽� ��ȯ �÷��װ� ���� ������, �̹� ��ȯ�� ��찡 �ƴ� ��� �� ��ü�� ���� ��ġ�� ��ȯ�Ѵ�.
    // �׸��� �浹 ����� ī�޶��� �͵� �־�� �Ѵ�.
    if (!MapTool::GetInstance()->isEditingMap() && summonOnTouch && !alreadySummoned && !enemyWave && collision.m_OtherCollider == GlobalReference::cameraRectCollider)
    {
        Invoke();
    }
}

void EnemyMarker::OnCollisionStay2D(const Collision2D& collision)
{
    // ���ۺ��� ī�޶� ��ġ�� Marker�� �ִ� ���
    // ���� �ش� �Լ��� �ۼ����� �ʰ� ī�޶� �ۿ��� ������ ��ġ�ϰų�,
    // ���� ȭ�� ���ο� ��ġ�� Enemy Unit �� �ϵ��ڵ����� ��ġ�ϴ� ���� ����� ����ϴ� ���� ���� ����
    if (!MapTool::GetInstance()->isEditingMap() && summonOnTouch && !alreadySummoned && !enemyWave && collision.m_OtherCollider == GlobalReference::cameraRectCollider)
    {
        Invoke();
    }
}

// opacity�� 0���ϸ� ������ �������.
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

