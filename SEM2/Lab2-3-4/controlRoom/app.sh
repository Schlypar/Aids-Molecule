#!/bin/bash

./controlRoom/build.sh

cd python

source env/bin/activate

python3 gui.py
