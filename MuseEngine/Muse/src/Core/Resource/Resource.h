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
            ar(test);
        }

        virtual bool operator==(const Resource a_Resource) const
        {
            ASSERT_ENGINE(!m_Path.empty() && !a_Resource.GetPath().empty(), "Resource path cannot be empty when comparing!");
            return m_Path == a_Resource.GetPath();
        }

    protected:
        std::string m_Name = "";
        std::string m_Path = "";
        int test = 0;

    private:
        static std::string ExtractName(const std::string& a_Path);

    };
}

CEREAL_REGISTER_TYPE(Muse::Resource)
