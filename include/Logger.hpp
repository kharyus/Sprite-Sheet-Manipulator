#ifndef LOGGER_HPP
#define LOGGER_HPP

/**
 *  This header adds the LOG(x) macro to be used while in debug mode.
 *  The LOG(x) statements get optimized out in release mode, since std::cout can consume some processing power.
 *
 *  This is the only macro, other than OS-specific ones, I intend to put on the program,
 *  since it avoids the possible performance loss of std::cout while on release.
 */
#if DEBUG
    // Include dependency
    #include <iostream>
    // Define Macro for logging.
    #define LOG(x) std::cout << x << std::endl;
    #define LOGNOBREAK(x) std::cout << x;
#elif RELEASE
    // Optimized out if build is Release.
    #define LOG(x) 0
    #define LOGNOBREAK(x) 0
#endif // DEBUG


#endif // LOGGER_HPP
