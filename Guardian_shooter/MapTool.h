#pragma once
#include <vector>
#include "YunutyEngine.h"
#include "MapImage.h"

class Cursor;
class Button;

using namespace gs_map;

namespace gs_map
{
    class MapObject;
    class TerrainPoint;
    class RailPoint;
    class PlayerStartPoint;
    class EnemyMarker;

    class MapTool
        : public Component
    {
    public:
        enum class BrushLayer { None, Terrain, Unit, Item, Background, Image, Rail };
        enum class BrushType {
            None, FreeSelect,
            HardTerrain, SoftTerrain,
            CameraRail,
            PlayerStartPoint, CommonEnemy, StrongEnemy, Shooter, Bomber, Laser_V, Laser_H, Elite, Boss,
            HealPack, AttackSpeedUp,
            Wave,
            Image
        };
        static MapTool* GetInstance() { return instance; }
        MapTool();
        ~MapTool();
        void Initialize();
        void SetMapToolActive(bool active);
        bool isEditingMap();
        void LoadMap(wstring fileName);
        void SaveMap(wstring fileName);
        void Restart();
        void Resume();
        void EnableMap() { GetGameObject()->SetSelfActive(true); }
        void DisableMap() { GetGameObject()->SetSelfActive(false); }

        BrushLayer GetBrushLayer() { return brushLayer; };
        BrushType GetBrushType() { return brush; };

        GameObject* GetBehindBackgroundLayer() { return behindBackgroundLayer; }; // 에디터의 배경 오브젝트들 배치
        GameObject* GetEditorBehindBackgroundLayer() { return editorBehindBackgroundLayer; }; // 에디터의 배경 오브젝트들 배치
        GameObject* GetBackgroundLayer() { return backgroundLayer; }
        GameObject* GetBackgroundObjectsLayer() { return backgroundObjectsLayer; }
        GameObject* GetRailLayer() { return railLayer; }    // 추가
        GameObject* GetEnemyLayer() { return enemyLayer; }
        GameObject* GetItemLayer() { return itemLayer; }
        GameObject* GetProjectileLayer() { return projectileLayer; }
        GameObject* GetPlayerProjectileLayer() { return playerProjectileLayer; }
        GameObject* GetPlayerLayer() { return playerLayer; }
        GameObject* GetForegroundObjectsLayer() { return foregroundObjectsLayer; }
        GameObject* GetGroundLayer() { return groundLayer; }
        GameObject* GetEditorBackgroundLayer() { return editorBackgroundLayer; }
        GameObject* GetEditorUnitsLayer() { return editorUnitsLayer; }
        GameObject* GetEditorForegroundLayer() { return editorForegroundLayer; }
        GameObject* GetForegroundLayer() { return foregroundLayer; }
        GameObject* GetMapToolUILayer() { return mapToolUILayer; }
        wstring GetLastEditedMapName();
    protected:
        virtual void Update();
    private:
        void SaveBackground(wstring fileName);
        void LoadBackground(wstring fileName);
        void SaveLastEditedMapName(wstring mapName);
        BrushLayer brushLayer = BrushLayer::None;
        BrushType brush = BrushType::None;
        D2DSprite* backgroundSprite = nullptr;
        MapImage::LayerType selectedImagelayerType = MapImage::LayerType::BackCharacters;

        GameObject* behindBackgroundLayer = nullptr; // 통짜배경 뒤에 배치되는 배경 오브젝트들 배치
        GameObject* editorBehindBackgroundLayer = nullptr; // 통짜배경 뒤에 배치되는 배경 오브젝트들을 위한 에디터 레이어
        GameObject* backgroundLayer = nullptr;  // 통짜 배경 배치
        GameObject* backgroundObjectsLayer = nullptr; // 배경 오브젝트들 배치
        GameObject* railLayer = nullptr;    // 추가
        GameObject* enemyLayer = nullptr; // 적군을 배치
        GameObject* itemLayer = nullptr;    // 아이템 배치
        GameObject* projectileLayer = nullptr; // 적군의 발사체를 배치
        GameObject* playerProjectileLayer = nullptr; // 플레이어의 발사체를 배치
        GameObject* playerLayer = nullptr; // 플레이어를 배치
        GameObject* foregroundObjectsLayer = nullptr; // 게임 객체들을 가리는 오브젝트들 배치
        GameObject* groundLayer = nullptr; // 충돌체 디버그 오브젝트를 배치
        GameObject* editorBackgroundLayer = nullptr; // 에디터의 배경 오브젝트들 배치
        GameObject* editorUnitsLayer = nullptr; // 에디터 진입시 editorUnitsLayer보다 위에 배치된 레이어들은 좀 흐릿하게 보여야 한다.
        GameObject* editorForegroundLayer = nullptr; // 유닛보다 우선하는 배경 오브젝트들 배치
        GameObject* foregroundLayer = nullptr; // 맵 에디터 진입시 반투명하게
        GameObject* mapToolUILayer = nullptr; // 맵 툴 UI 버튼들을 배치
        vector<Button*> deselectOnEmptyClick;

        void InitImageScreen(GameObject* parent, double x, double y);
        virtual void Start();

        static Button* lastSelectedEditorButton;
        static MapTool* instance;
    };
}
