#pragma once
#include "YunutyEngine.h"
#include "Timer.h"
#include "MapTool.h"

namespace gs_map
{
	class EnemyWave;
	class CameraRailPoint;
}
class Threat;

class GSCamera : public D2DCamera
{
public:
	static GSCamera* GetInstance() { return instance; }
	GSCamera() {
		if (!instance)instance = this;
		recoveryTimer.duration = 3;
		recoveryTimer.onUpdate = [this](double normT)
		{
			if(!gs_map::MapTool::GetInstance()->isEditingMap() && !lockState)
				GetTransform()->SetWorldPosition(Vector2d::Lerp(lastCamPoint, optimalPosition, normT));
			if (!gs_map::MapTool::GetInstance()->isEditingMap() && lockState)
				GetTransform()->SetWorldPosition(Vector2d::Lerp(lastCamPoint, lockPosition, normT));
		};
	}
	~GSCamera() { if (instance)instance = nullptr; }
	void Initialize();
	void Restart();
	void LerpToRailPoint();
	void LerpToWaveCamera(gs_map::EnemyWave* wave);
	void SetCameraPosToRail();
	void LockPosition(const Vector3d& lockPos);
	void UnLockPosition();
	Vector2d GetOptimalPosition();
	bool isRecovering() { return recoveryTimer.isActive; };
protected:
	virtual void Update() override;
	virtual void OnCollisionStay2D(const Collision2D& collision)override;
private:
	unordered_map<Threat*,double> threatsLegacy;
	Timer recoveryTimer;
	Vector2d lastCamPoint = Vector2d::zero;
	Vector2d optimalPosition = Vector2d::zero;
	Vector2d lockPosition = Vector2d::zero;
	bool lockState = false;
	static GSCamera* instance;
};