#include "YunutyEngine.h"
#include "YunuAlert2.h"
#include "DebugTilePool.h"

DebugTilePool* DebugTilePool::instance = nullptr;
DebugTilePool::DebugTilePool()
{
    SetObjectInitializer(Initialize);
    if (instance == nullptr)
        instance = this;
}
DebugTile* DebugTilePool::Borrow(Vector2d position, double tileSize, D2D1::ColorF color)
{
    auto item = GameObjectPool<DebugTile>::Borrow();
    item->GetTransform()->SetWorldPosition(position);
    item->rectImage->width = tileSize;
    item->rectImage->height = tileSize;
    item->rectImage->color = color;
    item->rectImage->color.a = 0;
    item->StartGlowing();
    return item;
}
void DebugTilePool::Initialize(GameObject* gameObject, DebugTile* tile)
{
    tile->rectImage = gameObject->AddComponent<D2DRectangle>();
}
