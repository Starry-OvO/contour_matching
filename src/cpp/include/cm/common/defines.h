#pragma once

#include "cmake.h"

#ifdef CM_BUILD_SHARED_LIBS
#    ifdef _MSC_VER
#        ifdef _CM_BUILD_LIBS
#            define CM_API __declspec(dllexport)
#        else
#            define CM_API __declspec(dllimport)
#        endif
#    else
#        ifdef _CM_BUILD_LIBS
#            define CM_API __attribute__((visibility("default")))
#        else
#            define CM_API
#        endif
#    endif
#else
#    define CM_API
#endif
