# basmat - Bashicu Matrix Calculator

This program calculates Bashicu Matrix. See http://gyafun.jp/ln/basmat.cgi for detail.

## Installation

Follow the normal UNIX installation procedure, such as:

    ./configure && make
    su
    make install

Basmat runs on Bash on ubuntu on Windows. Just run the following beforehand.

    sudo apt-get install gcc make

## Usage

    basmat [-v ver] [-o opt] [-s seq] [-t stp] [-dh] ini

* ini is Initial variable in the form of BM[n], 
    * BM sequence expression of Bashicu matrix.
    * n: natural number. n=2 if not given.
    * ex. "(0,0)(1,1)[3]"   (Quote to escape brackets in shell)
* ver is Version of Bashicu matrix system. Default = 3.
    * Available versions: 1, 2, 2.1, 2.2, 3
    * See https://googology.wikia.com/wiki/User:Kyodaisuu/BasmatVersion for detail
* opt is Calculation option.
    * opt = 1: n is constant. (Default)
    * opt = 2: n = n+1 for each loop.
    * opt = 3: n = n*n for each loop.
    * opt = 4: Simulate Hardy. n=n+1 for successor, and copy n-1.
* seq is Maximum length of sequence. Default = 20.
* stp is Maximum step of calculation. Default = 0 (no limit).

*  -d  Show detailed process of calculation.
*  -h  Show this help and exit.

Example:

     basmat -d "(0)(1)(2)[3]"
     basmat -o 2 -s 100000 "(0,0)(1,1)[3]"
     basmat -o 4 -s 100000 "(0,0)(1,1)[3]"
     basmat -o 1 -s 13000 -t 100000 "(0,0)(1,1)[3]"

## Version History

See ChangeLog

## Web interface

Web interface is available at
http://gyafun.jp/ln/basmat.cgi

## Author

Fish http://googology.wikia.com/wiki/User:Kyodaisuu
