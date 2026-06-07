#include "Components/ProtectionSystemComponent.hpp"
#include "Components/ProtectComponent.hpp"
#include "Entity.hpp"
#include <iostream>

void ProtectionSystemComponent::update(Context& context)
{
    std::vector<Entity*> protectors;
    for (auto entity : context.getEntities())
    {
        auto protectComponent = entity->getComponent<ProtectComponent>();
        if (protectComponent && protectComponent->protectsOthers)
        {
            protectors.push_back(entity);
        }
    }

    for (auto entity : context.getEntities())
    {
        auto protectComponent = entity->getComponent<ProtectComponent>();
        if (!protectComponent) continue;

        if (protectComponent->protectsOthers)
        {
            protectComponent->isProtected = true;
            continue;
        }

        protectComponent->isProtected = false;
        for (auto protector : protectors)
        {
            auto protectorComp = protector->getComponent<ProtectComponent>();
            float dist = (entity->position - protector->position).length();
            if (dist <= protectorComp->protectRange)
            {
                protectComponent->isProtected = true;
                break;
            }
        }
    }
}
