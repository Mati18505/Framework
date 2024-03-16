#include "pch.h"
#include "Logger.h"

// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/dup_filter_sink.h>
#pragma warning(pop)

namespace Framework
{
	static std::shared_ptr<spdlog::logger> coreLogger;
	static std::shared_ptr<spdlog::logger> clientLogger;
	static std::shared_ptr<spdlog::logger> dupLogger;

	void Logger::Log(Type type, Level level, std::string_view msg)
	{
		switch (type)
		{
		case Type::Core:
			coreLogger->log(spdlog::level::level_enum(level), msg);
			break;
		case Type::App:
			clientLogger->log(spdlog::level::level_enum(level), msg);
			break;
		case Type::WithoutDuplicates:
			dupLogger->log(spdlog::level::level_enum(level), msg);
			break;
		default:
			break;
		}
		
	}

	void Logger::Init()
	{
		std::vector<spdlog::sink_ptr> logSinks;
		logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("Framework.log", true));

		logSinks[0]->set_pattern("%^[%T] %n: %v%$");
		logSinks[1]->set_pattern("[%T] [%l] %n: %v");

		coreLogger = std::make_shared<spdlog::logger>("CORE", begin(logSinks), end(logSinks));
		spdlog::register_logger(coreLogger);
		coreLogger->set_level(spdlog::level::trace);
		coreLogger->flush_on(spdlog::level::trace);

		clientLogger = std::make_shared<spdlog::logger>("APP", begin(logSinks), end(logSinks));
		spdlog::register_logger(clientLogger);
		clientLogger->set_level(spdlog::level::trace);
		clientLogger->flush_on(spdlog::level::trace);

		auto dup_filter = std::make_shared<spdlog::sinks::dup_filter_sink_st>(std::chrono::seconds(5));
		dup_filter->set_sinks(logSinks);

		dupLogger = std::make_shared<spdlog::logger>("LOG", dup_filter);
		spdlog::register_logger(dupLogger);
		dupLogger->set_level(spdlog::level::trace);
		dupLogger->flush_on(spdlog::level::trace);
	}

}