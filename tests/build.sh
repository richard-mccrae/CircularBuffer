#!/bin/bash

EXE_NAME="TestRunner"

IUT_REL_PATH="../src/"
IUT_SRC=$IUT_REL_PATH"CircularBuffer.c"

TEST_SRC="TestCircularBuffer.c TestRunner.c"

UNITY_PATH="/usr/local/lib/Unity"
UNITY_SRC_MAIN=$UNITY_PATH"/src/unity.c"
UNITY_SRC_FIXTURE=$UNITY_PATH"/extras/fixture/src/unity_fixture.c"
UNITY_SRC_LIST=$UNITY_SRC_MAIN" "$UNITY_SRC_FIXTURE

SRC_LIST=$IUT_SRC" "$TEST_SRC" "$UNITY_SRC_LIST

gcc -Wall -o $EXE_NAME $SRC_LIST