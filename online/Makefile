INC_DIR:= ./inc/ ./inc/net/ ./inc/bo_threadpool/ ./inc/timer/ ./inc/cache/ ./deps/cppjieba/ ./deps/cpp_redis/ ./deps/tacopie/
SRC_DIR:= ./src/ ./src/net/ ./src/bo_threadpool/ ./src/timer/ ./src/cache/
SRCS:= $(wildcard ./src/*.cc) $(wildcard ./src/net/*.cc) $(wildcard ./src/bo_threadpool/*.cc) $(wildcard ./src/timer/*.cc) $(wildcard ./src/cache/*.cc)
OBJS:= $(patsubst %.cc, %.o, $(SRCS))
LIBS:= -lpthread -ljson -lcpp_redis -ltacopie

CXX:=g++

CXXFLAGS:= -w -g -std=c++11 $(addprefix -I, $(INC_DIR)) $(LIBS) -Wno-deprecated

EXE:=./bin/rssSearchServer.exe

$(EXE):$(OBJS)
	$(CXX) -o $(EXE) $(OBJS) $(CXXFLAGS)

clean:
	rm -rf $(EXE)
	rm -rf $(OBJS)
