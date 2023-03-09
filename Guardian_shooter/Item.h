#pragma once
#include "YunutyEngine.h"
#include "Timer.h"
#include "unordered_set"

enum class ItemType
{
	HealPack,
	AttackSpeedUp,
};

class Item : public Component
{
public:
	Item();
	~Item();
	static void RemoveAllItems();
	void Initialize(Vector3d pos, ItemType itemType, D2DAnimatedSprite* sprite);
	static GameObject* CreateItem(Vector3d _pos, ItemType _itemType);
protected:
	virtual void Update() override;
	virtual void OnCollisionEnter2D(const Collision2D& collision) override;

private:
	Vector3d speed;
	const double gravity = 9.8;
	//double rotation = 0;
	ItemType itemType;
	//GameObject* imageObject;
	BoxCollider2D* itemCollider;
	D2DAnimatedSprite* itemImage;
	Vector3d imagePos;
	Timer moveTimer;
	bool isMove = false;
	bool isOnGround = false;
	char verticalDir = 1;
	static unordered_set<Item*> items;
};

