FROM ubuntu:22.04 AS build

RUN apt-get update
RUN apt-get install -y build-essential libsdl1.2-dev libsdl-ttf2.0-dev

WORKDIR /simulation-coding-exercise

# COPY include/ ./include/
# COPY src/ ./src/
# COPY fonts/ ./fonts/
# COPY textures/ ./textures/
# COPY Makefile .
# RUN make