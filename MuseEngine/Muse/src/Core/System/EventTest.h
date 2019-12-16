#pragma once

#include <functional>
#include <vector>

#include <algorithm>

///https://stackoverflow.com/questions/18039723/c-trying-to-get-function-address-from-a-stdfunction

class EventTest
{
public:
	EventTest();
	~EventTest();

	void Subscribe(const std::function<void()>& function);
	void Unsubscribe(const std::function<void()>& function);
	void Dispatch();
	int GetSubscriptionCount() const;

private:

	std::vector<const std::function<void()>*> subscriptions;
};

EventTest::EventTest()
{
}

EventTest::~EventTest()
{
}

inline void EventTest::Subscribe(const std::function<void()>& function)
{

	//std::function::target<>()
	subscriptions.push_back(&function);
}

template <typename Function>
struct function_traits
	: public function_traits<decltype(&Function::operator())>
{
};

template <typename ClassType, typename ReturnType, typename... Args>
struct function_traits<ReturnType(ClassType::*)(Args...) const>
{
	typedef ReturnType(*pointer)(Args...);
	typedef std::function<ReturnType(Args...)> function;
};

template <typename Function>
typename function_traits<Function>::function
to_function(Function& lambda)
{
	return static_cast<typename function_traits<Function>::function>(lambda);
}

template <typename Lambda>
size_t getAddress(Lambda lambda)
{
	auto function = new decltype(to_function(lambda))(to_function(lambda));
	void* func = static_cast<void*>(function);
	return (size_t)func;
}

template<typename Clazz, typename Return, typename ...Arguments>
size_t getMemberAddress(std::function<Return(Clazz::*)(Arguments...)>& executor)
{
	typedef Return(Clazz::* fnType)(Arguments...);
	fnType** fnPointer = executor.template target<fnType*>();
	if (fnPointer != nullptr)
	{
		return (size_t)*fnPointer;
	}
	return 0;
}

inline void EventTest::Unsubscribe(const std::function<void()>& newFunction)
{
	for (const std::function<void()>* function : subscriptions)
	{
		if(getAddress(function) == getAddress(newFunction))
		{
			LOG_ENGINE_INFO("Comparison works!");
		}
	}


	//ASSERT_ENGINE(std::find(subscriptions.begin(), subscriptions.end(), &function) != subscriptions.end(), "Function not found!");

	//subscriptions.erase(std::remove(subscriptions.begin(), subscriptions.end(), &function), subscriptions.end());
}

inline void EventTest::Dispatch()
{
	for (const std::function<void()>* function : subscriptions)
	{
		(*function)();
	}
}

inline int EventTest::GetSubscriptionCount() const
{
	return subscriptions.size();
}


