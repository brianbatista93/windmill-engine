ARG VARIANT=debian-11
FROM mcr.microsoft.com/vscode/devcontainers/cpp:0-${VARIANT}

RUN apt-get -y update && export DEBIAN_FRONTEND=noninteractive \
     && apt-get -y upgrade