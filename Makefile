# compiler
GXX = g++

# compilation flags
CXXFLAGS = -g -Wall -Wextra -std=c++23

# giving all the source files
SRC = main.cpp \
	src/merge-sort/merge-sort.cpp \
	src/thread-pool/thread-pool.cpp

# build files of source files
BUILD_SRCS = $(SRC:.cpp=.o)

# main thing is the target
TARGET = app

# for running we need target
all: $(TARGET)

# compilation part where .cpp -> .o
%o: %.cpp 
	$(GXX) $(CXXFLAGS) $< -c $@

# for building the target we need built source files
# here all .o -> Target
$(TARGET): $(BUILD_SRCS)
	$(GXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -rf $(BUILD_SRCS) $(TARGET)