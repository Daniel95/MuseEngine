#include "MusePCH.h"
#include "SerializePntrTest.h"


RTTR_REGISTRATION
{
    rttr::registration::class_<SerializePntrTest>("SerializePntrTest")
        .constructor<>()
        (
            rttr::policy::ctor::as_std_shared_ptr
        )
    .property("m_TestFloat", &SerializePntrTest::m_TestFloat);
}

void SerializePntrTest::Test()
{
    m_TestFloat = 100.1f;
}
