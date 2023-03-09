#include "MapObject.h"

using namespace gs_map;

void MapObject::DeleteAllObjects()
{
    for (auto each : MapObject::mapObjects)
        Scene::getCurrentScene()->DestroyGameObject(each->GetGameObject());
}
void MapObject::CallOnRestartAll()
{
    for (auto each : mapObjects)
    {
        each->OnRestart();
    }
}
void MapObject::GlobalOnEngageMapEditorCallbacks()
{
    for (auto each : mapObjects)
    {
        each->OnEngagingMapEditMode();
    }
}
void MapObject::GlobalOnDisengageMapEditorCallbacks()
{
    for (auto each : mapObjects)
    {
        each->OnDisengagingMapEditMode();
    }
}

unordered_set<MapObject*> MapObject::mapObjects = unordered_set<MapObject*>();
