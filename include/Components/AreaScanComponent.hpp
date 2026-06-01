#pragma once
#include "context.hpp"
#include "Entity.hpp"
#include <SFML/Graphics.hpp>
#include "Components/VisibilityComponent.hpp"
#include "Components/ScanComponent.hpp"
#include "Components/RenderComponent.hpp"

class AreaScanComponent : public ScanComponent
{
    public:

    virtual void update(Context& context) override; 
};