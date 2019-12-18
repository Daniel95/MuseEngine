#pragma once

#include <functional>

#include "Core/Utilities/Utilities.h"

namespace Muse
{
	template<typename... Targs>
	class Event
	{
	public:
		Event() = default;
		virtual ~Event() = default;


		template<typename... Targs>
		void Subscribe(void* idPtr, const std::function<void(Targs ...)>& function);
		template<typename... Targs>
		void Unsubscribe(void* idPtr);
		template<typename... Targs>
		void Dispatch(Targs ...);
		template<typename... Targs>
		const int GetSubscriptionCount() const;

	private:
		static ullong PointerToHash(void* idPtr);

		template<typename... Targs>
		std::map<ullong, std::function<void(Targs ...)>> subscriptions;
	};
}