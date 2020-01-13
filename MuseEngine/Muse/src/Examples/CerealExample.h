#pragma once

#include <cereal/archives/binary.hpp>
#include <sstream>

struct MyClass
{
    int x, y, z;

    // This method lets cereal know which data members to serialize
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(x, y, z); // serialize things by passing them to the archive
    }
};

void TestSerializationCereal()
{
    std::stringstream ss; // any stream can be used

    {
        cereal::BinaryOutputArchive oarchive(ss); // Create an output archive

        MyClass m1, m2, m3;
        oarchive(m1, m2, m3); // Write the data to the archive
    } // archive goes out of scope, ensuring all contents are flushed

    {
        cereal::BinaryInputArchive iarchive(ss); // Create an input archive

        MyClass m1, m2, m3;
        iarchive(m1, m2, m3); // Read the data from the archive
    }
}