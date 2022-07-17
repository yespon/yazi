#指定编译器
CC = g++

#找出当前目录下，所有的源文件（以.cpp结尾）
SRCS := $(shell find ./* -type f | grep '\.cpp' | grep -v '\.svn' | grep -v '\./plugin' | grep -v '\./log' | grep -v 'main\.cpp')
$(warning SRCS is ${SRCS})

#确定cpp源文件对应的目标文件
OBJS := $(patsubst %.cpp, %.o, $(filter %.cpp, $(SRCS)))
$(warning OBJS is ${OBJS})

#编译选项
CFLAGS = -g -O2 -Wall -Werror -Wno-unused -ldl -fPIC
$(warning CFLAGS is ${CFLAGS})

#找出当前目录下所有头文件
INCLUDE_TEMP = $(shell find ./* -type d | grep -v '\.svn' | grep -v '\./plugin' | grep -v '\./client' | grep -v '\./config' | grep -v '\./log')
INCLUDE = $(patsubst %,-I %, $(INCLUDE_TEMP))
$(warning INCLUDE is ${INCLUDE})

LDFLAG = -lpthread -std=c++11

#主程序
SRC_MAIN = main.cpp
OBJ_MAIN = ${SRC_MAIN:%.cpp=%.o}
EXE_MAIN = main

target: ${EXE_MAIN}
$(EXE_MAIN): $(OBJ_MAIN) $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(INCLUDE) $(LDFLAG)

%.o: %.cpp
	${CC} ${CFLAGS} ${INCLUDE} -c $< -o $@

# test 插件
PULGIN_TEST = plugin/testplugin.so
SRC_PLUGIN_TEST = plugin/TestPlugin.cpp json/Parser.cpp json/Json.cpp

# user 插件
PLUGIN_USER = plugin/userplugin.so
SRC_PLUGIN_USER = plugin/UserPlugin.cpp json/Parser.cpp json/Json.cpp

# echo 插件
PLUGIN_ECHO = plugin/echoplugin.so
SRC_PLUGIN_ECHO = plugin/EchoPlugin.cpp utility/Logger.cpp utility/System.cpp

${PULGIN_TEST}:
	${CC} -shared -fPIC ${INCLUDE} ${SRC_PLUGIN_TEST} -o ${PULGIN_TEST}

${PLUGIN_USER}:
	${CC} -shared -fPIC ${INCLUDE} ${SRC_PLUGIN_USER} -o ${PLUGIN_USER}

${PLUGIN_ECHO}:
	${CC} -shared -fPIC ${INCLUDE} ${SRC_PLUGIN_ECHO} -o ${PLUGIN_ECHO}

plugin: ${PULGIN_TEST} ${PLUGIN_USER} ${PLUGIN_ECHO}
	
clean:
	rm -f ${OBJS} ${OBJ_MAIN} ${EXE_MAIN}
