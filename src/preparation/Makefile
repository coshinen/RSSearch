INC_DIR:= . ../../depends
SRC_DIR:= .
DIST_DIR:= ..
INST_DIR:= /usr/local/bin
SRCS:= $(wildcard $(SRC_DIR)/*.cc)
OBJS:= $(patsubst %.cc, %.o, $(SRCS))
LIBS:= -lboost_regex

CXX:=g++

CXXFLAGS:= -w -g -std=c++11 $(addprefix -I, $(INC_DIR)) $(LIBS) -Wno-deprecated

EXE:= createlib

$(EXE):$(OBJS)
	$(CXX) -o $(SRC_DIR)/$(DIST_DIR)/$(EXE) $(OBJS) $(CXXFLAGS)

install:
	install -m 0755 $(SRC_DIR)/$(DIST_DIR)/$(EXE) $(INST_DIR)

uninstall:
	rm -rf $(INST_DIR)/$(EXE)

clean:
	rm -rf $(SRC_DIR)/$(DIST_DIR)/$(EXE)
	rm -rf $(OBJS)
