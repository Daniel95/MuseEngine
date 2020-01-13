#include <string>
#include "Core/Utilities/json/from_json.h"
#include "Core/Utilities/json/to_json.h"

#include <rttr/registration>
#include <rttr/registration_friend.h>
#include "Core/Utilities/Log.h"
#include <iostream>
#include "SerializeTest.h"

void TestSerialization()
{
    SerializeTest* serializeObjectTest1 = new SerializeTest();

    serializeObjectTest1->Test();

    SerializeTest* serializeObjectTest2;


    std::string string = io::to_json(serializeObjectTest1);

    std::cout << string << std::endl;

    //LOG_ENGINE_INFO("serialized: {0}", string);


    io::from_json(string, serializeObjectTest2);

    serializeObjectTest2;
    //LOG_ENGINE_INFO("Circle c_2: {0}", io::to_json(c_2));

    string;
}