<!-- docker pull 32bit/ubuntu:16.04 #没有32位的18.04 -->
docker pull ubuntu:18.04
docker run --name hbapp_dev -d -v /Users/liweidong/work/mygit:/root/hbapp_dev -p 8899:8899 -it ubuntu:18.04 /bin/bash
docker exec -it hbapp_dev bash