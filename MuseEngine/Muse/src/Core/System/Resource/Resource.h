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
        virtual void Reload();
        const std::string& GetPath() const { return m_Path; }
        void SetPath(const std::string& a_Path) { m_Path = a_Path; }
        static ullong CalculateResourceId(const std::string& a_Path) { return StringHash(a_Path); }

    protected:
        std::string m_Path;

    };
}