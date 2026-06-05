#include "Components/ProtectionSystemComponent.hpp"
#include "Components/ProtectComponent.hpp"
#include "Entity.hpp"

void ProtectionSystemComponent::update(Context& context)
{
    std::vector<Entity*> protectors;
    for (auto entity : context.getEntities())
    {
        auto pc = entity->getComponent<ProtectComponent>();
        if (pc && pc->protectsOthers)
        {
            protectors.push_back(entity);
        }
    }

    for (auto entity : context.getEntities())
    {
        auto pc = entity->getComponent<ProtectComponent>();
        if (!pc) continue;

        if (pc->protectsOthers)
        {
            pc->isProtected = true;
            continue;
        }

        pc->isProtected = false;
        for (auto protector : protectors)
        {
            auto protectorComp = protector->getComponent<ProtectComponent>();
            float dist = (entity->position - protector->position).length();
            if (dist <= protectorComp->protectRange)
            {
                pc->isProtected = true;
                break;
            }
        }
    }
}
