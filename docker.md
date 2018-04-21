docker info
docker version 
docker search tornado
docker search -s 5 tornado
docker pull docker.io/centos
docker ps
docker ps -a
docker images

docker run -it docker.io/centos  env
docker run -it docker.io/centos  /bin/bash
docker top c42a8037264b
docker inspect c42a8037264b
docker stop c42a8037264b 


docker exec -it c42a8037264b /bin/bash

docker run --rm=true -v /mnt/docker_test/ -it docker.io/centos 
docker run --rm=true -v /mnt/docker_test/ -it docker.io/centos /bin/bash
docker run -it --rm=true -v /mnt/docker_kkk/:/kkk  centos /bin/bash

docker run --rm=true --privileged=true --volumes-from=c42a8037264b -it centos /bin/bash

docker默认是允许container互通，通过-icc=false关闭互通。一旦关闭了互通，只能通过-link name:alias命令
连接指定container. 
-- link redis:db的别名，会在/etc/hosts中生成对应的ip映射
docker run --rm=true --name=myjavaserver:javaserver1 -it java /bin/bash
docker daemon --icc=false --iptables=true







