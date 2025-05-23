CXXFLAGS = -O2 -Wall -Wextra -Werror -std=c++17 -Iinclude
SRCS = QAM.cpp main.cpp
OBJS = $(notdir $(SRCS:.cpp=.o))
VPATH = src:.
EXE = main

all: $(EXE)

$(EXE): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXE)

.PHONY: all clean