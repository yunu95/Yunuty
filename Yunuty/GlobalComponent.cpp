#include "YunutyEngine.h"
#include "GlobalComponent.h"

using namespace YunutyEngine;

//std::map<GlobalComponent*, unique_ptr<GlobalComponent>> YunutyEngine::GlobalComponent::globalComponents;
std::unordered_set<GlobalComponent*> YunutyEngine::GlobalComponent::globalComponents;
GlobalComponent::GlobalComponent()
{
    globalComponents.insert(this);
    //globalComponents.insert(make_pair(this, unique_ptr<GlobalComponent>(this)));
}
GlobalComponent::~GlobalComponent()
{
}
