INC_DIR:= ./inc/ ./inc/limonp ./inc/cppjieba ./inc/simhash/
SRC_DIR:= ./src/
SRCS:= $(wildcard ./src/*.cc)
OBJS:= $(patsubst %.cc, %.o, $(SRCS))
LIBS:= -lboost_regex

CXX:=g++

CXXFLAGS:= -w -g -std=c++11 $(addprefix -I, $(INC_DIR)) $(LIBS) -Wno-deprecated

EXE:=./bin/createLibrary.exe

$(EXE):$(OBJS)
	$(CXX) -o $(EXE) $(OBJS) $(CXXFLAGS)

clean:
	rm -rf $(EXE)
	rm -rf $(OBJS)
