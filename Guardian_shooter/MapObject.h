#pragma once
#include "YunutyEngine.h"
namespace gs_map
{
    class MapTool;
    class MapObject : public Component
    {
    public:
        static void DeleteAllObjects();
        static void CallOnRestartAll();
        static void GlobalOnEngageMapEditorCallbacks();
        static void GlobalOnDisengageMapEditorCallbacks();
        virtual void OnRestart() {};
        virtual void OnEngagingMapEditMode() {};
        virtual void OnDisengagingMapEditMode() {};
        virtual void Invoke() {};
        MapObject() { mapObjects.insert(this); }
        ~MapObject() { mapObjects.erase(this); }

    private:
        static unordered_set<MapObject*> mapObjects;
        friend gs_map::MapTool;
    };
}
