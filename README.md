# keyremaplinux
Work in progress.

Define your keyboard layout as C++ class.

My personal notes, will refine the later:

To run on mac:

docker-machine create --driver=virtualbox --virtualbox-memory 2048 --virtualbox-cpu-count 2 dev

docker build -t keyremaplinux .

docker run -v ~/github/keyremaplinux:/keyremaplinux -i -t -P keyremaplinux:latest /bin/bash

Try:
http://fabiorehm.com/blog/2014/09/11/running-gui-apps-with-docker/
