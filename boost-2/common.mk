# wildcard                  $(wildcard PATTERN...)   
#								展开为已经存在的、使用空格分开的、匹配此模式的所有文件列表                                                                               
# addprefix                 $(addprefix <prefix>,<names...>)            
#								把前缀 <prefix> 加到 <names> 中的每个单词前面
#   =                       是最基本的赋值    
#   :=                      是覆盖之前的值    
#   ?=                      是如果没有被赋值过就赋予等号后面的值    
#   +=                      是添加等号后面的值    
# OBJS DEPS                 根据.c文件 得到相应的 .o .d 文件,随后更新为每个文件的路径    
#   BIN                     可执行程序的位置    
# LINK_OBJ_DIR  DEP_DIR     .o 文件路径    

CC = g++ -g
FALG=-std=c++11 -ljsoncpp -lpthread
FALGBOOST=-lboost_filesystem -lboost_system
FALGJIEBA=-I $(BUILD_ROOT)/cppjieba-master/include

#得到所有 .cpp 文件名
SRCS = $(wildcard *.cpp)
# *.cpp -> *.o
OBJS = $(SRCS:.cpp=.o)
# *.cpp -> *d
DEPS = $(SRCS:.cpp=.d)

# 最后可执行文件路径 
BIN:=$(addprefix $(BUILD_ROOT)/,$(BIN))

LINK_OBJ_DIR = $(BUILD_ROOT)/link_obj
DEP_DIR = $(BUILD_ROOT)/dep

# 创建.o,.d文件放置的文件夹
$(shell mkdir -p $(LINK_OBJ_DIR))
$(shell mkdir -p $(DEP_DIR))

# 得到每个 .o .d 文件的路径
OBJS:=$(addprefix $(LINK_OBJ_DIR)/,$(OBJS))
DEPS:=$(addprefix $(DEP_DIR)/,$(DEPS))

LINK_OBJ=$(wildcard $(LINK_OBJ_DIR)/*.o)
LINK_OBJ += $(OBJS)

all:$(DEPS) $(OBJS) $(BIN)

$(BIN) : $(LINK_OBJ)
	$(CC) -o $@ $^ $(FALG) $(FALGBOOST) $(FALGJIEBA)
$(LINK_OBJ_DIR)/%.o : %.cpp
	$(CC) -c $^ -o $@ -I $(INCLUDE_PATH) $(FALG) $(FALGBOOST) $(FALGJIEBA)

$(DEP_DIR)/%.d : %.cpp
	echo -n $(LINK_OBJ_DIR)/ > $@
	$(CC) -I $(INCLUDE_PATH) $(FALG) $(FALGBOOST) $(FALGJIEBA) -MM $^ >> $@

