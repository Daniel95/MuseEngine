#pragma once

#include <functional>

#include "Core/Utilities/Utilities.h"

namespace Muse
{
	class Event
	{
	public:
		Event() = default;
		virtual ~Event() = default;

		void Subscribe(void* idPtr, const std::function<void()>& function);
		void Unsubscribe(void* idPtr);
		void Dispatch();
		const int GetSubscriptionCount() const;

	private:
		static ullong PointerToHash(void* idPtr);

		std::map<ullong, std::function<void()>> subscriptions;
	};
}