#include "spdlog/include/spdlog/sinks/basic_file_sink.h"
#include "spdlog/include/spdlog/sinks/stdout_color_sinks.h"

namespace paperback
{
	logger::MTWriter logger::mt_writer;
	logger::MTLogger logger::mt_logger;

	void logger::Init()
	{
		spdlog::set_pattern( "%^[%T] %n: %v%$" );

		mt_writer = spdlog::stdout_color_mt( "Console" );
		mt_logger = spdlog::basic_logger_mt( "Logger", "DebugLogs/log.txt" );

	#ifdef PAPERBACK_DEBUG
		spdlog::set_level( spdlog::level::trace );
	#else
		spdlog::set_level( spdlog::level::err );
	#endif
	}
}