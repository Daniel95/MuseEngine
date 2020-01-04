#pragma once

#include "Core/Utilities/Utilities.h"

#include <rttr/registration>

#include <string>

namespace Muse
{
    class Resource
    {
        RTTR_ENABLE();

    public:
        virtual ~Resource() = default;
        virtual void Reload() { }
        const std::string& GetPath() const { return m_Path; }
        const std::string& GetName() const { return m_Name; }
        void SetPathAndName(const std::string& a_Path);
        static ullong CalculateResourceId(const std::string& a_Path) { return StringHash(a_Path); }

    protected:
        std::string m_Name;
        std::string m_Path;
        bool m_IsInitialized = false;

    };
}