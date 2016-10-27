PROGNAME = cardrecognition genimagehash
CXX = g++
CFLAGS += 
CPPFLAGS += 
CXXFLAGS += -g -Wall -std=c++11
LDFLAGS_CR += $(shell pkg-config --libs opencv) -pthread
LDFLAGS_IR += -lpHash -pthread -Lir/mvptree -lmvptree
LDFLAGS_GIH += -lpHash -pthread
INCLUDE_DIR_IR += -Iir/mvptree
INCLUDE_DIR_CR += 

OBJFILES_CR := $(patsubst cr/%.cpp,obj/cr/%.o,$(wildcard cr/*.cpp))
OBJFILES_IH := $(patsubst ih/%.cpp,obj/ih/%.o,$(wildcard ih/*.cpp))
OBJFILES_CR += obj/cardrecognition.o

MODULES = cr ih
OBJ_DIRECTORIES = $(addprefix obj/,$(MODULES))

all: create_obj_dirs $(PROGNAME)

genimagehash: obj/genimagehash.o
	$(CXX) -o $@ obj/genimagehash.o -lpHash -pthread

cardrecognition: $(OBJFILES_CR) $(OBJFILES_IH)
	$(CXX) -o $@ $(INCLUDE_DIR_CR) $(OBJFILES_CR) $(OBJFILES_IH) $(LDFLAGS_CR)

obj/%.o: %.cpp
	$(CXX) -c $< -o $@ $(CFLAGS) $(CPPFLAGS) $(CXXFLAGS)

obj/ih/%.o: cr/%.cpp
	$(CXX) -c $< -o $@ $(CFLAGS) $(CPPFLAGS) $(CXXFLAGS)

obj/ih/%.o: ih/%.cpp
	$(CXX) -c $< -o $@ $(CFLAGS) $(CPPFLAGS) $(CXXFLAGS)

clean:
	rm -f $(OBJFILES_CR) $(OBJFILES_IH) $(PROGNAME)

rebuild: clean all

create_obj_dirs: $(OBJ_DIRECTORIES)

$(OBJ_DIRECTORIES): 
	mkdir -p $@

.PHONY: create_obj_dirs
