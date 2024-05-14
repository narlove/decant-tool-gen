FROM ubuntu:22.04

RUN apt-get update; \
    apt-get install -y clang cmake;

COPY src ./src
COPY CMakeLists.txt .
COPY reqs ./reqs
COPY target ./target

RUN mkdir build

WORKDIR /build

RUN cmake -DCMAKE_BUILD_TYPE=Release .. && \
    cmake --build . --parallel 8

CMD ["./DecantTool"]