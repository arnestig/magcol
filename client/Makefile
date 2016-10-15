PROGNAME = cardtracking imagerecognition
CXX = g++
CFLAGS += 
CPPFLAGS +=
CXXFLAGS += -g -Wall
LDFLAGS_CT += $(shell pkg-config --libs opencv)
LDFLAGS_IR += -lpHash
INCLUDE_DIR +=

OBJFILES_CT := $(patsubst ct/%.cpp,obj/ct/%.o,$(wildcard ct/*.cpp))
OBJFILES_IR := $(patsubst ir/%.cpp,obj/ir/%.o,$(wildcard ir/*.cpp))

all: $(PROGNAME)

cardtracking: $(OBJFILES_CT) 
	echo $(OBJFILES_CT)
	$(CXX) -o $@ $(INCLUDE_DIR) $(OBJFILES_CT) $(LDFLAGS_CT)

imagerecognition: $(OBJFILES_IR) 
	$(CXX) -o $@ $(INCLUDE_DIR) $(OBJFILES_IR) $(LDFLAGS_IR)

obj/ct/%.o: ct/%.cpp 
	echo ${OBJ_FILES_CT}
	@mkdir -p obj/ct
	$(CXX) -c $< -o $@ $(CFLAGS) $(CPPFLAGS) $(CXXFLAGS)

obj/ir/%.o: ir/%.cpp 
	@mkdir -p obj/ir
	$(CXX) -c $< -o $@ $(CFLAGS) $(CPPFLAGS) $(CXXFLAGS)

clean:
	rm -f $(OBJFILES) $(PROGNAME)

rebuild: clean all

