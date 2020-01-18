#pragma once
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/string.hpp>

class Base
{
public:
    Base() = default;
    virtual ~Base() = default;

    std::string common;

    template <class Archive>
    void serialize(Archive& ar)
    {
        ar(
            cereal::make_nvp("common", common)
        );
    }
};

CEREAL_REGISTER_TYPE_WITH_NAME(Base, "Base")
