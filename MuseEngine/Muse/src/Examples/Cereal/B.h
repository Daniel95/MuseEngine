#pragma once
#include "Base.h"

#include <cereal/types/polymorphic.hpp>

class B : public Base
{
public:
    B() = default;

    B(std::string text)
    {
        b = text;
    }

    std::string b;

    template <class Archive>
    void serialize(Archive& ar)
    {
        ar(cereal::make_nvp("Base", cereal::base_class<Base>(this)));
        ar(b);
    }
};

CEREAL_REGISTER_TYPE_WITH_NAME(B, "ClassB")

CEREAL_REGISTER_POLYMORPHIC_RELATION(Base, B)