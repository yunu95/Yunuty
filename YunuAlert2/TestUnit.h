#include "YunutyEngine.h"

class TestUnit : public YunutyEngine::Component
{
private:
    float scalingDirection = 1;
protected:
    virtual void OnCollisionEnter2D(const Collision2D& collision);
    virtual void OnCollisionExit2D(const Collision2D& collision);
    virtual void Update() override;
public:
    float shakeSpeed = 2;
    float minSize = 0.9;
    float maxSize = 1.1;
};
