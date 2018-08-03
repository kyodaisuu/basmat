# Installation

## Windows

Basmat runs on Windows Subsystem for Linux on Windows 10.

* [Install your Linux distribution of choice](https://docs.microsoft.com/en-us/windows/wsl/install-win10) following the instruction.
* Run the following in the bash shell (assuming that you chose Utuntu or Debian).
```
sudo apt install gcc make wget
```
* Download and unpack the latest development file by
```
wget https://github.com/kyodaisuu/basmat/tarball/master -O basmat.tar.gz
tar xfvz basmat.tar.gz
```
* Go to the directory of kyodaisuu-basmat-*** by
```
cd kyodaisuu-basmat***
```
* Install basmat by
```
./configure && make
sudo make install
```

## macOS

* Install [Homebrew](https://brew.sh/)
* Install latest release of basmat by
```
brew install kyodaisuu/math/basmat
```
* You can upgrade to the latest version of basmat and other programs installed by homebrew by
```
brew upgrade
```

## Unix system

* [Download the tar ball](https://github.com/kyodaisuu/basmat/tarball/master) of latest development version and unpack it.
* Go to the directory and follow the normal UNIX installation procedure, such as
```
./configure && make
su
make install
```

----
[Home](index.md)
