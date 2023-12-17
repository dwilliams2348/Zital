#pragma once

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>

#include <thread>

namespace Zital
{

	struct ProfileResult
	{
		std::string Name;
		long long Start, End;
		uint32_t ThreadID;
	};

	struct InstrumentationSession
	{
		std::string Name;
	};

	class Instrumentor
	{
	public:
		Instrumentor()
			: mCurrSession(nullptr), mProfileCount(0)
		{
		}

		void BeginSession(const std::string _name, const std::string& _filepath = "reults.json")
		{
			mOutputStream.open(_filepath);
			WriteHeader();
			mCurrSession = new InstrumentationSession{ _name };
		}

		void EndSession()
		{
			WriteFooter();
			mOutputStream.close();
			delete mCurrSession;
			mCurrSession = nullptr;
			mProfileCount = 0;
		}

		void WriteProfile(const ProfileResult& _result)
		{
			if (mProfileCount++ > 0)
				mOutputStream << ",";

			std::string name = _result.Name;
			std::replace(name.begin(), name.end(), '"', '\'');

			mOutputStream << "{";
			mOutputStream << "\"cat\":\"function\",";
			mOutputStream << "\"dur\":" << (_result.End - _result.Start) << ',';
			mOutputStream << "\"name\":\"" << name << "\",";
			mOutputStream << "\"ph\":\"X\",";
			mOutputStream << "\"pid\":0,";
			mOutputStream << "\"tid\":" << _result.ThreadID << ",";
			mOutputStream << "\"ts\":" << _result.Start;
			mOutputStream << "}";

			mOutputStream.flush();
		}

		void WriteHeader()
		{
			mOutputStream << "{\"otherData\": {}, \"traceEvents\":[";
			mOutputStream.flush();
		}

		void WriteFooter()
		{
			mOutputStream << "]}";
			mOutputStream.flush();
		}

		static Instrumentor& Get()
		{
			static Instrumentor instance;
			return instance;
		}

	private:
		InstrumentationSession* mCurrSession;
		std::ofstream mOutputStream;
		int mProfileCount;
	};

	class InstrumentationTimer
	{
	public:
		InstrumentationTimer(const char* _name)
			: mName(_name), mStopped(false)
		{
			mStartingTimepoint = std::chrono::high_resolution_clock::now();
		}

		~InstrumentationTimer()
		{
			if (!mStopped)
				Stop();
		}

		void Stop()
		{
			auto endTimepoint = std::chrono::high_resolution_clock::now();

			long long start = std::chrono::time_point_cast<std::chrono::microseconds>(mStartingTimepoint).time_since_epoch().count();
			long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

			uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());
			Instrumentor::Get().WriteProfile({ mName, start, end, threadID });

			mStopped = true;
		}

	private:
		const char* mName;
		std::chrono::time_point<std::chrono::steady_clock> mStartingTimepoint;
		bool mStopped;
	};

}

#define ZT_PROFILE 0
#if ZT_PROFILE
	#define ZT_PROFILE_BEGIN_SESSION(name, filepath) ::Zital::Instrumentor::Get().BeginSession(name, filepath)
	#define ZT_PROFILE_END_SESSION() ::Zital::Instrumentor::Get().EndSession()
	#define ZT_PROFILE_SCOPE(name) ::Zital::InstrumentationTimer timer##__LINE__(name);
	#define ZT_PROFILE_FUNCTION() ZT_PROFILE_SCOPE(__FUNCSIG__)
#else
	#define ZT_PROFILE_BEGIN_SESSION(name, filepath)
	#define ZT_PROFILE_END_SESSION()
	#define ZT_PROFILE_SCOPE(name)
	#define ZT_PROFILE_FUNCTION()
#endif