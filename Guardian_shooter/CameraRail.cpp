#include "Guardian_shooter.h"
#include "Player.h"
#include "DebugObject.h"
#include "CameraRail.h"
#define _DEBUG
using namespace YunutyEngine;

CameraRail* CameraRail::nowRail = nullptr;
CameraRail* CameraRail::firstRail = nullptr;

CameraRail* CameraRail::CreateCameraRail(const Vector2d& startPos, const Vector2d& endPos, const Vector2d& cameraProgressThreshold)
{
	auto railObj = Scene::getCurrentScene()->AddGameObject(gs_map::MapTool::GetInstance()->GetRailLayer());
	railObj->GetTransform()->position = startPos + endPos / 2;
	auto cameraRail = railObj->AddComponent<CameraRail>();
	cameraRail->startVector = startPos;
	cameraRail->railVector = endPos - startPos;

	/// 레일의 각도에 따라 Threshold 를 조절
	/// 기존 Threshold 에서 화면 사이즈까지 선형적으로 보간함
	Rect resolution = GSCamera::GetInstance()->GetResolution();
	Vector2d maxThreshold(resolution.width / 2, resolution.height / 2);
	Vector2d basisX(1, 0);
	Vector2d basisY(0, 1);
	Vector2d resizeThreshold;
	resizeThreshold.x = maxThreshold.x + (cameraProgressThreshold.x - maxThreshold.x) * abs(Vector2d::Dot(basisX, cameraRail->railVector)) / cameraRail->railVector.Magnitude();
	resizeThreshold.y = maxThreshold.y + (cameraProgressThreshold.y- maxThreshold.y) * abs(Vector2d::Dot(basisY, cameraRail->railVector)) / cameraRail->railVector.Magnitude();
	cameraRail->cameraProgressThreshold = resizeThreshold;
#ifdef _DEBUG
	cameraRail->startArrow = Scene::getCurrentScene()->AddGameObject(railObj);
	cameraRail->endArrow = Scene::getCurrentScene()->AddGameObject(railObj);
	cameraRail->startArrow->GetTransform()->SetWorldPosition(startPos);
	cameraRail->endArrow->GetTransform()->SetWorldPosition(endPos);
	DebugObject::CreateArrow(cameraRail->startArrow, cameraRail->endArrow);
#endif
	return cameraRail;
}

CameraRail* CameraRail::GetNearestRail(const Vector2d& targetPos)
{
	CameraRail* rail = firstRail;
	Vector2d posDirection;
	map<double, CameraRail*> distance;
	while (rail)
	{
		posDirection = targetPos - rail->startVector;
		distance[Vector3d::Cross(rail->railVector, posDirection).Magnitude() / rail->railVector.Magnitude()] = rail;
		rail = rail->nextRail;
	}
	return (*distance.begin()).second;
}

Vector2d CameraRail::GetNearestPos(const Vector2d& targetPos)
{
	auto rail = GetNearestRail(targetPos);
	Vector2d posDirection = targetPos - rail->startVector;
	return rail->startVector + rail->railVector * abs(Vector2d::Dot(posDirection, rail->railVector) / rail->railVector.MagnitudeSqr());
}


