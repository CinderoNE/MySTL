#! /bin/sh

set -x

SOURCE_DIR=`pwd`
BUILD_TYPE=${BUILD_TYPE:-Debug}
BUILD_DIR=${SOURCE_DIR}/build

mkdir -p $BUILD_DIR \
    && cd $BUILD_DIR \
    && cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE $SOURCE_DIR \
    && make $*