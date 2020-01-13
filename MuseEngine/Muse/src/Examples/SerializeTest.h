#pragma once
#include <rttr/registration>
#include <rttr/registration_friend.h>
#include "SerializePntrTest.h"

#include <memory>

class SerializeTest
{
    RTTR_ENABLE()
    RTTR_REGISTRATION_FRIEND

public:
    int GetTestNumber() const { return m_testNumber; }
    void SetTestNumber(int a_TestNumber) { m_testNumber = a_TestNumber; }

    void Test();

private:
    int m_testNumber = 3;
    std::shared_ptr<SerializePntrTest> m_SerializePntrTest;

};
