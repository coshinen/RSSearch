INC_DIR:= . ../../depends
LIB_DIR:= ../../lib
SRC_DIR:= .
DIST_DIR:= ..
INST_DIR:= /usr/local/bin
SRCS:= $(wildcard $(SRC_DIR)/*.cc)
OBJS:= $(patsubst %.cc, %.o, $(SRCS))
LIBS:= -lboost_regex

CXX:=g++

CXXFLAGS:= -std=c++11 -w -g -Wno-deprecated $(addprefix -I, $(INC_DIR)) $(addprefix -L, $(LIB_DIR)) $(LIBS)

ELF:= createlib

$(ELF):$(OBJS)
	$(CXX) -o $(SRC_DIR)/$(DIST_DIR)/$(ELF) $(OBJS) $(CXXFLAGS)

install:
	install -m 0755 $(SRC_DIR)/$(DIST_DIR)/$(ELF) $(INST_DIR)

uninstall:
	rm -rf $(INST_DIR)/$(ELF)

clean:
	rm -rf $(SRC_DIR)/$(DIST_DIR)/$(ELF)
	rm -rf $(OBJS)
