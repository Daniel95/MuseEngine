#pragma once

#include "Core/Utilities/Utilities.h"

#include <rttr/registration>

#include <string>
#include <cereal/cereal.hpp>
#include <cereal/types/polymorphic.hpp>

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
        void InitPath(const std::string& a_Path);
        void UpdatePath(const std::string& a_Path);
        static ullong CalculateResourceId(const std::string& a_Path) { return StringHash(a_Path); }

        template <class Archive>
        void serialize(Archive& ar)
        {
            ar(m_Name, m_Path);
        }

    protected:
        std::string m_Name = "";
        std::string m_Path = "";

    private:
        static std::string ExtractName(const std::string& a_Path);

    };
}

CEREAL_REGISTER_TYPE(Muse::Resource)
