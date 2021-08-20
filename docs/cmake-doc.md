##########编译选项 begin############
#SET( BUILD_SHARED_LIBS ON ) #所有库使用动态库
#add_compile_options(-fPIC) #编译静态库使用fPIC
#add_compile_options(-O3) #优化级别
#set(CMAKE_EXE_LINKER_FLAGS " -no-pie") #链接时不需要库是pic的
#add_definitions(-D_GLIBCXX_USE_CXX11_ABI=1) #不使用c11库

#SET (CMAKE_C_FLAGS                "-Wall -std=c99")
#SET (CMAKE_C_FLAGS_DEBUG          "-g")
#SET (CMAKE_C_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
#SET (CMAKE_C_FLAGS_RELEASE        "-O4 -DNDEBUG")
#SET (CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g")
#
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++14")
#SET (CMAKE_CXX_FLAGS_DEBUG          "-g")
#SET (CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
#SET (CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG")
#SET (CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")
##########编译选项 end############

#设置库的前缀
#SET(CMAKE_FIND_LIBRARY_PREFIXES "lib")
#设置库的后缀
#SET(CMAKE_FIND_LIBRARY_SUFFIXES ".a" ${CMAKE_FIND_LIBRARY_SUFFIXES})
#find_path 和 find_library 时，会搜索一些默认的路径。
#当我们将一些lib安装在非默认搜索路径时，cmake就没法搜索到了。
#这是我们需要添加路径。
#set(CMAKE_INCLUDE_PATH "include_path")
#set(CMAKE_LIBRARY_PATH "lib_path")
#引用PkgConfig，可使用pkg查找库
#find_package(PkgConfig REQUIRED)



修改默认gcc和g++为4.4的版本
update-alternatives --remove-all gcc
update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.4 40
update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-4.4 40
配置默认的gcc和g++
update-alternatives --config gcc
update-alternatives --config g++


cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=./script/linux.toolchain.cmake  . ..
make 