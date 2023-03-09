#pragma once
#include <fstream>
#include "EnemyType.h"
#include "MapObject.h"


namespace gs_map
{
    class MapTool;
    class EnemyWave;
    class EnemyMarker :
        public MapObject
    {
    public:
        friend class MapTool;

        EnemyMarker();
        virtual ~EnemyMarker();
        // 파일 입출력 로직
        static void Save(wstring mapRoot);
        static void Load(wstring mapRoot);
        static EnemyMarker* CreateEnemyMarker(Vector2d location, EnemyType enemyType,EnemyWave* wave=nullptr);
        static void DeleteEnemyMarker(EnemyMarker* marker);
        void SetOpacity(double opacity);
        virtual void Invoke();
        virtual void OnRestart();
        virtual void OnEngagingMapEditMode();
        virtual void OnDisengagingMapEditMode();
        EnemyType enemyType = EnemyType::CommonEnemy;
        bool alreadySummoned = false;
        bool summonOnTouch = true;

    protected:
        virtual void Update() override;
        virtual void OnCollisionEnter2D(const Collision2D& collision) override;
        virtual void OnCollisionStay2D(const Collision2D& collision) override;
    private:
        int markerIndex;
        D2DAnimatedSprite* markerSprite = nullptr;
        EnemyWave* enemyWave = nullptr;
        static EnemyMarker* selectedEnemyMarker;
        static vector<EnemyMarker*> enemyMarkers;
        static ifstream fin;
        static ofstream fout;

        friend EnemyWave;
        friend MapTool;
    };
}
