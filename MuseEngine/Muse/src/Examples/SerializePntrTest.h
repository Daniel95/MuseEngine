#pragma once

#include <rttr/registration>
#include <rttr/registration_friend.h>

class SerializePntrTest
{
    RTTR_ENABLE()
    RTTR_REGISTRATION_FRIEND

public:
    void Test();

private:
    float m_TestFloat = 99.9f;

};
