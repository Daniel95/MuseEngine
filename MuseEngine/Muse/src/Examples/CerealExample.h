#pragma once

#include <cereal/types/unordered_map.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <fstream>
#include <cereal/types/vector.hpp>

#include <vector>
#include <memory>

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

struct SomeData
{
    int32_t id;
    //std::shared_ptr<std::unordered_map<uint32_t, MyRecord>> data;
    std::vector<std::shared_ptr<MyRecord>> data;
    //std::vector<std::shared_ptr<int>> data;

    float test = 43;

    std::shared_ptr<MyRecord> m_record;

    template <class Archive>
    void serialize(Archive& ar)
    {
        ar(data);
    }

    void SaveToFile()
    {
        std::ofstream fs;
        std::string path = "assets/scenes/cereal.txt";
        fs.open(path);
        {
            cereal::JSONOutputArchive oarchive(fs);
            oarchive(cereal::make_nvp("Test", *this));
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

