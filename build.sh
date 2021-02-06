#!/usr/bin/env bash

CFLAGS=""
if [ "$(uname)" == "Darwin" ]; then
    # We will compile on Mac OS X
    my_os=mac
    #CFLAGS += -D OSX
elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
    # We will compile on GNU/Linux
    my_os=linux
    #CFLAGS += -D LINUX
elif [ "$(expr substr $(uname -s) 1 10)" == "MINGW32_NT" ]; then
    # We will compile on Windows32 (UGH!)
    my_os=win32
    #CFLAGS += -D WIN32
elif [ "$(expr substr $(uname -s) 1 10)" == "MINGW64_NT" ]; then
    # We will compile on Windows64
    my_os=win64
fi

# Linux: Check if all build dependencies are met, else install them, prompt for sudo
if [ $my_os = "linux" ]; then
    is_ubuntu=$(cat /etc/os-release | grep "Ubuntu\|Debian")
    is_fedora=$(cat /etc/os-release | grep "Fedora\|Redhat\|CentOS") #todo test
    is_suse=$(cat /etc/os-release | grep "Suse\|Opensuse") #todo test
    REQUIRED_PKG="build-essential qtbase5-dev qttools5-dev librtaudio-dev libjack-jackd2-dev"
    PKG_OK=$(dpkg-query -W --showformat='${Status}\n' $REQUIRED_PKG | grep "install ok installed")
    echo Checking for $REQUIRED_PKG: $PKG_OK
    if [ "" = "$PKG_OK" ]; then
    echo "No $REQUIRED_PKG. Installing $REQUIRED_PKG. Please provide pw for sudo"
    if [ $is_ubuntu ];then
        sudo apt --yes install $REQUIRED_PKG 
        fi
    if [ $is_fedora ];then
        sudo dnf install $REQUIRED_PKG 
        fi
    if [ $is_suse ];then
        sudo zypper install $REQUIRED_PKG 
        fi
    fi
    # Now we have build requirements installed, let's compile!
    cmake -B./build/ds-client -S./src/ds-client/
    cd build/ds-client
    make
fi
