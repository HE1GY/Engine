#pragma once

namespace Engine
{
	class TimeStep
	{
	public:
		TimeStep(float step)
				:
				m_time(step)
		{
		}
		operator float() const
		{
			return m_time;
		}
		inline float GetInSeconds()
		{
			return m_time;
		}
		inline float get_in_milliseconds()
		{
			return m_time * 1000;
		}

	private:
		float m_time;
	};
}