#include "Guardian_shooter.h"
//#define _MAPTOOL

using namespace gs_map;
using namespace UI;

MapTool* MapTool::instance = nullptr;
Button* MapTool::lastSelectedEditorButton = nullptr;


void MapTool::Start()
{
#ifdef _MAPTOOL
    LoadMap(GetLastEditedMapName());
#endif
}
MapTool::MapTool()
{
    if (!instance)
        instance = this;
}
MapTool::~MapTool()
{
    if (instance == this)
        instance = nullptr;
}
void MapTool::Initialize()
{
    // 레이어 구별 및 전체 화면 구성은 Initialize 함수에서 다 끝낸다.
    behindBackgroundLayer = GetGameObject()->AddGameObject();
    behindBackgroundLayer = GetGameObject()->AddGameObject();
    editorBehindBackgroundLayer = GetGameObject()->AddGameObject();
    backgroundLayer = GetGameObject()->AddGameObject();
    backgroundObjectsLayer = GetGameObject()->AddGameObject();
    railLayer = GetGameObject()->AddGameObject();
    enemyLayer = GetGameObject()->AddGameObject();
    itemLayer = GetGameObject()->AddGameObject();
    projectileLayer = GetGameObject()->AddGameObject();
    playerProjectileLayer = GetGameObject()->AddGameObject();
    playerLayer = GetGameObject()->AddGameObject();
    foregroundObjectsLayer = GetGameObject()->AddGameObject();
    groundLayer = GetGameObject()->AddGameObject();
    editorBackgroundLayer = GetGameObject()->AddGameObject();
    editorUnitsLayer = GetGameObject()->AddGameObject();
    editorForegroundLayer = GetGameObject()->AddGameObject();
    foregroundLayer = GetGameObject()->AddGameObject();
    mapToolUILayer = GetGameObject()->AddGameObject();

    backgroundSprite = backgroundLayer->AddComponent<D2DSprite>();
    mapToolUILayer->AddComponent<CameraSticky>();
    projectileLayer->AddComponent<ProjectilePool>();
    Cursor::CreateCursor();
#ifdef _MAPTOOL
    // 다른 정적 변수들 초기화
    MapImage::selectedRectangle = mapToolUILayer->AddGameObject()->AddComponent<D2DRectangle>();
    MapImage::selectedRectangle->GetGameObject()->SetSelfActive(false);
    MapImage::selectedRectangle->width = 0;
    MapImage::selectedRectangle->height = 0;
    MapImage::selectedRectangle->border = 5;
    MapImage::selectedRectangle->color = D2D1::ColorF::Red;
    MapImage::selectedRectangle->filled = false;

    GameObject* backgroundOptions = mapToolUILayer->AddGameObject();
    GameObject* terrainOptions = mapToolUILayer->AddGameObject();
    GameObject* unitOptions = mapToolUILayer->AddGameObject();
    GameObject* enemyOptions = mapToolUILayer->AddGameObject();
    GameObject* itemOptions = mapToolUILayer->AddGameObject();
    GameObject* imageOptions = mapToolUILayer->AddGameObject();
    GameObject* railOptions = mapToolUILayer->AddGameObject();

    for (auto each : { backgroundOptions ,terrainOptions,unitOptions,enemyOptions,itemOptions, imageOptions,railOptions })
        each->SetSelfActive(false);

    // 에디트 레이어 탐색 버튼 구현
    {
        constexpr double x = -650;
        constexpr double y = 500;
        constexpr double dx = 200;
        constexpr double dy = 0;
        D2DText* mapNameText;
        Button* mapName = Button::CreateToggleButton(mapToolUILayer, Vector2d(x + dx * 0 - 100, y + dy * 0), L"맵 이름 : default", 400, 75, &mapNameText);
        Button* newMap = Button::CreateButton(mapToolUILayer, Vector2d(x + dx * 1, y + dy * 1), L"새 맵", 150);
        Button* save = Button::CreateButton(mapToolUILayer, Vector2d(x + dx * 2, y + dy * 2), L"저장", 150);
        Button* load = Button::CreateButton(mapToolUILayer, Vector2d(x + dx * 3, y + dy * 3), L"불러오기", 150);
        Button* restart = Button::CreateButton(mapToolUILayer, Vector2d(x + dx * 4, y + dy * 4), L"재시작", 150);
        Button* resume = Button::CreateButton(mapToolUILayer, Vector2d(x + dx * 5, y + dy * 5), L"계속", 150);

        auto mapNameTextInput = mapName->GetGameObject()->AddComponent<TextInput>();
        mapNameTextInput->typedInput = GetLastEditedMapName();
        mapName->onSelect = [=]() {mapNameTextInput->isTyping = true; };
        mapName->onDeselect = [=]() {mapNameTextInput->isTyping = false; };
        mapName->onUpdateWhileSelected = [=]() { mapNameText->text = L"맵 이름 : " + mapNameTextInput->typedInput; };


        mapNameText->text = L"맵 이름 : " + mapNameTextInput->typedInput;

        newMap->onClick = [=]()
        {
            MapObject::DeleteAllObjects();
            Threat::RemoveAllThreats();
            Item::RemoveAllItems();
            TerrainPoint::extendStartPoint = nullptr;
            RailPoint::extendStartPoint = nullptr;
            PlayerStartPoint::instance = nullptr;
            EnemyMarker::enemyMarkers.clear();
            ItemMarker::itemMarkers.clear();
        };
        save->onClick = [=]() {SaveMap(mapNameTextInput->typedInput); SaveLastEditedMapName(mapNameTextInput->typedInput); };
        load->onClick = [=]() {LoadMap(mapNameTextInput->typedInput); SaveLastEditedMapName(mapNameTextInput->typedInput); };
        restart->onClick = [=]() {Restart(); };
    }

    // 에디트 레이어 탐색 버튼 구현
    {
        constexpr double x = 700;
        constexpr double y = 400;
        constexpr double dy = -100;
        Button* backgroud = Button::CreateToggleButton(mapToolUILayer, Vector2d(x, y + dy * 0), L"배경");
        Button* terrain = Button::CreateToggleButton(mapToolUILayer, Vector2d(x, y + dy * 1), L"지형");
        Button* unit = Button::CreateToggleButton(mapToolUILayer, Vector2d(x, y + dy * 2), L"유닛");
        Button* item = Button::CreateToggleButton(mapToolUILayer, Vector2d(x, y + dy * 3), L"아이템");
        Button* images = Button::CreateToggleButton(mapToolUILayer, Vector2d(x, y + dy * 4), L"추가 이미지");
        Button* rail = Button::CreateToggleButton(mapToolUILayer, Vector2d(x, y + dy * 5), L"카메라 레일");

        for (auto each : { backgroud,terrain,unit,item,images,rail })
            each->radioSelectGroup = { backgroud,terrain,unit,item,images,rail };

        backgroud->onSelect = [=]() { brush = BrushType::None; brushLayer = BrushLayer::Background; backgroundOptions->SetSelfActive(true); lastSelectedEditorButton = backgroud; };
        terrain->onSelect = [=]() { brush = BrushType::None; brushLayer = BrushLayer::Terrain; terrainOptions->SetSelfActive(true); lastSelectedEditorButton = terrain; };
        unit->onSelect = [=]() { brush = BrushType::PlayerStartPoint; brushLayer = BrushLayer::Unit; unitOptions->SetSelfActive(true); lastSelectedEditorButton = unit; };
        item->onSelect = [=]() { brush = BrushType::None; brushLayer = BrushLayer::Item; itemOptions->SetSelfActive(true); lastSelectedEditorButton = item; };
        images->onSelect = [=]()
        {
            brush = BrushType::Image;
            brushLayer = BrushLayer::Image;
            imageOptions->SetSelfActive(true);
            editorBackgroundLayer->SetSelfActive(true);
            editorForegroundLayer->SetSelfActive(true);
            lastSelectedEditorButton = images;
        };
        rail->onSelect = [=]() { brush = BrushType::CameraRail; brushLayer = BrushLayer::Rail; railOptions->SetSelfActive(true); lastSelectedEditorButton = rail; };

        backgroud->onDeselect = [=]()
        {
            backgroundOptions->SetSelfActive(false);
            if (lastSelectedEditorButton == backgroud)
            {
                brushLayer = BrushLayer::None;
                brush = BrushType::None;
            }
        };
        terrain->onDeselect = [=]()
        {
            terrainOptions->SetSelfActive(false);
            if (lastSelectedEditorButton == terrain)
            {
                brushLayer = BrushLayer::None;
                brush = BrushType::None;
            }
        };
        unit->onDeselect = [=]()
        {
            unitOptions->SetSelfActive(false); enemyOptions->SetSelfActive(false);
            if (lastSelectedEditorButton == unit)
            {
                brushLayer = BrushLayer::None;
                brush = BrushType::None;
            }
        };
        item->onDeselect = [=]()
        {
            itemOptions->SetSelfActive(false);
            if (lastSelectedEditorButton == item)
            {
                brushLayer = BrushLayer::None;
                brush = BrushType::None;
            }
        };
        images->onDeselect = [=]()
        {
            imageOptions->SetSelfActive(false);
            editorBackgroundLayer->SetSelfActive(false);
            editorForegroundLayer->SetSelfActive(false);
            if (lastSelectedEditorButton == images)
            {
                brushLayer = BrushLayer::None;
                brush = BrushType::None;
            }
        };
        rail->onDeselect = [=]()
        {
            railOptions->SetSelfActive(false);
            if (lastSelectedEditorButton == rail)
            {
                brushLayer = BrushLayer::None;
                brush = BrushType::None;
            }
        };
    }

    constexpr double x = -800;
    constexpr double y = 350;
    // 백그라운드 배경 화면 구성
    {
        D2DText* fileNameText;
        Button* fileName = Button::CreateToggleButton(backgroundOptions, Vector2d(x + 300, y), L"백그라운드 화면 파일 경로 : ", 800, 75, &fileNameText);
        auto fileNameTextInput = fileName->GetGameObject()->AddComponent<TextInput>();
        deselectOnEmptyClick.push_back(fileName);
        fileName->onSelect = [=]() {fileNameTextInput->isTyping = true; lastSelectedEditorButton = fileName; };
        fileName->onDeselect = [=]()
        {
            fileNameTextInput->isTyping = false;
            if (lastSelectedEditorButton == fileName)
            {
                brushLayer = BrushLayer::None;
                brush = BrushType::None;
            }
        };
        fileName->onUpdateWhileSelected = [=]()
        {
            fileNameText->text = L"백그라운드 화면 파일 경로 : " + fileNameTextInput->typedInput;
            backgroundSprite->SetSpriteFilePath(fileNameTextInput->typedInput);
        };
    }
    // 지형 화면 구성
    {
        constexpr double dx = 200;
        constexpr double dy = -100;
        Button* hard = Button::CreateToggleButton(terrainOptions, Vector2d(x + dx * 0, y + dy * 0), L"hard");
        Button* soft = Button::CreateToggleButton(terrainOptions, Vector2d(x + dx * 1, y + dy * 0), L"soft");
        for (auto each : { hard,soft })
        {
            each->radioSelectGroup = { hard,soft };
        }
        hard->onSelect = [=]() { brush = BrushType::HardTerrain; };
        hard->onEnable = [=]() { hard->OnSelected(); };
        soft->onSelect = [=]() { brush = BrushType::SoftTerrain; };
        soft->onEnable = [=]() { soft->OnSelected(); };
    }
    // 유닛 화면 구성
    {
        constexpr double dx = 175;
        constexpr double dy = -175;
        Button* player = Button::CreateToggleIconButton(unitOptions, Vector2d(x + dx * 0, y + dy * 0), L"sprites/player/torso/idle_forward");
        Button* enemyGroup = Button::CreateToggleIconButton(unitOptions, Vector2d(x + dx * 1, y + dy * 0), L"sprites/enemy/shooter/idle");
        for (auto each : { player, enemyGroup })
        {
            each->radioSelectGroup = { player,enemyGroup };
        }
        player->onSelect = [=]() { brush = BrushType::PlayerStartPoint; brushLayer = BrushLayer::Unit; lastSelectedEditorButton = player; };
        player->onDeselect = [=]()
        {
            if (lastSelectedEditorButton == player)
            {
                brush = BrushType::None;
            }
        };
        player->onEnable = [=]() { player->OnSelected(); };
        enemyGroup->onSelect = [=]() { brush = BrushType::None; brushLayer = BrushLayer::Unit; enemyOptions->SetSelfActive(true); lastSelectedEditorButton = enemyGroup; };
        enemyGroup->onEnable = [=]() { enemyGroup->OnSelected(); };
        enemyGroup->onDeselect = [=]()
        {
            enemyOptions->SetSelfActive(false);
            if (lastSelectedEditorButton == enemyGroup)
            {
                brush = BrushType::None;
            }
        };
    }
    // 이미지 화면 구성
    InitImageScreen(imageOptions, x, y);
    // Enemy 화면 구성
    {
        constexpr double dx = 175;
        constexpr double dy = -175;
        Button* commonEnemy = Button::CreateToggleIconButton(enemyOptions, Vector2d(x + dx * 0, y + dy * 1), L"sprites/enemy/shooter/idle");
        Button* strongEnemy = Button::CreateToggleIconButton(enemyOptions, Vector2d(x + dx * 1, y + dy * 1), L"sprites/enemy/shooter1/idle");
        Button* shooter = Button::CreateToggleIconButton(enemyOptions, Vector2d(x + dx * 2, y + dy * 1), L"sprites/enemy/shooter2/idle");
        Button* bomber = Button::CreateToggleIconButton(enemyOptions, Vector2d(x + dx * 3, y + dy * 1), L"sprites/enemy/shooter_curve/idle");
        Button* laser_V = Button::CreateToggleIconButton(enemyOptions, Vector2d(x + dx * 4, y + dy * 1), L"sprites/enemy/laserEnemy/idle");
        Button* laser_H = Button::CreateToggleIconButton(enemyOptions, Vector2d(x + dx * 5, y + dy * 1), L"sprites/enemy/shooter3/idle");
        Button* elite = Button::CreateToggleIconButton(enemyOptions, Vector2d(x + dx * 4, y + dy * 0), L"sprites/enemy/eliteMonster/idle");
        Button* boss = Button::CreateToggleIconButton(enemyOptions, Vector2d(x + dx * 5, y + dy * 0), L"sprites/spaceship/idle");
        Button* wave = Button::CreateToggleButton(enemyOptions, Vector2d(x + dx * 0, y + dy * 2), L"웨이브");

        for (auto each : { commonEnemy,strongEnemy, shooter, bomber, laser_V, laser_H, elite, boss ,wave })
        {
            each->radioSelectGroup = { commonEnemy,strongEnemy, shooter, bomber, laser_V, laser_H, elite, boss ,wave };
        }
        commonEnemy->onSelect = [=]() { brush = BrushType::CommonEnemy; };
        commonEnemy->onEnable = [=]() { commonEnemy->OnSelected(); };
        strongEnemy->onSelect = [=]() { brush = BrushType::StrongEnemy; };
        strongEnemy->onEnable = [=]() { strongEnemy->OnSelected(); };
        shooter->onSelect = [=]() { brush = BrushType::Shooter; };
        shooter->onEnable = [=]() { shooter->OnSelected(); };
        bomber->onSelect = [=]() { brush = BrushType::Bomber; };
        bomber->onEnable = [=]() { bomber->OnSelected(); };
        laser_V->onSelect = [=]() { brush = BrushType::Laser_V; };
        laser_V->onEnable = [=]() { laser_V->OnSelected(); };
        laser_H->onSelect = [=]() { brush = BrushType::Laser_H; };
        laser_H->onEnable = [=]() { laser_H->OnSelected(); };
        elite->onSelect = [=]() { brush = BrushType::Elite; };
        elite->onEnable = [=]() { elite->OnSelected(); };
        boss->onSelect = [=]() { brush = BrushType::Boss; };
        boss->onEnable = [=]() { boss->OnSelected(); };
        wave->onSelect = [=]() { brush = BrushType::Wave; };
    }
    /// 아이템 화면 구성
    {
        constexpr double dx = 175;
        constexpr double dy = -175;
        Button* healpack = Button::CreateToggleIconButton(itemOptions, Vector2d(x + dx * 0, y + dy * 0), L"sprites/item/healpack");
        Button* attackSpeedUp = Button::CreateToggleIconButton(itemOptions, Vector2d(x + dx * 1, y + dy * 0), L"sprites/item/forcepack");
        for (auto each : { healpack,attackSpeedUp })
        {
            each->radioSelectGroup = { healpack,attackSpeedUp };
        }
        healpack->onSelect = [=]() { brush = BrushType::HealPack; };
        healpack->onEnable = [=]() { healpack->OnSelected(); };
        attackSpeedUp->onSelect = [=]() { brush = BrushType::AttackSpeedUp; };
        attackSpeedUp->onEnable = [=]() { attackSpeedUp->OnSelected(); };
    }
#endif
    //editorBehindBackgroundLayer->SetSelfActive(false);
    //editorBackgroundLayer->SetSelfActive(false);
    //editorForegroundLayer->SetSelfActive(false);
    //editorUnitsLayer->SetSelfActive(false);
    DisableMap();
}
void MapTool::SetMapToolActive(bool active)
{
    if (active)
        Time::SetTimeScale(0.0000001);
    else
        Time::SetTimeScale(1);

    if (active)
        MapObject::GlobalOnEngageMapEditorCallbacks();
    else
        MapObject::GlobalOnDisengageMapEditorCallbacks();


    mapToolUILayer->SetSelfActive(active);
#ifdef _MAPTOOL
    if (active)
        UI::UIManager::GetInstance()->HideUI(UIManager::UIEnum::InGameUI);
    else
        UI::UIManager::GetInstance()->ShowUI(UIManager::UIEnum::InGameUI);

#endif
}
bool MapTool::isEditingMap()
{
#ifdef _MAPTOOL
    return mapToolUILayer->GetActive();
#else
    return false;
#endif
}
void MapTool::LoadMap(wstring mapRoot)
{
    mapRoot = L"map/" + mapRoot;
    TerrainPoint::Load(mapRoot);
    PlayerStartPoint::Load(mapRoot);
    EnemyMarker::Load(mapRoot);
    EnemyWave::Load(mapRoot);
    ItemMarker::Load(mapRoot);
    RailPoint::Load(mapRoot);
    MapImage::Load(mapRoot);
    LoadBackground(mapRoot);
    if (Player::GetInstance())
        Player::GetInstance()->ResetPlayerStatus();

}
void MapTool::SaveMap(wstring mapRoot)
{
    _wmkdir(L"map");
    mapRoot = L"map/" + mapRoot;
    _wmkdir(mapRoot.c_str());
    TerrainPoint::Save(mapRoot);
    PlayerStartPoint::Save(mapRoot);
    EnemyMarker::Save(mapRoot);
    EnemyWave::Save(mapRoot);
    ItemMarker::Save(mapRoot);
    RailPoint::Save(mapRoot);
    MapImage::Save(mapRoot);
    SaveBackground(mapRoot);
}
void MapTool::Update()
{
#ifdef _MAPTOOL
    if (D2DInput::isKeyPushed(KeyCode::Tab))
    {
        SetMapToolActive(!isEditingMap());
        if (isEditingMap())
        {

            brushLayer = gs_map::MapTool::BrushLayer::None;
            brush = gs_map::MapTool::BrushType::None;
        }
        else
        {
            // 플레이어가 없는 경우는 최초 시작이 아예 안된 경우다. 
            if (!Player::GetInstance())
                Restart();
        }
    }
    if (D2DInput::isKeyPushed(KeyCode::Insert))
        DebugObject::ToggleDebugmode();

    if (isEditingMap())
    {
        switch (brushLayer)
        {
        case gs_map::MapTool::BrushLayer::None:
            break;
        case gs_map::MapTool::BrushLayer::Terrain:
            break;
        case gs_map::MapTool::BrushLayer::Unit:
            break;
        case gs_map::MapTool::BrushLayer::Item:
            break;
        case gs_map::MapTool::BrushLayer::Background:
            break;
        case gs_map::MapTool::BrushLayer::Image:
        {
            constexpr double zoomSpeed = 0.5;
            double zoom = 0;
            constexpr double rotationSpeed[5] = { 3.75,7.5,15,30,60 };
            double rotation = 0;
            // 크기 조절
            if (D2DInput::isKeyDown(KeyCode::NUMPAD_5))
            {
                zoom = zoomSpeed;
            }
            if (D2DInput::isKeyDown(KeyCode::NUMPAD_4))
            {
                zoom = -zoomSpeed;
            }
            // 각도 조절
            if (D2DInput::isKeyDown(KeyCode::NUMPAD_1))
                rotation = rotationSpeed[4];
            if (D2DInput::isKeyDown(KeyCode::NUMPAD_2))
                rotation = -rotationSpeed[4];
            if (D2DInput::isKeyDown(KeyCode::NUM_1))
                rotation = rotationSpeed[0];
            if (D2DInput::isKeyDown(KeyCode::NUM_2))
                rotation = rotationSpeed[1];
            if (D2DInput::isKeyDown(KeyCode::NUM_3))
                rotation = rotationSpeed[2];
            if (D2DInput::isKeyDown(KeyCode::NUM_4))
                rotation = rotationSpeed[3];
            if (D2DInput::isKeyDown(KeyCode::NUM_5))
                rotation = rotationSpeed[4];
            if (D2DInput::isKeyDown(KeyCode::NUM_6))
                rotation = -rotationSpeed[0];
            if (D2DInput::isKeyDown(KeyCode::NUM_7))
                rotation = -rotationSpeed[1];
            if (D2DInput::isKeyDown(KeyCode::NUM_8))
                rotation = -rotationSpeed[2];
            if (D2DInput::isKeyDown(KeyCode::NUM_9))
                rotation = -rotationSpeed[3];
            if (D2DInput::isKeyDown(KeyCode::NUM_0))
                rotation = -rotationSpeed[4];
            if (D2DInput::isKeyPushed(KeyCode::ESC))
            {
                MapImage::selectedImage = nullptr;
                MapImage::selectedRectangle->GetGameObject()->SetSelfActive(false);
            }
            if (MapImage::selectedImage)
            {
                auto transform = MapImage::selectedImage->GetTransform();
                transform->scale.x += zoom * Time::GetDeltaTimeUnscaled();
                transform->scale.y += zoom * Time::GetDeltaTimeUnscaled();
                transform->rotation = Vector3d(0, 0, transform->rotation.Euler().z + rotation * Time::GetDeltaTimeUnscaled());
                MapImage::selectedImage->PropagateTransform();
                if (D2DInput::isKeyPushed(KeyCode::NUMPAD_8))
                {
                    MapImage::selectedImage->PlaceTop();
                }
                if (D2DInput::isKeyPushed(KeyCode::NUMPAD_7))
                {
                    MapImage::selectedImage->PlaceBottom();
                }
            }
        }
        break;
        case gs_map::MapTool::BrushLayer::Rail:
            break;
        default:
            break;
        }

        // 모든 클릭에 대한 이벤트
        if (D2DInput::isKeyPushed(KeyCode::MouseLeftClick))
        {
            for (auto each : deselectOnEmptyClick)
                each->OnDeselected();
        }
        // 빈 공간에 대한 클릭에 따른 이벤트
        if (D2DInput::isKeyPushed(KeyCode::MouseLeftClick) && Cursor::touchingNothing)
        {
            switch (brush)
            {
            case gs_map::MapTool::BrushType::FreeSelect:
                break;
            case gs_map::MapTool::BrushType::HardTerrain:
                if (D2DInput::isKeyDown(KeyCode::Control))
                    TerrainPoint::CreateTerrainPoint(D2DInput::getMouseWorldPosition(), nullptr, false);
                else if (D2DInput::isKeyDown(KeyCode::LShift))
                    TerrainPoint::CreateTerrainPoint(D2DInput::getMouseWorldPosition(), TerrainPoint::extendStartPoint, false);
                break;
            case gs_map::MapTool::BrushType::SoftTerrain:
                if (D2DInput::isKeyDown(KeyCode::Control))
                    TerrainPoint::CreateTerrainPoint(D2DInput::getMouseWorldPosition(), nullptr, true);
                else if (D2DInput::isKeyDown(KeyCode::LShift))
                    TerrainPoint::CreateTerrainPoint(D2DInput::getMouseWorldPosition(), TerrainPoint::extendStartPoint, true);
                break;
            case gs_map::MapTool::BrushType::CameraRail:
                if (D2DInput::isKeyDown(KeyCode::LShift))
                    RailPoint::CreateRailPoint(D2DInput::getMouseWorldPosition(), RailPoint::extendStartPoint);
                break;
            case gs_map::MapTool::BrushType::PlayerStartPoint:
                if (D2DInput::isKeyDown(KeyCode::Control))
                    PlayerStartPoint::CreatePlayerStartPoint(D2DInput::getMouseWorldPosition());
                break;
            case gs_map::MapTool::BrushType::CommonEnemy:
                if (D2DInput::isKeyDown(KeyCode::Control))
                    EnemyMarker::CreateEnemyMarker(D2DInput::getMouseWorldPosition(), EnemyType::CommonEnemy, EnemyWave::selectedWave);
                break;
            case gs_map::MapTool::BrushType::StrongEnemy:
                if (D2DInput::isKeyDown(KeyCode::Control))
                    EnemyMarker::CreateEnemyMarker(D2DInput::getMouseWorldPosition(), EnemyType::StrongEnemy, EnemyWave::selectedWave);
                break;
            case gs_map::MapTool::BrushType::Shooter:
                if (D2DInput::isKeyDown(KeyCode::Control))
                    EnemyMarker::CreateEnemyMarker(D2DInput::getMouseWorldPosition(), EnemyType::Shooter, EnemyWave::selectedWave);
                break;
            case gs_map::MapTool::BrushType::Bomber:
                if (D2DInput::isKeyDown(KeyCode::Control))
                    EnemyMarker::CreateEnemyMarker(D2DInput::getMouseWorldPosition(), EnemyType::Bomber, EnemyWave::selectedWave);
                break;
            case gs_map::MapTool::BrushType::Laser_V:
                if (D2DInput::isKeyDown(KeyCode::Control))
                    EnemyMarker::CreateEnemyMarker(D2DInput::getMouseWorldPosition(), EnemyType::Laser_V, EnemyWave::selectedWave);
                break;
            case gs_map::MapTool::BrushType::Laser_H:
                if (D2DInput::isKeyDown(KeyCode::Control))
                    EnemyMarker::CreateEnemyMarker(D2DInput::getMouseWorldPosition(), EnemyType::Laser_H, EnemyWave::selectedWave);
                break;
            case gs_map::MapTool::BrushType::Elite:
                if (D2DInput::isKeyDown(KeyCode::Control))
                    EnemyMarker::CreateEnemyMarker(D2DInput::getMouseWorldPosition(), EnemyType::Elite, EnemyWave::selectedWave);
                break;
            case gs_map::MapTool::BrushType::Boss:
                if (D2DInput::isKeyDown(KeyCode::Control))
                    EnemyMarker::CreateEnemyMarker(D2DInput::getMouseWorldPosition(), EnemyType::Boss, EnemyWave::selectedWave);
                break;
            case gs_map::MapTool::BrushType::Wave:
                if (D2DInput::isKeyDown(KeyCode::Control))
                    EnemyWave::CreateEnemyWave(D2DInput::getMouseWorldPosition());
                break;
            case gs_map::MapTool::BrushType::HealPack:
                if (D2DInput::isKeyDown(KeyCode::Control))
                    ItemMarker::CreateItemMarker(D2DInput::getMouseWorldPosition(), ItemType::HealPack);
                break;
            case gs_map::MapTool::BrushType::AttackSpeedUp:
                if (D2DInput::isKeyDown(KeyCode::Control))
                    ItemMarker::CreateItemMarker(D2DInput::getMouseWorldPosition(), ItemType::AttackSpeedUp);
                break;
            case gs_map::MapTool::BrushType::Image:
                if (D2DInput::isKeyDown(KeyCode::Control))
                    if (MapImage::selectedImage)
                        MapImage::CreateMapImage(D2DInput::getMouseWorldPosition(), MapImage::selectedImage);
                    else
                        MapImage::CreateMapImage(D2DInput::getMouseWorldPosition(), selectedImagelayerType);
                break;
            default:
                break;
            }
        }
        // Delete 키를 눌렀을 때의 이벤트
        if (D2DInput::isKeyPushed(KeyCode::Delete))
        {
            switch (brush)
            {
            case gs_map::MapTool::BrushType::FreeSelect:
                if (RailPoint::extendStartPoint)
                    RailPoint::DeleteRailPoint(RailPoint::extendStartPoint);
                break;
            case gs_map::MapTool::BrushType::HardTerrain:
            case gs_map::MapTool::BrushType::SoftTerrain:
                if (TerrainPoint::extendStartPoint)
                    TerrainPoint::DeleteTerrainPoint(TerrainPoint::extendStartPoint);
                break;
            case gs_map::MapTool::BrushType::CameraRail:
                if (RailPoint::extendStartPoint)
                    RailPoint::DeleteRailPoint(RailPoint::extendStartPoint);
                break;
            case gs_map::MapTool::BrushType::Image:
                if (MapImage::selectedImage)
                    MapImage::DeleteMapImage(MapImage::selectedImage);
                break;
            default:
                if (EnemyWave::selectedWave && brushLayer == gs_map::MapTool::BrushLayer::Unit)
                    EnemyWave::DeleteWave(EnemyWave::selectedWave);
                if (EnemyMarker::selectedEnemyMarker && brushLayer == gs_map::MapTool::BrushLayer::Unit)
                    EnemyMarker::DeleteEnemyMarker(EnemyMarker::selectedEnemyMarker);
                if (ItemMarker::selectedItemMarker && brushLayer == gs_map::MapTool::BrushLayer::Item)
                    ItemMarker::DeleteItemMarker(ItemMarker::selectedItemMarker);
                break;
            }
        }
    }
#endif
}

void gs_map::MapTool::Restart()
{
    MapObject::CallOnRestartAll();
    CameraRail::nowRail = CameraRail::firstRail;
    GSCamera* cam = GSCamera::GetInstance();
    //cam->GetTransform()->SetWorldPosition(Player::GetInstance()->GetTransform()->GetWorldPosition());
    Threat::RemoveAllThreats();
    Item::RemoveAllItems();
    if (!(CameraRail::nowRail == nullptr))
    {
        cam->Restart();
        DebugObject::CreatePopUpCircle(cam->GetGameObject(), 30, 0.5, D2D1::ColorF::Green);
    }

    /// 카메라 레일을 따라가지 않는 경우 사용할 코드
    //cam->GetTransform()->SetWorldPosition(PlayerStartPoint::GetInstance()->GetTransform()->GetWorldPosition() + Vector2d(0, 200));
}

void gs_map::MapTool::Resume()
{

}

void gs_map::MapTool::SaveBackground(wstring mapRoot)
{
    wfstream file;
    const wstring filename = L"background.txt";
    file.open(mapRoot + L'\\' + filename, fstream::out | fstream::trunc);
    //if (file.fail())
    //    _wassert(_CRT_WIDE("map/맵 이름/ 폴더를 먼저 만들어야 저장이 가능합니다."), _CRT_WIDE(__FILE__), (unsigned)(__LINE__));
//#endif

    file << backgroundSprite->GetSpriteFilePath();
    file.close();
}
void gs_map::MapTool::LoadBackground(wstring mapRoot)
{
    wfstream file;
    const wstring filename = L"background.txt";
    file.open(mapRoot + L'\\' + filename);
    if (file.fail())
        return;

    wstring path;
    file >> path;
    backgroundSprite->SetSpriteFilePath(path);
    file.close();
}

wstring gs_map::MapTool::GetLastEditedMapName()
{
    wfstream file;
    const wstring filename = L"lastMapName.txt";
    file.open(L"map\\" + filename);
    if (file.fail())
        return L"default";

    wstring mapName;
    file >> mapName;
    file.close();
    if (mapName == L"")
        return L"default";
    else
        return mapName;
}
void gs_map::MapTool::SaveLastEditedMapName(wstring mapName)
{
    wfstream file;
    const wstring filename = L"lastMapName.txt";
    file.open(L"map\\" + filename, fstream::out | fstream::trunc);

    if (file.fail())
        return;


    file << mapName;
    file.close();
}
void gs_map::MapTool::InitImageScreen(GameObject* parent, double x, double y)
{
    constexpr double dx = 120;
    constexpr double dy = -80;

    D2DText* fileNameText;
    D2DText* scrollText;
    Button* path = Button::CreateToggleButton(parent, Vector2d(x + dx * 1, y + dy * 0), L"이미지 경로 : ", 350, 75, &fileNameText);
    Button* scroll = Button::CreateToggleButton(parent, Vector2d(x + dx * 1, y + dy * 1), L"스크롤 속도 배율 : ", 350, 75, &scrollText);
    //Button* size = Button::CreateToggleButton(parent, Vector2d(x + dx * 1, y + dy * 2), L"이미지 크기 배율 : ", 350);
    //Button* order = Button::CreateToggleButton(parent, Vector2d(x + dx * 1, y + dy * 3), L"이미지 출력 순서 : ", 350);
    Button* behindBackground = Button::CreateButton(parent, Vector2d(x + dx * 0, y + dy * 4), L"배경 뒤", 100);
    Button* backCharacters = Button::CreateButton(parent, Vector2d(x + dx * 1, y + dy * 4), L"캐릭터 뒤", 100);
    Button* foreCharacters = Button::CreateButton(parent, Vector2d(x + dx * 2, y + dy * 4), L"캐릭터 앞", 100);

    //Button* behindBackground = Button::CreateToggleSimpleIconButton(parent, Vector2d(x + dx * 0, y + dy * 5), L"sprites/test.png", 100, 100);
    //Button* backCharacters = Button::CreateToggleSimpleIconButton(parent, Vector2d(x + dx * 1, y + dy * 5), L"sprites/buttons/background.png", 100, 100);
    //Button* foreCharacters = Button::CreateToggleSimpleIconButton(parent, Vector2d(x + dx * 2, y + dy * 5), L"sprites/buttons/foreground.png", 100, 100);


    auto fileNameTextInput = path->GetGameObject()->AddComponent<TextInput>();
    deselectOnEmptyClick.push_back(path);
    path->onSelect = [=]() {
        fileNameTextInput->isTyping = true;
        lastSelectedEditorButton = path;
        if (MapImage::selectedImage)
        {
            wstringstream wsstream;
            wsstream << MapImage::selectedImage->sprite->GetSpriteFilePath();
            fileNameTextInput->typedInput = wsstream.str();
        }
    };

    path->deselectOnESC = true;
    path->onDeselect = [=]()
    {
        fileNameTextInput->isTyping = false;
    };
    path->onUpdate = [=]()
    {
        static MapImage* lastSelectedImage = nullptr;
        if (MapImage::selectedImage && lastSelectedImage != MapImage::selectedImage)
        {
            wstringstream wsstream;
            fileNameText->text = L"이미지 경로 : " + MapImage::selectedImage->sprite->GetSpriteFilePath();
        }
        lastSelectedImage = MapImage::selectedImage;
    };
    path->onUpdateWhileSelected = [=]()
    {
        fileNameText->text = L"이미지 경로 : " + fileNameTextInput->typedInput;
        if (MapImage::selectedImage)
        {
            MapImage::selectedImage->sprite->SetSpriteFilePath(fileNameTextInput->typedInput);
            if (MapImage::selectedImage->sprite->GetDrawRect().width == 0)
                MapImage::selectedImage->sprite->SetSpriteFilePath(L"sprites/no_image.png");
            else
            {
                MapImage::selectedImage->boxCollider->SetWidth(MapImage::selectedImage->sprite->GetDrawRect().width);
                MapImage::selectedImage->boxCollider->SetHeight(MapImage::selectedImage->sprite->GetDrawRect().height);
                MapImage::selectedRectangle->width = MapImage::selectedImage->boxCollider->GetWidth();
                MapImage::selectedRectangle->height = MapImage::selectedImage->boxCollider->GetHeight();
            }
        }
    };
    // 스크롤 배율
    deselectOnEmptyClick.push_back(scroll);
    auto scrollFactorInput = scroll->GetGameObject()->AddComponent<TextInput>();
    scroll->deselectOnESC = true;
    scroll->onSelect = [=]() {
        scrollFactorInput->isTyping = true; lastSelectedEditorButton = path;
        if (MapImage::selectedImage)
        {
            wstringstream wsstream;
            wsstream << MapImage::selectedImage->scrollRate;
            scrollFactorInput->typedInput = wsstream.str();
        }
    };
    scroll->onDeselect = [=]()
    {
        scrollFactorInput->isTyping = false;
    };
    scroll->onUpdate = [=]()
    {
        static MapImage* lastSelectedImage = nullptr;
        if (MapImage::selectedImage && lastSelectedImage != MapImage::selectedImage)
        {
            wstringstream wsstream;
            wsstream << MapImage::selectedImage->scrollRate;
            scrollFactorInput->typedInput = wsstream.str();
            scrollText->text = L"스크롤 속도 배율 : " + scrollFactorInput->typedInput;
        }
        lastSelectedImage = MapImage::selectedImage;
    };
    scroll->onUpdateWhileSelected = [=]()
    {
        if (MapImage::selectedImage)
        {
            scrollText->text = L"스크롤 속도 배율 : " + scrollFactorInput->typedInput;
            try {
                MapImage::selectedImage->SetScrollRate(stod(scrollFactorInput->typedInput));
            }
            catch (exception e) {};
        }
    };
    // 출력 순서 
    //auto orderInput = order->GetGameObject()->AddComponent<TextInput>();
    behindBackground->onClick = [=]()
    {
        //editorBackgroundLayer->SetSelfActive(true);
        //editorForegroundLayer->SetSelfActive(false);
        if (MapImage::selectedImage)
        {
            MapImage::selectedImage->SetScrollRate(0.5);
            MapImage::selectedImage->SetLayerType(MapImage::LayerType::BehindBackground);
        }
    };
    backCharacters->onClick = [=]()
    {
        //editorbackgroundlayer->setselfactive(true);
        //editorforegroundlayer->setselfactive(false);
        if (MapImage::selectedImage)
            MapImage::selectedImage->SetLayerType(MapImage::LayerType::BackCharacters);
    };
    foreCharacters->onClick = [=]()
    {
        /*editorBackgroundLayer->SetSelfActive(false);
        editorForegroundLayer->SetSelfActive(true);*/
        if (MapImage::selectedImage)
            MapImage::selectedImage->SetLayerType(MapImage::LayerType::ForeCharacters);
    };

    editorBackgroundLayer->SetSelfActive(false);
    editorForegroundLayer->SetSelfActive(false);
}
