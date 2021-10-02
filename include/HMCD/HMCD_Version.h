#define HMCD_VERSION_MAJOR 1
#define HMCD_VERSION_MINOR 3
#define HMCD_VERSION_PATCH 0
#define RELEASE_BUILD 0

#ifdef RELEASE_BUILD
    #define NDEBUG
#endif

#ifdef __WIN32
    #define __FILENAME__ (__FILE__ + 49)
#else
    #define __FILENAME__ (__FILE__ + 49)
#endif
