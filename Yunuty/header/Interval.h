#pragma once

#ifdef YUNUTY_EXPORTS
#define YUNUTY_API __declspec(dllexport)
#else
#define YUNUTY_API __declspec(dllimport)
#endif

namespace YunutyEngine
{
    class YUNUTY_API Interval
    {
    public:
        Interval() = default;
        Interval(double centre, double radius);
        double left = 0;
        double right = 0;
        bool leftClosed = true;
        bool rightClosed = true;
        bool OverlappedWith(const Interval& other);
    };
}
