#include "Components/ResourceComponent.hpp"
#include "Components/ResourceManager.hpp"
#include "context.hpp"

void ResourceComponent::update(Context& context)
{
    if (manager == nullptr)
        manager = context.resourceManager;
}
