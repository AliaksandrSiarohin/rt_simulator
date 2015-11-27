CXXFLAGS = -Wall -O0 -std=c++11
LDFLAGS = -pthread
LDLIBS = -lm -lrt

TASK=task
SOURCE_TASK=task_main.cpp task.cpp random_var.cpp
HEADERS_TASK=task.h random_var.h

all: $(SOURCE_TASK) $(HEADERS_TASK)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(SOURCE_TASK) $(LDLIBS) -o $(TASK)

clean:
	rm -f *.d *.o $(TASK)