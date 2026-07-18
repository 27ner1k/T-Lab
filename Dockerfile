FROM ubuntu:22.04

RUN apt-get update && \
    apt-get install -y --no-install-recommends g++ make && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY src/ ./src/

RUN g++ -std=c++17 -O2 -fopenmp src/main.cpp -o main

CMD ["./main"]