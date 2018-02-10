#pragma once
#include <vector>

typedef uint8 SystemPriority;
static const SystemPriority PRIORITY_LAST = 9;
static const SystemPriority PRIORITY_SHIELD_REINFORCEMENT = 9;
static const SystemPriority PRIORITY_LOADING_WEAPONS = 8;
static const SystemPriority PRIORITY_MOVEMENT = 7;
static const SystemPriority PRIORITY_HOLDING_WEAPONS = 6;

class ISFCPowerConsumer {
    virtual float ConsumePower(SystemPriority Priority, float AvailablePower, float TurnFraction) = 0;
};

class ISFCPoweredSystem {
public:

    virtual std::vector<ISFCPowerConsumer*> GetPowerConsumers() = 0;
};

