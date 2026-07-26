#!/bin/bash


if [ ! -f "bin_string" ]; then
    make
fi


./bin_string

rm bin_string