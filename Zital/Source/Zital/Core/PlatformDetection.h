//platform detection using predefined macros rather than build tools
#ifdef _WIN32
//windows x64/x86
#	ifdef _WIN64
		//windows x64
#		define ZT_PLATFORM_WINDOWS
#	else
		//windows x86
#		error "x86 builds are not supported at this time"
#	endif
#elif defined(__APPLE__) || defined (__MACH__)
#	include <TargetConditionals.h>
#	if TARGET_IPHONE_SIMULATOR == 1
#		error "IOS simulator is not supported".
#	elif TARGET_OS_IPHONE == 1
#		define ZT_PLATFORM_IOS
#		error "IOS is not supported at this time."
#	elif TARGET_OS_MAC == 1
#		define ZT_PLATFORM_MACOS
#		error "MacOS is not supported at this time."
#	else
#		error "Unknown Apple platform."
#	endif
#elif defined(__ANDROID__)
#	define ZT_PLATFORM_ANDROID
#	error "Android is not supported at this time."
#elif defined(__linux__)
#	define ZT_PLATFORM_LINUX
#	error "Linux is not supported at this time."
#else
#	error "Unknown platform"
#endif