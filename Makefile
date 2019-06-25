INC_DIR:= src src/net src/bo_threadpool src/timer src/cache depends
LIB_DIR:= lib
SRC_DIR:= src
NET_DIR:= src/net
TP_DIR:= src/bo_threadpool
TIMER_DIR:= src/timer
CACHE_DIR:= src/cache
INS_DIR:= /usr/local/bin
SRCS:= $(wildcard $(SRC_DIR)/*.cc) $(wildcard $(NET_DIR)/*.cc) $(wildcard $(TP_DIR)/*.cc) $(wildcard $(TIMER_DIR)/*.cc) $(wildcard $(CACHE_DIR)/*.cc)
OBJS:= $(patsubst %.cc, %.o, $(SRCS))
LIBS:= -lpthread -ljson -lcpp_redis -ltacopie

CXX:= g++

CXXFLAGS:= -std=c++11 -w -g -Wno-deprecated $(addprefix -I, $(INC_DIR)) $(addprefix -L, $(LIB_DIR)) $(LIBS)

ELF:= searchengined

$(ELF):$(OBJS)
	$(CXX) -o $(SRC_DIR)/$(ELF) $(OBJS) $(CXXFLAGS)
	cd src/preparation; make

install:
	install -m 0755 $(SRC_DIR)/$(ELF) $(INS_DIR)
	cd src/preparation; make install

uninstall:
	rm -rf $(INS_DIR)/$(ELF)
	cd src/preparation; make uninstall

clean:
	rm -rf $(SRC_DIR)/$(ELF)
	rm -rf $(OBJS)
	cd src/preparation; make clean
