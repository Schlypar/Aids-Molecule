#!/bin/bash

make -e PROGRAM=bin/debug.exe

valgrind ./bin/debug.exe
