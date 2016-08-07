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
	void AddCallback(Class * Listener, CallbackMethod<Class> ListenerMethod) 
	{
		Callbacks.push_back([=](const CallbackArguments & ... Arguments) { (Listener->*ListenerMethod)(Arguments...); });
	}

	template <typename Class>
	void operator+=(std::pair<Class *, CallbackMethod<Class> > Callback)
	{
		AddCallback(Callback.first, Callback.second);
	}

	void operator()(const CallbackArguments & ... Arguments)
	{
		for (auto & Callback : Callbacks)
		{
			Callback(Arguments...);
		}
	}

private:
	std::vector<CallbackFunction> Callbacks;
};

