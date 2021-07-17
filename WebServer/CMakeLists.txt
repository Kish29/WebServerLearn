# 整个WebServer需要依赖的源文件
set(
        SOURCES
        Main.cpp
        component/Utils.cpp
        component/HttpData.cpp
        component/Channel.cpp
        component/Timer.cpp
        component/EventLoop.cpp
        component/Server.cpp
        component/ArgsParser.cpp
        component/Epoll.cpp
#        tests/threadpool/ThreadPool.cpp
)

# 父目录的CMakeLists也会对子目录的文件生效
# 所以这里配置include目录
include_directories(base)
include_directories(component)
#include_directories(${PROJECT_SOURCE_DIR}/base)
#include_directories(${PROJECT_SOURCE_DIR}/component)

add_executable(WebServer ${SOURCES})
# WebServer需要base动态链接库
target_link_libraries(WebServer libserver_base)

add_subdirectory(base)
add_subdirectory(tests)
