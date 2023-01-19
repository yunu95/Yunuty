#include "StatusText.h"

void StatusText::Update()
{
    wstringstream str;
    str << (int)D2DInput::getMouseWorldPosition().x << L"," << (int)D2DInput::getMouseWorldPosition().y;
    text = str.str();
}
