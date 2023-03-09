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

        GameObject* GetBehindBackgroundLayer() { return behindBackgroundLayer; }; // �������� ��� ������Ʈ�� ��ġ
        GameObject* GetEditorBehindBackgroundLayer() { return editorBehindBackgroundLayer; }; // �������� ��� ������Ʈ�� ��ġ
        GameObject* GetBackgroundLayer() { return backgroundLayer; }
        GameObject* GetBackgroundObjectsLayer() { return backgroundObjectsLayer; }
        GameObject* GetRailLayer() { return railLayer; }    // �߰�
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

        GameObject* behindBackgroundLayer = nullptr; // ��¥��� �ڿ� ��ġ�Ǵ� ��� ������Ʈ�� ��ġ
        GameObject* editorBehindBackgroundLayer = nullptr; // ��¥��� �ڿ� ��ġ�Ǵ� ��� ������Ʈ���� ���� ������ ���̾�
        GameObject* backgroundLayer = nullptr;  // ��¥ ��� ��ġ
        GameObject* backgroundObjectsLayer = nullptr; // ��� ������Ʈ�� ��ġ
        GameObject* railLayer = nullptr;    // �߰�
        GameObject* enemyLayer = nullptr; // ������ ��ġ
        GameObject* itemLayer = nullptr;    // ������ ��ġ
        GameObject* projectileLayer = nullptr; // ������ �߻�ü�� ��ġ
        GameObject* playerProjectileLayer = nullptr; // �÷��̾��� �߻�ü�� ��ġ
        GameObject* playerLayer = nullptr; // �÷��̾ ��ġ
        GameObject* foregroundObjectsLayer = nullptr; // ���� ��ü���� ������ ������Ʈ�� ��ġ
        GameObject* groundLayer = nullptr; // �浹ü ����� ������Ʈ�� ��ġ
        GameObject* editorBackgroundLayer = nullptr; // �������� ��� ������Ʈ�� ��ġ
        GameObject* editorUnitsLayer = nullptr; // ������ ���Խ� editorUnitsLayer���� ���� ��ġ�� ���̾���� �� �帴�ϰ� ������ �Ѵ�.
        GameObject* editorForegroundLayer = nullptr; // ���ֺ��� �켱�ϴ� ��� ������Ʈ�� ��ġ
        GameObject* foregroundLayer = nullptr; // �� ������ ���Խ� �������ϰ�
        GameObject* mapToolUILayer = nullptr; // �� �� UI ��ư���� ��ġ
        vector<Button*> deselectOnEmptyClick;

        void InitImageScreen(GameObject* parent, double x, double y);
        virtual void Start();

        static Button* lastSelectedEditorButton;
        static MapTool* instance;
    };
}
