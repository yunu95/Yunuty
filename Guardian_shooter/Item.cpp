#include "Item.h"
#include "Player.h"
#include "Guardian_shooter.h"

unordered_set<Item*> Item::items;

Item::Item()
{
	items.insert(this);
}

Item::~Item()
{
	items.erase(this);
}

void Item::RemoveAllItems()
{
	for (auto each : items)
	{
		Scene::getCurrentScene()->DestroyGameObject(each->GetGameObject());
	}
}

void Item::Initialize(Vector3d pos, ItemType itemType, D2DAnimatedSprite* sprite)
{
	GetTransform()->SetWorldPosition(pos);
	this->itemType = itemType;
	itemImage = sprite;
	itemImage->SetWidth(120);
	itemImage->SetHeight(120);
	switch (itemType)
	{
		case ItemType::HealPack:
			itemImage->LoadAnimationFromFile(L"sprites/item/healpack");
			break;
		case ItemType::AttackSpeedUp:
			itemImage->LoadAnimationFromFile(L"sprites/item/forcepack");
			break;
		default:
			break;
	}
	itemImage->SetIsRepeating(false);
	moveTimer.duration = 0.7;
	moveTimer.onExpiration = [this]()
	{
		isMove = false;
		if (verticalDir == 1)
		{
			verticalDir = -1;
		}
		else
		{
			verticalDir = 1;
		}
	};
}

GameObject* Item::CreateItem(Vector3d _pos, ItemType _itemType)
{
	auto itemObject = Scene::getCurrentScene()->AddGameObject();
	auto collider = itemObject->AddComponent<BoxCollider2D>();
	collider->SetWidth(57);
	collider->SetHeight(110);
	itemObject->AddComponent<Item>()->itemCollider = collider;

	DebugObject::CreateColliderImage(collider);

	auto imageObj = Scene::getCurrentScene()->AddGameObject(itemObject);
	imageObj->GetTransform()->position = Vector3d(-1000, 0, 0);
	auto sprite = imageObj->AddComponent<D2DAnimatedSprite>();

	itemObject->GetComponent<Item>()->Initialize(_pos, _itemType, sprite);
	return itemObject;
}

void Item::Update()
{
	if (!isMove)
	{
		moveTimer.Start();
		isMove = true;
	}
	moveTimer.Update();
	if (isMove)
	{
		//rotation += -verticalDir * 0.35 * Time::GetDeltaTime();
		imagePos.y += verticalDir * 20 * Time::GetDeltaTime();
		itemImage->GetTransform()->position = imagePos;
	}
	if (!isOnGround)
		speed.y += -gravity * 100 * Time::GetDeltaTime();
	//GetTransform()->SetWorldRotation(Vector3d(0, 0, Vector2d(20 * cos(rotation), 20 * sin(rotation)).GetAngleDegree()));
	GetTransform()->SetWorldPosition(GetTransform()->GetWorldPosition() + speed * Time::GetDeltaTime());
}

void Item::OnCollisionEnter2D(const Collision2D& collision)
{
	auto ground = collision.m_OtherCollider->GetGameObject()->GetComponent<Ground>();
	if (ground != nullptr)
	{
		speed.y = 0;
		isOnGround = true;
	}

	auto player = collision.m_OtherCollider->GetGameObject()->GetComponent<Player>();
	if (player != nullptr)
	{
		switch (itemType)
		{
			case ItemType::HealPack:
				SoundSystem::PlaySoundfile("sounds/player/Player_itemget.wav");
				player->Heal(1.0f);
				Scene::getCurrentScene()->DestroyGameObject(this->GetGameObject());
				break;
			case ItemType::AttackSpeedUp:
				SoundSystem::PlaySoundfile("sounds/player/Player_itemget2.wav");
				player->AttackSpeedUp();
				Scene::getCurrentScene()->DestroyGameObject(this->GetGameObject());
				break;
			default:
				break;
		}
	}
}
