FROM manjarolinux/build:latest

RUN pacman -Syyu \
    && pacman -Syu --noconfirm base-devel \
    cmake \
    cppcheck \
    valgrind \
    gdb \
    ninja

CMD ["/bin/sh"]
