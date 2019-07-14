############################################################
# 变量处理与准备 
############################################################

TARGET  := $(strip $(TARGET))
TYPE    := $(strip $(TYPE))
CC       = gcc
CXX      = g++
CPP      = cpp
CFLAGS   += $(C_INC)
CXXFLAGS += $(C_INC)
CFLAGS	 += $(EXTRA_CFLAGS)
CXXFLAGS += $(EXTRA_CXXFLAGS)

CFLAGS	 += -Wall #-Werror
CXXFLAGS += -Wall #-Werror
CXXFLAGS += -Wno-pmf-conversions 
EXTRA_CXXFLAGS += -std=c++11

debug = 1
ifeq ($(debug), 1)
CFLAGS   += -g -DDEBUG_GLOBAL #--coverage
CXXFLAGS += -g -DDEBUG_GLOBAL #--coverage
#LDFLAGS  += --coverage
else
CFLAG += -DNDEBUG
CXXFLAGS += -DNDEBUG
endif

INC_DIR  += $(COMM_INC_DIR)
INC_DIR  += /usr/include/mysql/ /usr/include/ ./ $(ROOT_DIR)primary $(ROOT_DIR)third
LIB_DIR  += $(COMM_LIB_DIR)
LIB_DIR  += /usr/lib/ /usr/lib64/ /usr/lib64/mysql/
SYS_LIB  += libprotobuf librt libz libssl libcurl libmysqlclient libcrypto libpthread #libiconv
C_INC    = $(INC_DIR:%=-I%)
C_LIB    = $(LIB_DIR:%=-L%) $(LIB_DEPENDS:lib%=-l%) $(SYS_LIB:lib%=-l%)

SRC_DIR += .
C_SRC   = $(wildcard $(SRC_DIR:%=%/*.c))
CXX_SRC = $(wildcard $(SRC_DIR:%=%/*.cpp))
CCC_SRC = $(wildcard $(SRC_DIR:%=%/*.cc))
C_OBJ   = $(C_SRC:%.c=$(OBJ_DIR)%.o)
CCC_OBJ = $(CCC_SRC:%.cc=$(OBJ_DIR)%.o)
CXX_OBJ = $(CXX_SRC:%.cpp=$(OBJ_DIR)%.o)
C_DEP   = $(C_SRC:%.c=$(OBJ_DIR)%.d)
CXX_DEP = $(CXX_SRC:%.cpp=$(OBJ_DIR)%.d)
CCC_DEP = $(CCC_SRC:%.cc=$(OBJ_DIR)%.d)

ALL_OBJ = $(C_OBJ) $(CXX_OBJ) $(CCC_OBJ)
ALL_DEP = $(C_DEP) $(CXX_DEP) $(CCC_DEP)

ALL_GCNO = $(ALL_OBJ:%.o=%.gcno)
ALL_GCDA = $(ALL_OBJ:%.o=%.gcda)

############################################################
# 基础规则定义
############################################################

.PHONY: all clean install echo obj dir

OBJ_DIR  ?= obj/
ifeq ($(TYPE), lib)
INSTALL_DIR ?= $(ROOT_DIR)libs/
endif
ifeq ($(TYPE), app)
INSTALL_DIR ?= $(ROOT_DIR)bin/
endif

TARGET_MAKE := $(OBJ_DIR)$(TARGET)
TARGET_INSTALL := $(INSTALL_DIR)$(TARGET)

all:$(TARGET_MAKE)
install: all
	cp -f $(TARGET_MAKE) $(TARGET_INSTALL)

obj: $(ALL_OBJ)
	@echo complied all boject
dir:
	mkdir -p $(OBJ_DIR)
	mkdir -p $(INSTALL_DIR)

############################################################
# 编译目标文件
############################################################

ifneq ($(MAKECMDGOALS), echo)
$(C_DEP): $(OBJ_DIR)%.d : %.c
	@mkdir -p $(dir $@)
	$(CPP) $(EXTRA_CFLAGS) $(C_INC) -M $< > $@
	@sed -r -i 's|^(\w+)\.o[ :]*|$(@:.d=.o) : |' $@
$(CXX_DEP): $(OBJ_DIR)%.d : %.cpp
	@mkdir -p $(dir $@)
	$(CPP) $(EXTRA_CXXFLAGS) $(C_INC) -MM $< > $@
	@sed -r -i 's|^(\w+)\.o[ :]*|$(@:.d=.o) : |' $@
$(CCC_DEP): $(OBJ_DIR)%.d : %.cc
	@mkdir -p $(dir $@)
	$(CPP) $(EXTRA_CXXFLAGS) $(C_INC) -M $< > $@
	@sed -r -i 's|^(\w+)\.o[ :]*|$(@:.d=.o) : |' $@
# 将 .d 文件路径本身也插入依赖文件
# @sed -r -i 's|^(\w+)\.o[ :]*|$(@:.d=.o) $@ : |' $@

$(C_OBJ): $(OBJ_DIR)%.o : %.c
	@mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) $(EXTRA_CFLAGS) -o $@ $<
$(CXX_OBJ): $(OBJ_DIR)%.o : %.cpp
	@mkdir -p $(dir $@)
	$(CXX) -c $(CXXFLAGS) $(EXTRA_CXXFLAGS) -o $@ $<
$(CCC_OBJ): $(OBJ_DIR)%.o : %.cc
	@mkdir -p $(dir $@)
	$(CXX) -c $(CXXFLAGS) $(EXTRA_CXXFLAGS) -o $@ $<
endif

ifneq ($(MAKECMDGOALS), clean)
-include $(ALL_DEP)
endif

############################################################
# 编译最终目标
############################################################

ifeq ($(TYPE), app)
$(TARGET_MAKE): $(ALL_DEP) $(ALL_OBJ) $(OBJ_DEPENDS)
	$(CXX) $(ALL_OBJ) -Wl,--start-group $(OBJ_DEPENDS) -Wl,--end-group $(LDFLAGS) -o $@ $(C_LIB)
endif

ifeq ($(TYPE), lib)
$(TARGET_MAKE):$(ALL_DEP) $(ALL_OBJ) $(OBJ_DEPENDS)
	$(AR) r $(TARGET_MAKE) $(ALL_OBJ)
	cp -f $(TARGET_MAKE) $(TARGET_INSTALL)
endif

ifeq ($(TYPE), so)
SO_DEPENDS = $(LIB_DEPENDS:%=$(COMM_LIB_DIR)/%.so)
$(TARGET_MAKE): $(ALL_DEP) $(ALL_OBJ) $(SO_DEPENDS) $(OBJ_DEPENDS)
	$(CXX) -shared -fPIC $(C_LIB) $(ALL_OBJ) -Wl,--start-group $(OBJ_DEPENDS) -Wl,--end-group $(LDFLAGS) -o $@
endif

############################################################
# 清理目标
############################################################

clean:
	rm -rf $(ALL_OBJ) $(ALL_DEP) $(TARGET_MAKE) $(ALL_GCNO) $(ALL_GCDA)

echo:
	@echo $(ALL_SRC)
	@echo mkdir -p $(OBJ_DIR) $(INSTALL_DIR)
	@echo $(CPP) $(EXTRA_CXXFLAGS) $(C_INC)
	@echo $(ALL_DEP)
	@echo $(CXX) -c $(CXXFLAGS) $(EXTRA_CXXFLAGS)
	@echo $(ALL_OBJ)
	@echo $(CXX) $(C_LIB) $(ALL_OBJ) -Wl,--start-group $(OBJ_DEPENDS) -Wl,--end-group $(LDFLAGS) -o $(TARGET_MAKE)
	@echo $(TARGET) $(TARGET_MAKE) $(TARGET_INSTALL)

