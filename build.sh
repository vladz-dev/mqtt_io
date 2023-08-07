#!/bin/bash

cmake --list-presets=all .

cmake --preset "debug" --debug-find-pkg=Qt6Mqtt
cmake --build --preset "Build-Debug" --debug-find-pkg=Qt6Mqtt