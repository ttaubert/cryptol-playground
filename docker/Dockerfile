FROM ubuntu:17.04
MAINTAINER Tim Taubert <tim@timtaubert.de>

RUN useradd -d /home/saw -s /bin/bash -m saw
WORKDIR /home/saw

# Install dependencies.
ADD setup.sh /tmp/setup.sh
RUN bash /tmp/setup.sh

# Change user.
USER saw
RUN mkdir /home/saw/bin

# Install dependencies.
ADD setup-user.sh /tmp/setup-user.sh
RUN bash /tmp/setup-user.sh

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

# Cryptol and SAW binaries.
ENV PATH "${PATH}:/home/saw/bin/saw/bin:/home/saw/bin/cryptol/bin"

# Set a default command for debugging.
CMD ["/bin/bash", "--login"]
