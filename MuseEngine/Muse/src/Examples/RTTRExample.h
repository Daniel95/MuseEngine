#include <string>
#include "Core/Utilities/json/from_json.h"
#include "Core/Utilities/json/to_json.h"

#include <rttr/registration>
#include "Core/Utilities/Log.h"
#include <iostream>

class SerializeTest
{
    RTTR_ENABLE()
public:
    int GetTestNumber() const { return m_testNumber; }
    void SetTestNumber(int a_TestNumber) { m_testNumber = a_TestNumber; }

    //RTTR_REGISTRATION_FRIEND

private:

    int m_testNumber = 3;
};


RTTR_REGISTRATION
{
    rttr::registration::class_<SerializeTest>("SerializeTest")
        .constructor<>()
        (
            rttr::policy::ctor::as_raw_ptr
        )
    .property("test", &SerializeTest::GetTestNumber, &SerializeTest::SetTestNumber);
//.property("test", &SerializeObjectTest::m_testNumber);
}

void TestSerialization()
{
    SerializeTest serializeObjectTest1;

    serializeObjectTest1.SetTestNumber(9);
    SerializeTest serializeObjectTest2;


    std::string string = io::to_json(serializeObjectTest1);

    std::cout << string << std::endl;

    //LOG_ENGINE_INFO("serialized: {0}", string);


    io::from_json(string, serializeObjectTest2);


    //LOG_ENGINE_INFO("Circle c_2: {0}", io::to_json(c_2));

    string;
}