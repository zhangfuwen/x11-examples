#!/bin/bash

sudo cp lotime.xml /usr/share/ibus/component
sudo pkill /usr/bin/ibus-lotime
sudo cp ./build/fake_ime /usr/bin/ibus-lotime

ibus-daemon -Rr