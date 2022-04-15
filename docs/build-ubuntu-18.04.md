# Ubuntu Server 18.04 instructions

These instructions assume:
* You are using a minimal Ubuntu Server 18.04 image; other versions may not work
* You are using a fresh image or container. If you installed any packages before
  following these instructions, or selected any optional components (e.g. compilers,
  services, GUIs, etc.), then these instructions may fail.

The repo also includes a Dockerfile which does these steps. The Dockerfile needs
10 cores and plenty of RAM to build.

docker run --name cppenv -d -v /Users/liweidong/work/mygit:/root/mygit -p 8805:8805 -p 8806:8806 -p 8807:8807 -it ubuntu:18.04 bash
docker exec -it cppenv

# Install build environment

Install clang 12 and other needed tools:
```
# apt install aptitude 一个安装依赖的工具
apt update && apt install -y wget curl vim
apt-get install build-essential autoconf libtool pkg-config libc++-dev

cd ~
wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add -
cat <<EOT >>/etc/apt/sources.list
deb http://apt.llvm.org/bionic/ llvm-toolchain-bionic main
deb-src http://apt.llvm.org/bionic/ llvm-toolchain-bionic main
deb http://apt.llvm.org/bionic/ llvm-toolchain-bionic-12 main
deb-src http://apt.llvm.org/bionic/ llvm-toolchain-bionic-12 main
EOT

apt update # update agin

apt install -y \
    bzip2                 \
    clang-12              \
    clang-format-12       \
    lld-12                \
    lldb-12               \
    libssl-dev            \
    libcurl4-openssl-dev  \
    zlib1g-dev

#设置默认编译器的
update-alternatives --install /usr/bin/clang clang /usr/bin/clang-12 100
update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-12 100
update-alternatives --install /usr/bin/clang-format clang-format /usr/bin/clang-format-12 100
```
# Install openssl

#### 1.0.1
```
wget https://www.openssl.org/source/old/1.0.1/openssl-1.0.1e.tar.gz
tar -zxvf openssl-1.0.1e.tar.gz
cd openssl-1.0.1e
./config --prefix=/usr/local/openssl-1.0.1e
make
make install #解决方法是：编辑/usr/bin/pod2man文件，注释掉第71行。千万注意，不要像网上有些说的rm /usr/bin/pod2man，即删除pod2man这个文件，否则安装pcsc组件会出错
```
#### 1.1.1
```
wget https://www.openssl.org/source/old/1.1.1/openssl-1.1.1e.tar.gz
tar -zxvf openssl-1.1.1e.tar.gz 
cd openssl-1.1.1e
./config --prefix=/usr/local/openssl-1.1.1e
make 
make install
```
#### 修改系统openssl
```
mv /usr/bin/openssl /usr/bin/openssl.old
mv /usr/include/openssl /usr/include/openssl.old 
ln -s /usr/local/openssl-1.1.1e/bin/openssl /usr/bin/openssl
ln -s /usr/local/openssl-1.1.1e/include/openssl /usr/include/openssl
echo "/usr/local/openssl-1.1.1e/lib" >> /etc/ld.so.conf                #在/etc/ld.so.conf文件中写入openssl库文件的搜索路径
ldconfig -v                                                 #使修改后的/etc/ld.so.conf生效
openssl version                                             #查看版本，看更新是否成功
#ldd /usr/bin/openssl发现使用的不是/openssl-1.1.1e/lib/目录，所以
#ln -s /usr/local/openssl-1.1.1e/lib/libssl.so.1.1 /usr/lib/x86_64-linux-gnu/libssl.so.1.1
#ln -s /usr/local/openssl-1.1.1e/lib/libcrypto.so.1.1 /usr/lib/x86_64-linux-gnu/libcrypto.so.1.1
```

# Install boost
Build and install Boost 1.70. Adjust `-j10` to match your machine. Bad things will
happen if you don't have enough RAM for the number of cores you use:

```
cd ~
# https://boostorg.jfrog.io/native/main/release 
wget https://boostorg.jfrog.io/artifactory/main/release/1.77.0/source/boost_1_77_0.tar.gz
tar xf boost_1_77_0.tar.gz
cd boost_1_77_0
./bootstrap.sh
./b2 toolset=clang -j10 install
```

Build and install CMake 3.21.1. Adjust `--parallel=` and `-j` to match your machine.
Bad things will happen if you don't have enough RAM for the number of cores you use:

# Install cmake

```
cd ~
wget https://github.com/Kitware/CMake/releases/download/v3.21.1/cmake-3.21.1.tar.gz
tar xf cmake-3.21.1.tar.gz
cd cmake-3.21.1
./bootstrap --parallel=10
make -j10
make -j10 install
```

# Install crypto++
```
mkdir tmp && cd tmp
git clone https://github.com/weidai11/cryptopp.git
git clone https://github.com/noloader/cryptopp-pem.git
cp cryptopp-pem/* cryptopp/ #把.h .cpp文件复制过去，cryptopp会编译所以的cpp，安装的时候会把所有.h文件安装
cd cryptopp
make distclean
make -j 9
make install
```

# Install mysql++

```
apt install -y libmysqlclient-dev
cd ~
wget https://tangentsoft.com/mysqlpp/releases/mysql++-3.2.4.tar.gz
tar -zxvf mysql++-3.2.4.tar.gz
cd mysql++-3.2.4
./configure --prefix=/usr/opt/mysql++ --with-mysql-lib=/usr/lib/x86_64-linux-gnu
make
make install
find . -name "*.o" | xargs -i ar rcu libmysqlpp.a {}
cp libmysql*.a /usr/opt/mysql++/lib/
```


# Build hbapps

```
cd ~
git clone --recursive https://127.0.0.1.git
cd hbapps
mkdir build
cd build
cmake -DCMAKE_CXX_COMPILER=clang++-8 -DCMAKE_C_COMPILER=clang-8 ..
make
```
