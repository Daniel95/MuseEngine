#pragma once

#include "Core/Utilities/Log.h"

#include <cmath>
#include <rttr/registration>
#include <sstream>
#include <string>

using namespace rttr;

static void Intro()
{
    LOG_ENGINE_INFO("Hello World");
}

static void Log(std::string a_LogString)
{
    LOG_ENGINE_INFO(a_LogString);
}

struct BaseStruct
{
    RTTR_ENABLE();
public:

    BaseStruct() { }; 
    void Func(int a_Param) { m_Data += a_Param; }; 
    
    int m_Data = 1;

};

struct BasicStruct
{
    RTTR_ENABLE();
public:
    bool m_On = true;

};

struct DerivedStruct : public BaseStruct
{
    RTTR_ENABLE(BaseStruct);
public:

    DerivedStruct() { };
    void Func2(int a_Param)
    {
        m_Data *= a_Param;
        m_Data2 = a_Param;
    };

    int m_Data2;
};

struct AnotherDerivedStruct : DerivedStruct, BasicStruct
{
    RTTR_ENABLE(DerivedStruct, BasicStruct);
public:
    std::string m_Word = "Hi";

};

RTTR_REGISTRATION
{
    registration::method("Intro", &Intro);
    registration::method("Log", &Log);

    registration::method("pow", select_overload<float(float, float)>(&pow))
                    .method("pow", select_overload<double(double, double)>(&pow));

    registration::class_<BaseStruct>("BaseStruct")
                    .constructor<>()
                    (
                        policy::ctor::as_raw_ptr // By default it is rttr::policy::ctor::as_shared_ptr
                    )
                    .property("Data", &BaseStruct::m_Data)
                    .method("Func", &BaseStruct::Func);

    registration::class_<DerivedStruct>("DerivedStruct")
                    .constructor<>()
                    (
                        policy::ctor::as_object
                    )
                    .property("Data2", &DerivedStruct::m_Data2)
                    .method("Func2", &DerivedStruct::Func2);

    // Register external classes in a seperate header specifically for this
}

void TestRTTR()
{
    // Recycling variables
    std::ostringstream stringStream;

    // Test Hello World
    type::invoke("Intro", {});
    // End Test

    // Test overloaded global functions
    float tempPowFloatInputP = 2.f;
    float tempPowFloatInputW = 3.f;
    float tempPowFloatOutput = 0.f;
    double tempPowDoubleInputP = 4.f;
    double tempPowDoubleInputW = 2.f;
    double tempPowDoubleOutput = 0.f;

    variant float_return_value = type::invoke("pow", {tempPowFloatInputP, tempPowFloatInputW});
    if (float_return_value.is_valid() && float_return_value.is_type<float>())
    {
        tempPowFloatOutput = float_return_value.get_value<float>();
    }

    method meth = type::get_global_method("pow", {type::get<double>(), type::get<double>()});
    if (meth)
    {
        auto params = meth.get_parameter_infos();
        for (parameter_info paramInfo : params)
        {
            std::string paramType = paramInfo.get_type().get_name().to_string();
            std::string logString = "Type: " + paramType;
            type::invoke("Log", {logString});
        }

        variant double_return_value = meth.invoke({}, tempPowDoubleInputP, tempPowDoubleInputW);
        if (double_return_value.is_valid() && double_return_value.is_type<double>())
        {
            tempPowDoubleOutput = double_return_value.get_value<double>();
        }
    }

    stringStream.clear();
    stringStream << "Float Pow Test.\nInput: " << tempPowFloatInputP << " & " << tempPowFloatInputW << ".\nOutput: " << tempPowFloatOutput << ".\n";
    stringStream << "Double Pow Test.\nInput: " << tempPowDoubleInputP << " & " << tempPowDoubleInputW << ".\nOutput: " << tempPowDoubleOutput << ".";
    type::invoke("Log", {stringStream.str()});
    // End Test

    // Test rttr_cast
    AnotherDerivedStruct ads = AnotherDerivedStruct();
    BaseStruct* base = &ads;
    BasicStruct* basic = rttr_cast<BasicStruct*>(base); // Even though base & basic have nothing with each other, ads inherits both from them, so this cast is succesfull
    std::string text = basic->m_On ? "It is on!" : "It is not on :(";
    type::invoke("Log", {text});
    // End Test

    // Test creating classes and iterate over them / invoke functions
    DerivedStruct derived = DerivedStruct();
    base = &derived;
    type rttrType = type::get(base);

    stringStream.str("");
    stringStream.clear();
    stringStream << "Name: " << rttrType.get_name().to_string() << ", Is a pointer: " << (rttrType.is_pointer() ? "Yess" : "Nooo...") << ", other info to fill in: " << 0 << ".";
    type::invoke("Log", {stringStream.str()});
    // End Test
}

void TestSerialization(Muse::Application& a_Application)
{
    /*
    Engine::Scene* scene = new Engine::Scene(a_Application);
    Engine::GameObject* gameObject = new Engine::GameObject(*scene);
    gameObject->AddComponent<Engine::BoxCollider2D>();
    Engine::TransformComponent* transform = gameObject->GetComponent<Engine::TransformComponent>();

    transform->Move(DXS::Vector2(12, 0));

    std::string jsonString;
    jsonString = io::to_json(gameObject);
    LOG_APP_INFO("JSON: {0}", jsonString);
    */
}
