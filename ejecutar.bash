#!/usr/bin/env bash
if [ ! -d compiled ]; then
    mkdir compiled
else
    rm -r ./compiled/*
fi

cd compiled
cmake ..
make

cp ../keywords .
cp ../regression.d .

./analizadorLexico
cd ..
