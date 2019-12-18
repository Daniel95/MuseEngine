#include "MusePCH.h"
#include "Event.h"

namespace Muse
{
	template <typename ... Targs>
	void Event<Targs ...>::Subscribe(void* idPtr, const std::function<void(Targs ...)>& function)
	{
		ullong id = PointerToHash(idPtr);

		ASSERT_ENGINE(subscriptions.count(id) == 0, "Cannot Subscribe: This ID is already subscribed to this event!");

		subscriptions[id] = function;
	}

	template <typename ... Targs>
	void Event<Targs ...>::Unsubscribe(void* idPtr)
	{
		ullong id = PointerToHash(idPtr);

		ASSERT_ENGINE(subscriptions.count(id) != 0, "Cannot Unsubscribe: This ID not subscribed to this event!");

		subscriptions.erase(id);
	}

	template <typename ... Targs>
	void Event<Targs ...>::Dispatch(Targs ...)
	{
		for (std::pair<ullong, const std::function<void()>> pair : subscriptions)
		{
			pair.second(Targs ...);
		}
	}

	template <typename ... Targs>
	const int Event<Targs ...>::GetSubscriptionCount() const
	{
		return subscriptions.size();
	}

	template <typename ... Targs>
	ullong Event<Targs ...>::PointerToHash(void* idPtr)
	{
		const std::string name = PointerToString(idPtr);
		return Muse::StringHash(name);
	}
}
