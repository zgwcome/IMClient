# 使用Ubuntu作为基础镜像
FROM ubuntu:22.04
 
# 安装构建所需的依赖
RUN apt-get update && apt-get install -y \
    cmake \
    g++ \
    make \
    libcrypto++-dev zlib1g-dev\
    && rm -rf /var/lib/apt/lists/*

# 设置工作目录
WORKDIR /app

# 复制程序源代码到容器中
COPY . .

# 构建应用程序
RUN mkdir build && cd build && cmake .. && make
# 设置容器启动时执行的命令
CMD ["./client"]
