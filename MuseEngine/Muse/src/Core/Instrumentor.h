#pragma once

#include "Core/Utilities/Defines.h"

#include <string>
#include <chrono>
#include <algorithm>
#include <fstream>

#include <thread>

namespace Muse
{
	struct ProfileResult
	{
		std::string Name;
		long long Start, End;
		uint32_t ThreadId;
	};

	struct InstrumentationSession
	{
		std::string Name;
	};

	class Instrumentor
	{
	public:
		Instrumentor() = default;

		void BeginSession(const std::string& a_Name, const std::string& a_Filepath = "results.json")
		{
			ASSERT_ENGINE(m_CurrentSession == nullptr, "Another instrumentor session is already active!");

			m_OutputStream.open(a_Filepath);
			WriteHeader();
			m_CurrentSession = new InstrumentationSession{ a_Name };
		}

		void EndSession()
		{
			WriteFooter();
			m_OutputStream.close();
			delete m_CurrentSession;
			m_CurrentSession = nullptr;
			m_ProfileCount = 0;
		}

		void WriteProfile(const ProfileResult& a_Result)
		{
			if (m_ProfileCount++ > 0)
			{
				m_OutputStream << ",";
			}

			std::string name = a_Result.Name;
			std::replace(name.begin(), name.end(), '"', '\'');

			m_OutputStream << "{";
			m_OutputStream << "\"cat\":\"function\",";
			m_OutputStream << "\"dur\":" << (a_Result.End - a_Result.Start) << ',';
			m_OutputStream << "\"name\":\"" << name << "\",";
			m_OutputStream << "\"ph\":\"X\",";
			m_OutputStream << "\"pid\":0,";
			m_OutputStream << "\"tid\":" << a_Result.ThreadId << ",";
			m_OutputStream << "\"ts\":" << a_Result.Start;
			m_OutputStream << "}";

			m_OutputStream.flush();
		}

		void WriteHeader()
		{
			m_OutputStream << "{\"otherData\": {},\"traceEvents\":[";
			m_OutputStream.flush();
		}

		void WriteFooter()
		{
			m_OutputStream << "]}";
			m_OutputStream.flush();
		}

		static Instrumentor& Get()
		{
			static Instrumentor instance;
			return instance;
		}

	private:
		InstrumentationSession* m_CurrentSession = nullptr;
		std::ofstream m_OutputStream;
		int m_ProfileCount = 0;
	};

	class InstrumentationTimer
	{
	public:
		InstrumentationTimer(const char* a_Name)
			: m_Name(a_Name)
		{
			m_StartTimepoint = std::chrono::high_resolution_clock::now();
		}

		~InstrumentationTimer()
		{
			if (!m_Stopped)
			{
				Stop();
			}
		}

		void Stop()
		{
			auto endTimepoint = std::chrono::high_resolution_clock::now();

			long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
			long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

            const uint32_t threadId = static_cast<uint32_t>(std::hash<std::thread::id>{}(std::this_thread::get_id()));
			Instrumentor::Get().WriteProfile({ m_Name, start, end, threadId });

			m_Stopped = true;
		}
	private:
		const char* m_Name;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
		bool m_Stopped = false;

	};
}

#define MUSE_PROFILE 0
#if MUSE_PROFILE
#define MUSE_PROFILE_BEGIN_SESSION(name, filepath) ::Muse::Instrumentor::Get().BeginSession(name, filepath)
#define MUSE_PROFILE_END_SESSION() ::Muse::Instrumentor::Get().EndSession()
#define MUSE_PROFILE_SCOPE(name) ::Muse::InstrumentationTimer timer##__LINE__(name);
#define MUSE_PROFILE_FUNCTION() MUSE_PROFILE_SCOPE(__FUNCSIG__)
#else
#define MUSE_PROFILE_BEGIN_SESSION(name, filepath)
#define MUSE_PROFILE_END_SESSION()
#define MUSE_PROFILE_SCOPE(name)
#define MUSE_PROFILE_FUNCTION()
#endif