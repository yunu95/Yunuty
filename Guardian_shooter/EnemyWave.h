#pragma once
#include "MapObject.h"
#include "EnemyMarker.h"
#include "Timer.h"

namespace gs_map
{
    class EnemyMarker;
    class EnemyWave :public MapObject
    {
    public:
        static EnemyWave* CreateEnemyWave(Vector3d position);
        static void DeleteWave(EnemyWave*);
        static void Save(wstring mapRoot);
        static void Load(wstring mapRoot);
        void InsertMarker(EnemyMarker* marker);
        void ToggleMarkerEssential(EnemyMarker* marker);
        void HandleDeathEvent(Threat* threat);
        void HandleMarkerDeletion(EnemyMarker* threat);
        EnemyWave();
        ~EnemyWave();

        void Relocate(Vector3d position);
        virtual void OnRestart();
        virtual void Invoke();
        virtual void OnEngagingMapEditMode();
        Timer waveTimer;
        vector<pair<double, EnemyMarker*>> enemyMarkers;
        priority_queue<pair<double, EnemyMarker*>, vector<pair<double, EnemyMarker*>>, greater<pair<double, EnemyMarker*>>> enemyMarkersQueue;
        bool isMapEditor = true;
    protected:
        virtual void Start() override;
        virtual void Update() override;
        virtual void OnCollisionEnter2D(const Collision2D& collision) override;
    private:
        bool waveEnded;
        static EnemyWave* selectedWave;
        static unordered_set<EnemyWave*> waves;
        unordered_set<EnemyMarker*> passRequirementsLeft;
        unordered_set<EnemyMarker*> passRequirements;
        unordered_map<EnemyMarker*,GameObject*> markersArrow;
        unordered_map<EnemyMarker*,GameObject*> requirementsArrows;
        Button* uiButton = nullptr;
        BoxCollider2D* eventCollider = nullptr;
        // 깊은 뜻이 있는 getter,setter
        Vector3d GetButtonPosition();
        bool isInMapEditor() { return isMapEditor; };
        bool invokedBefore = false;
        friend Button;
        friend MapTool;
        friend EnemyMarker;
    };
}
