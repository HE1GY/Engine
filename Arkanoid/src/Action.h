#pragma once
#include <functional>

template<typename... Args>
class Action
{
public:
	using EventHandler = std::function<void(Args...)>;

	void Invoke(Args... args)
	{
		for (auto& subscriber : m_subscribers)
		{
			subscriber(args...);
		}
	}

	void operator+=(EventHandler subscriber)
	{
		m_subscribers.push_back(subscriber);
	}

	void operator-=(EventHandler subscriber)
	{
		m_subscribers.erase(std::remove(m_subscribers.begin(), m_subscribers.end(), subscriber),
				m_subscribers.end());
	}
private:
	std::vector<EventHandler> m_subscribers;
};