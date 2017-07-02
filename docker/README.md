# cryptol-saw-docker
A Docker image template to experiment with [SAW](https://saw.galois.com/) and [Cryptol](https://cryptol.net/).

## Usage

First, build the Docker image locally.

```
docker build -t cryptol-saw .
```

When that's done you can use the `saw` and `cryptol` binaries to experiment with SAW and/or Cryptol.

```
docker run -ti cryptol-saw bash
saw >
   ___  __ _ _ _ _
  / __|/ _' | | | |
  \__ \ (_| | | | |
  |___/\__,_|\_,_/ version 0.2 (f95efd7)

Loading module Cryptol
sawscript>
```
