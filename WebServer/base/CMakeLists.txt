# 动态链接库libserver_base所依赖的源文件
set(
        LIB_SRC
        # logger相关
        log/AppendFile.cpp
        log/LogFile.cpp
        log/Logger.cpp
        log/LogStream.cpp
        log/AsyncLogging.cpp
        # thread相关
        thread/Thread.cpp
        thread/CountDownLatch.cpp
)

# 声明动态链接库
add_library(libserver_base ${LIB_SRC})
# 该库需要的额外动态链接库
target_link_libraries(libserver_base pthread rt)
# 该动态链接库的名称
set_target_properties(libserver_base PROPERTIES OUTPUT_NAME "server_base")

add_subdirectory(tests)