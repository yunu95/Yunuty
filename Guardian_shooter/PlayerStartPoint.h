#pragma once
#include <fstream>
#include "MapObject.h"

namespace gs_map
{
	class MapTool;
	class PlayerStartPoint :
		public MapObject
	{
	public:
		friend class MapTool;

		PlayerStartPoint() { instance = this; }
		virtual ~PlayerStartPoint() { }
        // 파일 입출력 로직
        static void Save(wstring mapRoot);
        static void Load(wstring mapRoot);
		static PlayerStartPoint* GetInstance() { return instance; }
		static PlayerStartPoint* CreatePlayerStartPoint(Vector2d location);
        virtual void OnEngagingMapEditMode();
        virtual void OnDisengagingMapEditMode();
	protected:
		virtual void Update() override;
		virtual void OnRestart() override;
	private:
		static PlayerStartPoint* instance;
		static ifstream fin;
		static ofstream fout;
	};
}
