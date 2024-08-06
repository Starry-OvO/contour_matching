set(__RESTORE_BUILD_TYPE ${CMAKE_BUILD_TYPE})
set(CMAKE_BUILD_TYPE Release)

find_package(OpenCV REQUIRED imgcodecs imgproc)

set(CMAKE_BUILD_TYPE ${__RESTORE_BUILD_TYPE})
