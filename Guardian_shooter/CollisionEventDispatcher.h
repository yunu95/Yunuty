#pragma once
#include "YunutyEngine.h"

class CollisionEventDispatcher : public Component
{
public:
	Component* target = nullptr;

protected:
	virtual void OnCollisionEnter2D(const Collision2D& collision) override;
	virtual void OnCollisionStay2D(const Collision2D& collision) override;
	virtual void OnCollisionExit2D(const Collision2D& collision) override;

private:
};