ROOT_DIR	= ../../
TYPE		= app
TARGET		= tast_gateway 
INSTALL_DIR	= $(ROOT_DIR)bin/
SRC_DIR		=  errors servto action
INC_DIR		=
LIB_DIR		= 
SYS_LIB		=
LIB_DEPENDS	=  
OBJ_DEPENDS	= ../../libs/libCommonUtil.a \
	../../libs/libCommonNet.a \
	../../libs/libjsoncpp.a \
	../../libs/libfastcgi.a
EXTRA_CFLAGS	= 
EXTRA_CXXFLAGS	= 

# make qtest=1
ifeq ($(qtest), 1)
SRC_DIR += qtest
EXTRA_CXXFLAGS	+= -DQUICK_TEST
endif

# make logstd=1
ifeq ($(logstd), 1)
EXTRA_CXXFLAGS	+= -DLOGTO_STDERR
endif

include $(ROOT_DIR)root.mk

