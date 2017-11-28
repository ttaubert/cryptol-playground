#!/usr/bin/env bash

set -v -e -x

# Update packages.
export DEBIAN_FRONTEND=noninteractive
apt-get -y update && apt-get -y upgrade

apt_packages=()
apt_packages+=('ca-certificates')
apt_packages+=('clang-3.8')
apt_packages+=('curl')
apt_packages+=('git')
apt_packages+=('locales')
apt_packages+=('silversearcher-ag')
apt_packages+=('unzip')
apt_packages+=('vim')

# Install packages.
apt-get install -y ${apt_packages[@]}

# Install Z3.
curl -LO https://github.com/Z3Prover/z3/releases/download/z3-4.5.0/z3-4.5.0-x64-ubuntu-14.04.zip
unzip z3*.zip
cp -r z3*/* /usr/local/
rm -fr z3*

# Install SAW/Cryptol.
curl -LO https://saw.galois.com/builds/nightly/saw-0.2-2017-11-26-Ubuntu14.04-64.tar.gz
tar xzvf saw-*.tar.gz -C /usr/local --strip-components=1
rm saw-*.tar.gz

locale-gen en_US.UTF-8
dpkg-reconfigure locales

# Cleanup.
rm -rf ~/.ccache ~/.cache
apt-get autoremove -y
apt-get clean
apt-get autoclean
rm $0
