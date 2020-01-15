#include "MusePCH.h"

#include "ComponentTest.h"
#include "Core/Gameplay/GameObject.h"
#include "Core/Instrumentor.h"
#include <cereal/archives/json.hpp>
#include "CerealExample.h"

namespace Muse 
{
    void ComponentTest::Init(GameObject* a_GameObject)
    {
		MUSE_PROFILE_FUNCTION();

        m_GameObject = a_GameObject;

		if (m_isEnabled)
		{
			OnEnable();
		}
		else
		{
			OnDisable();
		}

		OnInit();


		{
			std::string a_FilePath = "assets/scenes/cerealComponent.txt";

			std::filesystem::path path{ a_FilePath }; //creates TestingFolder object on C:
			std::filesystem::create_directories(path.parent_path()); //add directories based on the object path (without this line it will not work)
			std::ifstream fs(path);

			{
				cereal::JSONInputArchive iarchive(fs);
				iarchive(cereal::make_nvp(a_FilePath, *this));
			}
			fs.close();
		}
    }

	void ComponentTest::Enable()
	{
		MUSE_PROFILE_FUNCTION();

		m_isEnabled = true;
        OnEnable();
	}

	void ComponentTest::Disable()
	{
		MUSE_PROFILE_FUNCTION();

		m_isEnabled = false;
        OnDisable();
	}

	std::shared_ptr<TransformComponent> ComponentTest::GetTransform() const
    {
		return m_GameObject->GetTransform();
    }
}
