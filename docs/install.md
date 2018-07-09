# Installation

## Windows

Basmat runs on Windows Subsystem for Linux on Windows 10.

* [Install your Linux distribution of choice] (https://docs.microsoft.com/en-us/windows/wsl/install-win10) following the instruction.
* Run the following in the bash shell.

    sudo apt-get install gcc make

* Download the latest distribution of basmat from [release](https://github.com/kyodaisuu/basmat/releases). Download the archive of tar.gz.
* Unpack the file by

    tar xfvz vx.y.tar.gz

* Go to the directory of basmat-x.y.tar.gz by

    cd basmat-x.y.tar.gz

* Install basmat by

    ./configure && make
    sudo make install

## macOS

* Install [Homebrew](https://brew.sh/)
* Install basmat by

    brew install kyodaisuu/math/basmat

## Unix system

Follow the normal UNIX installation procedure, such as:

    ./configure && make
    su
    make install

----
[Home](index.md)
