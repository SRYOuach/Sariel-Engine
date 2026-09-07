#pragma once

namespace sariel
{
    class Engine;

    class EngineKey
    {
        friend class Engine;
        EngineKey() = default;

    public:
        EngineKey(const EngineKey &) = default;
    };
}