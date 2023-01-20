#pragma once
#include "YunutyEngine.h"

using namespace std;
using namespace YunutyEngine;

class DebugTile;
class DebugTilePool :public GameObjectPool<DebugTile>
{
public:
    static DebugTilePool* instance;
    DebugTilePool();
    DebugTile* Borrow(Vector2d position, double tileSize, D2D1::ColorF color);
private:
    static void Initialize(GameObject* gameObject, DebugTile*);

};

