#include "MusePCH.h"
#include "SerializeTest.h"


RTTR_REGISTRATION
{
    rttr::registration::class_<SerializeTest>("SerializeTest")
        .constructor<>()
        (
            rttr::policy::ctor::as_raw_ptr
        )
    .property("m_testNumber", &SerializeTest::m_testNumber)
    .property("m_SerializePntrTest", &SerializeTest::m_SerializePntrTest);
}

void SerializeTest::Test()
{
    m_SerializePntrTest.reset(new SerializePntrTest());
    m_SerializePntrTest->Test();
    m_testNumber = 69;
}
