if (CM_BUILD_SHARED_LIBS)
    set(CM_LIB_TYPE "SHARED")
    set(CM_LIB_COMPILE_DEFS "_CM_BUILD_LIBS")
else ()
    set(CM_LIB_TYPE "STATIC")
endif ()
set(CM_LIB_DEP_SCOPE "PUBLIC")

set(CM_LIB_SRCS "lib.cpp")

set(CM_LIB_NAME cm)
add_library(${CM_LIB_NAME} ${CM_LIB_TYPE} ${CM_LIB_SRCS})
target_compile_features(${CM_LIB_NAME} ${CM_LIB_DEP_SCOPE} cxx_std_20)
target_compile_definitions(${CM_LIB_NAME} PRIVATE ${CM_LIB_COMPILE_DEFS})
target_include_directories(${CM_LIB_NAME} ${CM_LIB_DEP_SCOPE}
        "${PROJECT_SOURCE_DIR}/src/cpp/include"
        ${OpenCV_INCLUDE_DIRS}
)
target_link_libraries(${CM_LIB_NAME} ${CM_LIB_DEP_SCOPE}
        ${OpenCV_LIBS}
)

install(TARGETS ${CM_LIB_NAME} DESTINATION ".")
