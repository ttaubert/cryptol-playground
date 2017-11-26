FROM ubuntu:17.04
MAINTAINER Tim Taubert <tim@timtaubert.de>

RUN useradd -d /home/saw -s /bin/bash -m saw
WORKDIR /home/saw

# Install dependencies.
ADD setup.sh /tmp/setup.sh
RUN bash /tmp/setup.sh

# Change user.
USER saw

# Env variables.
ENV HOME /home/saw
ENV SHELL /bin/bash
ENV USER saw
ENV LOGNAME saw
ENV HOSTNAME saw
ENV LANG en_US.UTF-8
ENV LC_ALL en_US.UTF-8
ENV HOST localhost
ENV DOMSUF localdomain

# Set a default command for debugging.
CMD ["/bin/bash", "--login"]
