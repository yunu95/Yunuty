#pragma once
#include <random>

#ifdef YUNUTY_EXPORTS
#define YUNUTY_API __declspec(dllexport)
#else
#define YUNUTY_API __declspec(dllimport)
#endif

using namespace std;

namespace YunutyEngine
{
    namespace YunutyMath
    {
        class YUNUTY_API Random
        {
        public:
            static int GetRandomInt(int min,int max);
        private:
            Random();
            static Random* GetInstance();
            static Random* instance;
            random_device rd;
            mt19937 gen;
        };
    }
}
