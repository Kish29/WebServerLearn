CXX := g++
CXXFLAGS := --std=c++11 -Wall -g -I ../component
LDFLAGS := -lpthread

target: ComponentTest.o ThreadPool.o
	$(CXX) $(CXXFLAGS) -o target ComponentTest.o ThreadPool.o $(LDFLAGS)

ThreadPool.o: ../component/threadpool/ThreadPool.cpp
	$(CXX) $(CXXFLAGS) -c ../component/threadpool/ThreadPool.cpp $(LDFLAGS)

ComponentTest.o: ComponentTest.cpp
	$(CXX) $(CXXFLAGS) -c ComponentTest.cpp $(LDFLAGS)

.PHONY : clean

clean:
	rm -rf *.o target