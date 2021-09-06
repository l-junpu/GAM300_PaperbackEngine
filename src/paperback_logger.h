#pragma once

#include "spdlog/include/spdlog/spdlog.h"
#include "spdlog/include/spdlog/fmt/ostr.h"

namespace paperback
{
	class logger
	{
	public:

		using MTLogger = std::shared_ptr< spdlog::logger >;
		using MTWriter = std::shared_ptr< spdlog::logger >;

		PPB_INLINE
		static void Init( void );

		PPB_INLINE
		static MTLogger& GetLogger( void ) { return mt_logger; }

		PPB_INLINE
		static MTWriter& GetWriter( void ) { return mt_writer; }

	private:

		// Loggers & filesink
		static MTWriter mt_writer;
		static MTLogger mt_logger;
	};
}


#ifdef PAPERBACK_DEBUG

	// Writer Methods
	#define TRACE_PRINT( ... ) ::Paperback::Log::GetWriter()->trace( __VA_ARGS__ )
	#define DEBUG_PRINT( ... ) ::Paperback::Log::GetWriter()->debug( __VA_ARGS__ )
	#define INFO_PRINT( ... ) ::Paperback::Log::GetWriter()->info( __VA_ARGS__ )
	#define WARN_PRINT( ... ) ::Paperback::Log::GetWriter()->warn( __VA_ARGS__ )
	#define ERROR_PRINT( ... ) ::Paperback::Log::GetWriter()->error( __VA_ARGS__ )
	#define CRITICAL_PRINT( ... ) ::Paperback::Log::GetWriter()->critical( __VA_ARGS__ )

	// Logger Methods
	#define TRACE_LOG( ... ) ::Paperback::Log::GetLogger()->trace( __VA_ARGS__ )
	#define DEBUG_LOG( ... ) ::Paperback::Log::GetLogger()->debug( __VA_ARGS__ )
	#define INFO_LOG( ... ) ::Paperback::Log::GetLogger()->info( __VA_ARGS__ )
	#define WARN_LOG( ... ) ::Paperback::Log::GetLogger()->warn( __VA_ARGS__ )
	#define ERROR_LOG( ... ) ::Paperback::Log::GetLogger()->error( __VA_ARGS__ )
	#define CRITICAL_LOG( ... ) ::Paperback::Log::GetLogger()->critical( __VA_ARGS__ )

	// Assertion Methods
	#define PPB_ASSERT( expr )														\
		if (expr) {																	\
			ERROR_PRINT("Assertion occured at {}: Line {}", __FILE__, __LINE__);	\
			ERROR_LOG("Assertion occured at {}: Line {}", __FILE__, __LINE__);		\
			DEBUG_BREAK();															\
		}

	#define PPB_ASSERT_MSG( expr, ... )												\
		if (expr) {																	\
			ERROR_PRINT("Assertion occured at {}: Line {}", __FILE__, __LINE__);	\
			ERROR_PRINT(__VA_ARGS__);												\
			ERROR_LOG("Assertion occured at {}: Line {}", __FILE__, __LINE__);		\
			ERROR_LOG(__VA_ARGS__);													\
			DEBUG_BREAK();															\
		}

#else

	// Writer Methods
	#define TRACE_PRINT( ... ) 
	#define DEBUG_PRINT( ... ) 
	#define INFO_PRINT( ... )
	#define WARN_PRINT( ... )
	#define ERROR_PRINT( ... ) ::Paperback::Log::GetWriter()->error( __VA_ARGS__ )
	#define CRITICAL_PRINT( ... ) ::Paperback::Log::GetWriter()->critical( __VA_ARGS__ )

	// Logger Methods
	#define TRACE_LOG( ... )
	#define DEBUG_LOG( ... )
	#define INFO_LOG( ... )
	#define WARN_LOG( ... )
	#define ERROR_LOG( ... ) ::Paperback::Log::GetLogger()->error( __VA_ARGS__ )
	#define CRITICAL_LOG( ... ) ::Paperback::Log::GetLogger()->critical( __VA_ARGS__ )

	// Assertion Methods
	#define PPB_ASSERT( expr )
	#define PPB_ASSERT_MSG( expr, ... )

#endif