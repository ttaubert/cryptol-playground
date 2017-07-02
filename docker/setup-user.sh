#!/usr/bin/env bash

set -v -e -x

# Install SAW.
mkdir ~/bin/saw
curl -LO https://saw.galois.com/builds/nightly/saw-0.2-2017-06-24-Ubuntu14.04-64.tar.gz
tar xvzf saw*.tar.gz -C ~/bin/saw --strip-components=1
rm saw*.tar.gz

# Install Cryptol.
mkdir ~/bin/cryptol
curl -LO https://saw.galois.com/builds/nightly/cryptol/cryptol-2.4.0-2017-06-24-Ubuntu14.04-64.tar.gz
tar xvzf cryptol*.tar.gz -C ~/bin/cryptol --strip-components=1
rm cryptol*.tar.gz
