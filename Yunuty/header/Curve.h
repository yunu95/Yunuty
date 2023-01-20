#pragma once
#include <vector>
#include <functional>

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
        // ���׽��� ���� 2���� ������ ��׷����� �׸� �� �ִ� Ŀ�긦 ��Ÿ���ϴ�.
        class YUNUTY_API Curve
        {
        private:
            // curveFunction�� ���� Evaluate �Լ��� ��ȯ ���� �޶����ϴ�.
            function<double(double)> curveFunction;
        public:
            Curve() :Curve([](double t) {return t; }) {}
            Curve(const function<double(double)> curveFunction) :curveFunction(curveFunction) {}
            double Evaluate(double t) const { return curveFunction(t); }
        };
        // Linear : ������ y=x
        extern const Curve Linear;
        // �ӵ��� �������ٰ� ���ڱ� ��ü�� ���ߴ� ����� ǥ���ϱ⿡ ������ Ŀ���Դϴ�.
        extern const Curve EaseInQuad;
        // �ʱ�ӵ��� ���� ���¿��� �ӵ��� �پ��� ������ ��ü�� ���ߴ� ����� ǥ���ϱ⿡ ������ Ŀ���Դϴ�.
        extern const Curve EaseOutQuad;
        // �ʱ�ӵ��� �������ٰ� �ٽ� �ӵ��� �پ��� ������ ��ü�� ���ߴ� ����� ǥ���ϱ⿡ ������ Ŀ���Դϴ�. 
        extern const Curve easeInOutQuad;
    };
}
