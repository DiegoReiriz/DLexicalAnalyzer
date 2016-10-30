#!/usr/bin/env bash

# checks if the directory exists
if [ ! -d compiled ]; then
    # creates a directory
    mkdir compiled
else
    # if exists, deletes the content
    rm -r ./compiled/*
fi

# goes to the directory
cd compiled

# builds the application inside the directory
cmake ..
make

# copies the needed files
cp ../keywords .
cp ../regression.d .

# runs the program
./analizadorLexico

# goes to the parent directory
cd ..
