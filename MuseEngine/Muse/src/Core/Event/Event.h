﻿#pragma once

#include <functional>
#include <unordered_map>

#include "Core/Utilities/Utilities.h"

namespace Muse
{
	template<typename... Args>
	class Event
	{
	public:
		Event() = default;
		virtual ~Event() = default;

		void Subscribe(const void* a_PointerID, const std::function<void(Args ...)>& a_Function);
		void Unsubscribe(const void* a_PointerID);
		void Dispatch(Args ... a_Args);
		const int GetSubscriptionCount() const;

	private:
		static ullong PointerToHash(const void* a_PointerID);

		std::unordered_map<ullong, std::function<void(Args ...)>> subscriptions;
	};

	template <typename ... Args>
	void Event<Args ...>::Dispatch(Args ... a_Args)
	{
		for (auto pair : subscriptions)
		{
			pair.second(a_Args ...);
		}
	}

	template <typename ... Args>
	void Event<Args ...>::Subscribe(const void* a_PointerID, const std::function<void(Args ...)>& a_Function)
	{
		ullong id = PointerToHash(a_PointerID);

		ASSERT_ENGINE(subscriptions.count(id) == 0, "Cannot Subscribe: This ID is already subscribed to this event!");

		subscriptions[id] = a_Function;
	}

	template <typename ... Args>
	void Event<Args ...>::Unsubscribe(const void* a_PointerID)
	{
		ullong id = PointerToHash(a_PointerID);

		ASSERT_ENGINE(subscriptions.count(id) != 0, "Cannot Unsubscribe: This ID not subscribed to this event!");

		subscriptions.erase(id);
	}

	template <typename ... Args>
	const int Event<Args ...>::GetSubscriptionCount() const
	{
		return subscriptions.size();
	}

	template <typename ... Args>
	ullong Event<Args ...>::PointerToHash(const void* a_PointerID)
	{
		const std::string name = PointerToString(a_PointerID);
		return Muse::StringHash(name);
	}
}