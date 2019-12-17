#include "MusePCH.h"
#include "Event.h"

namespace Muse
{
	Event::Event()
	{
	}

	Event::~Event()
	{
	}

	void Event::Subscribe(void* idPtr, const std::function<void()>& function)
	{
		ullong id = PointerToHash(idPtr);

		ASSERT_ENGINE(subscriptions.count(id) == 0, "Cannot Subscribe: This ID is already subscribed to this event!");

		subscriptions[id] = function;
	}

	void Event::Unsubscribe(void* idPtr)
	{
		ullong id = PointerToHash(idPtr);

		ASSERT_ENGINE(subscriptions.count(id) != 0, "Cannot Unsubscribe: This ID not subscribed to this event!");

		subscriptions.erase(id);
	}

	void Event::Dispatch()
	{
		for (std::pair<ullong, const std::function<void()>> pair : subscriptions)
		{
			pair.second();
		}
	}

	const int Event::GetSubscriptionCount() const
	{
		return subscriptions.size();
	}

	ullong Event::PointerToHash(void* idPtr)
	{
		const std::string name = PointerToString(idPtr);
		return Muse::StringHash(name);
	}
}
