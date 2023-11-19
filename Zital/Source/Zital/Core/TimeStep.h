#pragma once

namespace Zital
{

	class TimeStep
	{
	public:
		TimeStep(float _time = 0.f)
			: mTime(_time)
		{
		}

		operator float() const { return mTime; }

		float GetSeconds() const { return mTime; }
		float GetMilliseconds() const { return mTime * 1000.f; }

	private:
		float mTime;
	};

}