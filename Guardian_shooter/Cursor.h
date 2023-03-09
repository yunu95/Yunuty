#pragma once
#include "YunutyEngine.h"
#include "Button.h"
#include "Timer.h"

namespace gs_map
{
    class MapTool;
}

class Cursor :
    public Component
{
public:
    Cursor();
    virtual ~Cursor();
    static Cursor* CreateCursor();
    static bool touchingNothing;
    static void ReportDead(Button* button);
protected:
    virtual void Update();
    virtual void OnCollisionEnter2D(const Collision2D& collision);
    virtual void OnCollisionStay2D(const Collision2D& collision);
    virtual void OnCollisionExit2D(const Collision2D& collision);
private:
    static Cursor* instance;
    Timer dragStartTimer;
    Button* interactingButton = nullptr;
    unordered_set<Button*> buttonList;
};
