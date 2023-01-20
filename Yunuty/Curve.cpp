#include "Curve.h"

// Linear : 일직선 y=x
const YunutyEngine::YunutyMath::Curve YunutyEngine::YunutyMath::Linear =
YunutyEngine::YunutyMath::Curve([](double t) {return t; });
// 속도가 빨라지다가 갑자기 물체가 멈추는 모습을 표현하기에 적절한 커브입니다.
const YunutyEngine::YunutyMath::Curve YunutyEngine::YunutyMath::EaseInQuad =
YunutyEngine::YunutyMath::Curve([](double t) {return t * t; });
// 초기속도가 빠른 상태에서 속도가 줄어들며 서서히 물체가 멈추는 모습을 표현하기에 적절한 커브입니다.
const YunutyEngine::YunutyMath::Curve YunutyEngine::YunutyMath::EaseOutQuad =
YunutyEngine::YunutyMath::Curve([](double t) {return 1 - (1 - t) * (1 - t); });
// 초기속도가 빨라지다가 다시 속도가 줄어들며 서서히 물체가 멈추는 모습을 표현하기에 적절한 커브입니다. 
const YunutyEngine::YunutyMath::Curve YunutyEngine::YunutyMath::easeInOutQuad =
YunutyEngine::YunutyMath::Curve([](double t) {return  t < 0.5 ? 2 * t * t : 1 - (-2 * t + 2, 2) * (-2 * t + 2, 2) / 2; });
