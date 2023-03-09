#pragma once
#include <fstream>
#include "MapObject.h"
#include "CameraRail.h"

namespace gs_map
{
	class MapTool;
	class RailPoint :
		public MapObject
	{
	public:
		friend MapTool;

		RailPoint() { railPoints.insert(this); }
		~RailPoint() { railPoints.erase(this); }

		// 파일 입출력 로직
		static void Save(wstring mapRoot);
		static void Load(wstring mapRoot);
		static RailPoint* CreateRailPoint(Vector2d location, RailPoint* previous = nullptr);
		static void DeleteRailPoint(RailPoint* point);
	protected:

	private:
		static CameraRail* CreateCameraRail(RailPoint* rp1, RailPoint* rp2);
		static void AdjustRails(RailPoint* rp1);

		RailPoint* previous = nullptr;
		RailPoint* next = nullptr;
		CameraRail* beforeRail = nullptr;
		CameraRail* nextRail = nullptr;
		static unordered_set<RailPoint*> railPoints;
		static RailPoint* extendStartPoint;
		static RailPoint* firstPoint;
	};
}
