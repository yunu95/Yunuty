#include "YunutyEngine.h"

class ShakyFella : public YunutyEngine::Component
{
private:
    float scalingDirection = 1;
protected:
    virtual void OnCollisionEnter2D(const Collision2D& collision);
    virtual void OnCollisionExit2D(const Collision2D& collision);
    virtual void Update() override;
public:
    ~ShakyFella()
    {
        int a = 3;
    };
    float shakeSpeed = 2;
    float minSize = 0.9;
    float maxSize = 1.1;
};
