PROGNAME=opencv_demo

CXX = g++
CFLAGS += 
CPPFLAGS +=
CXXFLAGS += -g -Wall
LDFLAGS += $(shell pkg-config --libs opencv)
INCLUDE_DIR +=

OBJFILES := $(patsubst %.cpp,obj/%.o,$(wildcard *.cpp))

all: $(PROGNAME)

$(PROGNAME): $(OBJFILES) 
	echo ${LDFLAGS}
	$(CXX) -o $(PROGNAME) $(INCLUDE_DIR) $(OBJFILES) $(LDFLAGS)

obj/%.o: %.cpp 
	@mkdir -p obj
	$(CXX) -c $< -o $@ $(CFLAGS) $(CPPFLAGS) $(CXXFLAGS)

clean:
	rm -f $(OBJFILES) $(PROGNAME)

rebuild: clean all

