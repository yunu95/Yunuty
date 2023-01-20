#pragma once
#include <stack>
#include "D2DGraphic.h"

#ifdef YUNUTY_EXPORTS
#define YUNUTY_API __declspec(dllexport)
#else
#define YUNUTY_API __declspec(dllimport)
#endif

namespace YunutyEngine
{
    /// <summary>
    /// a��Ÿ �˰����� �̿��� ��ã�Ⱑ �����ϵ��� 2d ��ǥ ������ �̻��� ����� �������� �ɰ��� �ִ� �������� Ŭ�����̴�.
    /// </summary>

    class NavigationUnit2DReport;
    class YUNUTY_API NavigationField2D : public Component
    {
    protected:
        static constexpr int dx[8] = { 0,0,1,-1,1,1,-1,-1 };
        static constexpr int dy[8] = { 1,-1,0,0,-1,1,-1,1 };
    public:
        class Tile;
        NavigationField2D();
        ~NavigationField2D();
        void SetField(Rect fieldRect, int minTileSize, int layerNum = 1);
        double GetTileSize() { return minTileSize; }
        Vector2d TileToWorldLocaction(const Tile& tile);
        static NavigationField2D* GetAssignedField(const Vector2d& worldLocation);
        static NavigationField2D* GetAssignedField(const Vector2d& worldLocation, const Vector2d& worldLocation2);
        // origin���� destination���� ���� ���� ��ȯ�Ѵ�.
        queue<Vector2d> RequestPath(Vector2d origin, Vector2d destination, int maxPathLength = 30, unordered_set<const Tile*>* openSetLocations = nullptr, unordered_set<const Tile*>* closedSetLocations = nullptr, unordered_set<const Tile*>* path=nullptr);
        void ReceiveUnitStatus(const NavigationUnit2DReport& newStatus, const NavigationUnit2DReport& lastStatus);
        // A��Ÿ �˰��� �ʿ��� ������ ������ �� �ִ� Ÿ�� ��� ��ü
        class Tile
        {
        public:
            enum class State
            {
                Closed,
                Open,
                Open_Occupied,
                Open_Occupied_Temporarily
            };
            Tile(int row, int col) : row(row), col(col) {}
            State state = State::Open;
            int row, col;
            double g_score = 0;
            mutable double f = 0;
            const Tile* previous = nullptr;
            static const Tile* destination;
            double heuristic() const { return Vector2d(row - destination->row, col - destination->col).Magnitude(); }
            double getFScore() const { return (f = g_score + heuristic()); }
            struct TileCompare
            {
                bool operator()(Tile* lhs, Tile* rhs)const {
                    return lhs->getFScore() > rhs->getFScore();
                }
            };
        };
    protected:
        static unordered_set<NavigationField2D*> navFields;
        Rect fieldRect;
        int layerNum = 1;
        int tileWidth = 0;
        int tileHeight = 0;
        vector<vector<Tile>> tileMap;
        vector<vector<vector<Tile>>> upperLayers;
        double minTileSize = 0;
        Tile* WorldLocactionToTile(const Vector2d& worldPosition);
    };
}
