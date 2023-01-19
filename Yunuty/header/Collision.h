#pragma once
#include <thread>
#include "Object.h"
#include <functional>

#ifdef YUNUTY_EXPORTS
#define YUNUTY_API __declspec(dllexport)
#else
#define YUNUTY_API __declspec(dllimport)
#endif

using namespace std;

namespace YunutyEngine
{
    class YUNUTY_API Collision
    {
    };
}
