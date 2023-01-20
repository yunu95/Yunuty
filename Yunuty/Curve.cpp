#include "Curve.h"

// Linear : ������ y=x
const YunutyEngine::YunutyMath::Curve YunutyEngine::YunutyMath::Linear =
YunutyEngine::YunutyMath::Curve([](double t) {return t; });
// �ӵ��� �������ٰ� ���ڱ� ��ü�� ���ߴ� ����� ǥ���ϱ⿡ ������ Ŀ���Դϴ�.
const YunutyEngine::YunutyMath::Curve YunutyEngine::YunutyMath::EaseInQuad =
YunutyEngine::YunutyMath::Curve([](double t) {return t * t; });
// �ʱ�ӵ��� ���� ���¿��� �ӵ��� �پ��� ������ ��ü�� ���ߴ� ����� ǥ���ϱ⿡ ������ Ŀ���Դϴ�.
const YunutyEngine::YunutyMath::Curve YunutyEngine::YunutyMath::EaseOutQuad =
YunutyEngine::YunutyMath::Curve([](double t) {return 1 - (1 - t) * (1 - t); });
// �ʱ�ӵ��� �������ٰ� �ٽ� �ӵ��� �پ��� ������ ��ü�� ���ߴ� ����� ǥ���ϱ⿡ ������ Ŀ���Դϴ�. 
const YunutyEngine::YunutyMath::Curve YunutyEngine::YunutyMath::easeInOutQuad =
YunutyEngine::YunutyMath::Curve([](double t) {return  t < 0.5 ? 2 * t * t : 1 - (-2 * t + 2, 2) * (-2 * t + 2, 2) / 2; });
