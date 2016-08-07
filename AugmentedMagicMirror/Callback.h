#pragma once

template <typename CallbackArgument>
class Callback
{
	template <typename Class>
	using CallbackMethod = void (Class::*)(const CallbackArgument &);

	typedef std::function<void(const CallbackArgument &)> CallbackFunction;

public:
	template <typename Class>
	void AddCallback(Class * Listener, CallbackMethod<Class> ListenerMethod) 
	{
		Callbacks.push_back([=](const CallbackArgument & Argument) { (Listener->*ListenerMethod)(Argument); });
	}

	template <typename Class>
	void operator+=(std::pair<Class *, CallbackMethod<Class> > Callback)
	{
		AddCallback(Callback.first, Callback.second);
	}

	void operator()(const CallbackArgument & Argument)
	{
		for (auto & Callback : Callbacks)
		{
			Callback(Argument);
		}
	}

private:
	std::vector<CallbackFunction> Callbacks;
};

