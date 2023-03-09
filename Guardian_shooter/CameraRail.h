#pragma once
#include "YunutyEngine.h"
#define _DEBUG

class CameraRail
	: public Component
{
public:
	static CameraRail* CreateCameraRail(const Vector2d& startPos, const Vector2d& endPos, const Vector2d& cameraProgressThreshold = Vector2d(100,100));
	static CameraRail* GetNearestRail(const Vector2d& targetPos);
	static Vector2d GetNearestPos(const Vector2d& targetPos);
private:
	Vector2d startVector = Vector2d();
	Vector2d railVector = Vector2d();
	double progress = 0;
	Vector2d cameraProgressThreshold = Vector2d();
	CameraRail* beforeRail = nullptr;
	CameraRail* nextRail = nullptr;
	static CameraRail* nowRail;
	static CameraRail* firstRail;

#ifdef _DEBUG
	GameObject* startArrow = nullptr;
	GameObject* endArrow = nullptr;
#endif

	friend class gs_map::MapTool;
	friend class gs_map::RailPoint;
	friend class GSCamera;
};

