#pragma once

namespace Framework {

	class Logger
	{
	public:
		enum class Type {
			Core,
			App,
			WithoutDuplicates
		};
		enum class Level {
			// Compatible with spdlog::level::level_enum.
			Trace,
			Debug,
			Info,
			Warn,
			Error,
			Critical
		};

		static void Log(Type type, Level level, std::string_view msg);

		template <typename... Args>
		static void Log(Type type, Level level, std::string_view msg, Args&&...args)
		{
			std::string formatted = std::vformat(msg, std::make_format_args(args...));
			Log(type, level, formatted);
		}

		static void Init();
	};

}

// Core log macros.
#define LOG_CORE_TRACE(...)    ::Framework::Logger::Log(::Framework::Logger::Type::Core, ::Framework::Logger::Level::Trace, __VA_ARGS__)
#define LOG_CORE_INFO(...)     ::Framework::Logger::Log(::Framework::Logger::Type::Core, ::Framework::Logger::Level::Info, __VA_ARGS__)
#define LOG_CORE_WARN(...)     ::Framework::Logger::Log(::Framework::Logger::Type::Core, ::Framework::Logger::Level::Warn, __VA_ARGS__)
#define LOG_CORE_ERROR(...)    ::Framework::Logger::Log(::Framework::Logger::Type::Core, ::Framework::Logger::Level::Error, __VA_ARGS__)
#define LOG_CORE_CRITICAL(...) ::Framework::Logger::Log(::Framework::Logger::Type::Core, ::Framework::Logger::Level::Critical, __VA_ARGS__)

// Client log macros.
#define LOG_TRACE(...)         ::Framework::Logger::Log(::Framework::Logger::Type::App, ::Framework::Logger::Level::Trace, __VA_ARGS__)
#define LOG_INFO(...)          ::Framework::Logger::Log(::Framework::Logger::Type::App, ::Framework::Logger::Level::Info, __VA_ARGS__)
#define LOG_WARN(...)          ::Framework::Logger::Log(::Framework::Logger::Type::App, ::Framework::Logger::Level::Warn, __VA_ARGS__)
#define LOG_ERROR(...)         ::Framework::Logger::Log(::Framework::Logger::Type::App, ::Framework::Logger::Level::Error, __VA_ARGS__)
#define LOG_CRITICAL(...)      ::Framework::Logger::Log(::Framework::Logger::Type::App, ::Framework::Logger::Level::Critical, __VA_ARGS__)

#define LOG_WITHOUT_DUPLICATES(...) ::Framework::Logger::Log(::Framework::Logger::Type::WithoutDuplicates, ::Framework::Logger::Level::Trace, __VA_ARGS__)

#define LOG_ONCE(...) \
	static bool logged = false; \
	if(!logged) \
	{ \
		logged=true; \
		::Framework::Logger::Log(::Framework::Logger::Type::WithoutDuplicates, ::Framework::Logger::Level::Trace, __VA_ARGS__); \
	}