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
        Resource(ullong a_ID, const std::string& a_Path);
        virtual ~Resource() = default;
        virtual void Reload();
        const std::string& Path() const { return m_Path; }
        static ullong CalculateResourceID(std::string a_Path) { return StringHash(a_Path); }

    protected:
        ullong m_ID;
        std::string m_Path;

    };
}