#pragma once
#include <fstream>
#include "MapObject.h"

namespace gs_map
{
    class MapTool;
    class TextInput : public Component
    {
    public:
        TextInput() { textInputs.insert(this); };
        virtual ~TextInput() { textInputs.erase(this); };
        bool isTyping=false;
        wstring typedInput;
        virtual void Update();
        static bool isInTypeMode() 
        {
            for (auto each : textInputs)
            {
                if (each->GetGameObject()->GetActive() && each->isTyping)
                    return true;
            }
            return false;
        }
    private:
        static unordered_set<TextInput*> textInputs;
    };
}
