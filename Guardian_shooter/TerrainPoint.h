#pragma once
#include <fstream>
#include "MapObject.h"
#include "Ground.h"

namespace gs_map
{
    class MapTool;
    class TerrainPoint : public MapObject
    {
    public:
        TerrainPoint() { terrainPoints.insert(this); }
        ~TerrainPoint()
        {
            terrainPoints.erase(this);
        }
        // 파일 입출력 로직
        static void Save(wstring mapRoot);
        static void Load(wstring mapRoot);
        static TerrainPoint* CreateTerrainPoint(Vector2d location, TerrainPoint* previous = nullptr, bool penetrable = false);
        static void DeleteTerrainPoint(TerrainPoint* point);
    protected:
    private:
        static unordered_set<TerrainPoint*> terrainPoints;
        static TerrainPoint* extendStartPoint;
        static Ground* CreateGround(TerrainPoint* tp1, TerrainPoint* tp2);
        static void AdjustGrounds(TerrainPoint* tp1);

        bool penetrable = false;
        TerrainPoint* previous = nullptr;
        TerrainPoint* next = nullptr;
        Ground* leftGround = nullptr;
        Ground* rightGround = nullptr;
        friend MapTool;
    };
}
