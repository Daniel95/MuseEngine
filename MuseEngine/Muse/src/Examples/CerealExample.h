#pragma once

#include <cereal/types/unordered_map.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <fstream>

enum class CollisionType { None, Dynamic, Static };

struct MyRecord
{
    uint8_t x, y;
    float z;
    CollisionType m_CollisionType;



    template <class Archive>
    void serialize(Archive& ar)
    {
        ar(x, y, z, m_CollisionType);
    }
};

class GameObject;
class BoxCollider2D;
class TransformComponent;

class ComponentTest
{
public:
    ComponentTest() = default;
    virtual ~ComponentTest() = default;

    void Init(GameObject* a_GameObject);
    void Update(float a_DeltaTime) { OnUpdate(a_DeltaTime); }
    void FixedUpdate() { OnFixedUpdate(); }
    void Enable();
    void Disable();

    GameObject* GetGameObject() const { return m_GameObject; }
    TransformComponent* GetTransform() const;
    bool IsEnabled() const { return m_isEnabled; };
    //template<typename T>
    //T* GetComponent() { return m_GameObject->GetComponent<T>(); }

protected:
    virtual void OnUpdate(float a_DeltaTime) {}
    virtual void OnInit() {}
    virtual void OnFixedUpdate() {}
    virtual void OnEnable() {}
    virtual void OnDisable() {}

private:
    GameObject* m_GameObject = nullptr;
    bool m_isEnabled = true;

};

struct SomeData
{
    int32_t id;
    std::shared_ptr<std::unordered_map<uint32_t, MyRecord>> data;

    float test = 43;

    MyRecord* m_record;

    void Test(MyRecord* a_MyRecord)
    {
        m_record = a_MyRecord;
    }


    template <class Archive>
    void serialize(Archive& ar)
    {
        ar(id, data, test);
    }

    void SaveToFile()
    {
        std::ofstream fs;
        std::string path = "assets/scenes/cereal.txt";
        fs.open(path);
        {
            cereal::JSONOutputArchive oarchive(fs);
            oarchive(cereal::make_nvp("Gameplay", *this));
        }
        fs.close();
    }

    void LoadFromFile()
    {
        std::ifstream fs;
        const std::string path = "assets/scenes/cereal.txt";
        fs.open(path);

        if (fs.is_open())
        {
            cereal::JSONInputArchive iarchive(fs);
            iarchive(cereal::make_nvp("Gameplay", *this));
        }
        fs.close();
    }
};

void TestSerializationCereal()
{
    SomeData someData1;
    someData1.test = 99;
    someData1.SaveToFile();

    SomeData someData2;
    someData2.LoadFromFile();
    someData2;
}

