#include "Components/ResourceManagerComponent.hpp"

ResourceManagerComponent::ResourceManagerComponent()
{
    onAddMetal = [this]() { metal += 10; };
    onAddFood = [this]() { food += 10; };
    onAddPeople = [this]() { people += 10; };
}

void ResourceManagerComponent::update(Context& context)
{
}
