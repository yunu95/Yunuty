#pragma once
#include "D2DGraphic.h"
#include "YunutyEngine.h"

#ifdef YUNUTY_EXPORTS
#define YUNUTY_API __declspec(dllexport)
#else
#define YUNUTY_API __declspec(dllimport)
#endif

using namespace YunutyEngine::D2D;

namespace YunutyEngine
{
    namespace D2D
    {
        typedef vector<pair<double, wstring>> SpriteAnim;
        class YUNUTY_API D2DAnimatedSprite : public D2DGraphic
        {
        public:
            void SetIsRepeating(bool repeating) { this->isRepeating = repeating; }
            bool GetIsRepeating() { return this->isRepeating; }
            void LoadAnimationFromFile(wstring folderName,double interval=0.1);
            void Play();
        protected:
            virtual void Update() override;
            virtual void Render(D2D1::Matrix3x2F transform) override;
            static const SpriteAnim* LoadAnimation(wstring folderName, double interval = 0.1);
            void SetAnimation(const SpriteAnim* animation);
            const SpriteAnim* GetAnimSprites() const;
        private:
            bool isRepeating = true;
            const SpriteAnim* animSprites = nullptr;
            int index = 0;
            double elapsed = 0;
            static unordered_map<wstring, vector<pair<double, wstring>>> cachedAnims;
        };
    }
}
