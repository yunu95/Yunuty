#pragma once
#ifdef YUNUTY_EXPORTS
#define YUNUTY_API __declspec(dllexport)
#else
#define YUNUTY_API __declspec(dllimport)
#endif

namespace YunutyEngine
{
    namespace YunutyMath
    {
        YUNUTY_API extern const double PI;
        YUNUTY_API extern const double Deg2Rad;
        YUNUTY_API extern const double Rad2Deg;
        template<typename real>
        real Min(real a,real b) 
        {
            return a < b ? a : b;
        }
        template<typename real>
        real Max(real a,real b) 
        {
            return a > b ? a : b;
        }
    };
}
