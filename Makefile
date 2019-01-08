PROGRAM_NAME = arrow

SRCS = arrow.cpp

CXXFLAGS= -O3 -std=c++11
LDFLAGS= $(CXXFLAGS) $(LIBDIRS) -std=c++11
LDLIBS = -lfreeglut -lopengl32 -lglu32 -lglew32 -lm
OBJS=$(SRCS:%.cpp=%.o)

default: $(PROGRAM_NAME)

$(PROGRAM_NAME): $(OBJS)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

clean:
	-@rm$(OBJS) $(PROGRAM_NAME).exe

.PHONY: default clean