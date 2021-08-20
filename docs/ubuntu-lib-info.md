# build-essential
#Ubuntu缺省情况下，并没有提供C/C++的编译环境，因此还需要手动安装。但是如果单独安装gcc以及g++比较麻烦，幸运的是，Ubuntu提供了一个build-essential软件包。查看该软件包的依赖关

apt-cache depends build-essential
build-essential
 |Depends: libc6-dev
  Depends: <libc-dev>
    libc6-dev
  Depends: gcc
  Depends: g++
  Depends: make
    make-guile
  Depends: dpkg-dev
也就是说，安装了该软件包，编译c/c++所需要的软件包也都会被安装。因此如果想在Ubuntu中编译c/c++程序,只需要安装该软件包就可以了。

# clang-tidy
是一个基于clang的静态代码分析框架 它是一个功能更强大的lint工具

# clang-format 
lint

# clang-tools

# clang#

# clangd

# libc++-dev libc++1 libc++abi-dev libc++abi1


# 全部
apt install -y clang-format clang-tidy clang-tools clang clangd libc++-dev libc++1 libc++abi-dev libc++abi1 libclang-dev libclang1 liblldb-11-dev libllvm-ocaml-dev libomp-dev libomp5 lld lldb llvm-dev llvm-runtime llvm python-clang