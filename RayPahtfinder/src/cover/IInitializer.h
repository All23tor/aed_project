#pragma once

class IInitializer {
public:
    virtual void Initialize() = 0;
    virtual ~IInitializer() = default;
};