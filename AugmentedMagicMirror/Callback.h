#pragma once

template <typename ... CallbackArguments>
class Callback
{
	//using Test = const CallbackArguments & ...;

	template <typename Class>
	using CallbackMethod = void (Class::*)(const CallbackArguments & ...);

	typedef std::function<void(const CallbackArguments & ...)> CallbackFunction;

public:
	template <typename Class>
	void AddCallback(_In_ Class * Listener, _In_ CallbackMethod<Class> ListenerMethod)
	{
		Callbacks.push_back([=](const CallbackArguments & ... Arguments) { (Listener->*ListenerMethod)(Arguments...); });
	}

	template <typename Class>
	void operator+=(_In_ std::pair<Class *, CallbackMethod<Class> > Callback)
	{
		AddCallback(Callback.first, Callback.second);
	}

	void operator()(_In_ const CallbackArguments & ... Arguments)
	{
		for (auto & Callback : Callbacks)
		{
			Callback(Arguments...);
		}
	}

private:
	std::vector<CallbackFunction> Callbacks;
};

