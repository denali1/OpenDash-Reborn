#!/bin/bash

BUILD=`cat build.txt`
let "BUILD += 1"
echo $BUILD > build.txt
echo $BUILD
