#pragma once

#include <memory>

struct Referenced
{
    int test1 = 324;
    int test2 = 4;
    int test3 = 12;
};

class Owner
{
public:
    Owner()
    {
        uniquePtr = std::make_unique<Referenced>();
        sharedPtr = std::make_unique<Referenced>();
    }
    std::shared_ptr<Referenced> GetShared() { return sharedPtr; }

private:
    std::unique_ptr<Referenced> uniquePtr;
    std::shared_ptr<Referenced> sharedPtr;

};

void TestSmartPointer()
{
    
}