#include "Components/AttackComponent.hpp"
#include "Components/AudioComponent.hpp"
#include "Components/AudioSystemComponent.hpp"

void AttackComponent::update(Context& context)
{
    auto scanComponent = owner->getComponent<ScanComponent>();

    if(scanComponent != nullptr)
    {
        bool isAttacking = attackDerived(context, scanComponent->getCollection());
        if(isAttacking)
        {
            if(auto* audio = owner->getComponent<AudioComponent>())
            {
                audio->playSound(context, SoundEvent::AttackSound, gunshotVolume);
                if(!wasAttacking)
                    audio->playVoiceline(SoundEvent::Attack, attackVoiceVolume);
        }
        }
        wasAttacking = isAttacking;
    }
}
