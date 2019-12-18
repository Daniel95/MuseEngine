#pragma once

#include <functional>
#include <map>

#include "Core/Utilities/Utilities.h"

namespace Muse
{
	template<typename... Args>
	class Event
	{
	public:
		Event() = default;
		virtual ~Event() = default;

		void Subscribe(void* idPtr, const std::function<void(Args ...)>& function);
		void Unsubscribe(void* idPtr);
		void Dispatch(Args ...);
		const int GetSubscriptionCount() const;

	private:
		static ullong PointerToHash(void* idPtr);

		std::map<ullong, std::function<void(Args ...)>> subscriptions;
	};

	template <typename ... Args>
	void Event<Args ...>::Dispatch(Args ... args)
	{
		for (auto pair : subscriptions)
		{
			pair.second(args ...);
		}
	}

	template <typename ... Args>
	void Event<Args ...>::Subscribe(void* idPtr, const std::function<void(Args ...)>& function)
	{
		ullong id = PointerToHash(idPtr);

		ASSERT_ENGINE(subscriptions.count(id) == 0, "Cannot Subscribe: This ID is already subscribed to this event!");

		subscriptions[id] = function;
	}

	template <typename ... Args>
	void Event<Args ...>::Unsubscribe(void* idPtr)
	{
		ullong id = PointerToHash(idPtr);

		ASSERT_ENGINE(subscriptions.count(id) != 0, "Cannot Unsubscribe: This ID not subscribed to this event!");

		subscriptions.erase(id);
	}

	template <typename ... Args>
	const int Event<Args ...>::GetSubscriptionCount() const
	{
		return subscriptions.size();
	}

	template <typename ... Args>
	ullong Event<Args ...>::PointerToHash(void* idPtr)
	{
		const std::string name = PointerToString(idPtr);
		return Muse::StringHash(name);
	}
}