# for my llvm 
#path to top level of llvm hierarchy


CXX := clang++
CFLAGS := -Wall `llvm-config --cppflags` -fno-rtti
LDFLAGS := `llvm-config --ldflags`
LIBS := `llvm-config --libs`
OPT := opt

#name of the library to build
LIBRARYNAME := callgraph.so

TEST_PROGS :=


SRCS :=  callgraph.cc
OBJS := $(SRCS:%.cc=%.o)

all : $(LIBRARYNAME)

#incude tht makefile implementation stuff


%.o : %.cc
	$(CXX) $(CFLAGS) -c -o $@ $<
	   
%.so : callgraph.o
	$(CXX) $(CFLAGS) $(LDFLAGS) -shared -o $@ $^ $(LDFLAGS)
		 
clean :
	rm -f $(OBJS) $(LIBRARYNAME)
		 
test : 
	$(OPT) -load ./callgraph.so < tests/cp.bc > /dev/null
