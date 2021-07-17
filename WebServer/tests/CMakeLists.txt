add_executable(
        ComponentTest ComponentTest.cpp
        ../component/threadpool/ThreadPool.cpp
)

target_link_libraries(ComponentTest pthread rt)