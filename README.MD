# CT

This document describes how to set the project in an eclipse environnement (tested on ubuntu 14.04 LTS)

## Purpose

CT is used to detect and track object from a video stream, counting them when they go beyong set a set line.

For instance you can count the number of car going through a lane both way, etc.

CT Uses a detector or a set of detectors to detect and track object. 

## OpenCV install

Download and install openCV in your machine : http://docs.opencv.org/3.1.0/d7/d9f/tutorial_linux_install.html#gsc.tab=0

## Dlib install

Download an dinstall dlib :

1. download -> http://dlib.net/
2. unpack dlib (you can unpack it anywhere we'll call this path $root)
3. Install dlib 

`cd $root/dlib`

`mkdir build`

`cd build`

`cmake ..`

`sudo make install`

`sudo ldconfig`

## Boost install

This will probably become optional very soon, but for now it is necessary.

In ubuntu type : sudo apt-get install : `sudo apt-get install libboost-all-dev`


## Eclipse setup

If you are using eclipse you may follow these steps: 

1. open eclipse
2. file -> import -> Git -> Projects from Git then hit next
3. choose Clone URI then hit next
4. Paste this URI in the URI field https://github.com/Vivien-/dlibCT.git
5. enter your username and password then hit next
6. the next view will show the branches, leave everything as ot is and hit next
7. choose a local directory where to clone the repository the hit next
8. choose import existing project then hit next
9. hit finish

that's all you can now build and run the project
