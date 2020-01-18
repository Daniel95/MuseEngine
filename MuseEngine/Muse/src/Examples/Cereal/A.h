#pragma once
#include "Base.h"

#include <cereal/types/polymorphic.hpp>

class A : public Base
{
public:
    A() = default;

    A(int v)
    {
        a = v;
    }

    int a;

    template <class Archive>
    void serialize(Archive& ar)
    {
        ar(cereal::make_nvp("Base", cereal::base_class<Base>(this)));
        ar(a);
    }
};

CEREAL_REGISTER_TYPE(A)

CEREAL_REGISTER_POLYMORPHIC_RELATION(Base, A)