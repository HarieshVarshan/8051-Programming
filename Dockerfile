# 8051 Development Environment
# Uses SDCC (Small Device C Compiler) for 8051/8052 microcontroller programming

FROM ubuntu:22.04

LABEL maintainer="8051-Programming"
LABEL description="Development environment for 8051 microcontroller programming with SDCC"

# Avoid interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Install SDCC and build tools
RUN apt-get update && apt-get install -y \
    sdcc \
    sdcc-doc \
    sdcc-libraries \
    make \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /workspace

# Default command
CMD ["/bin/bash"]
