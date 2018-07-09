# basmat - Bashicu Matrix Calculator

This program calculates Bashicu Matrix. Bashicu matrix system is a notation designed to produce large numbers. Bashicu matrix is a matrix such as ![BM](http://gyafun.jp/ln/basmat/eq1.png "BM") where all elements are nonnegative integers. The matrix can be written in the form of (a<sub>11</sub>, a<sub>21</sub>)(a<sub>12</sub>, a<sub>22</sub>)(a<sub>13</sub>, a<sub>23</sub>); sequence of transpose of each column. With an algorithm invented by Bashicu in 2014 and updated afterwards, Bashicu matrix BM works as a function from a natural number n to a natural number BM[n] (provided that the calculation ends), and written as (0,0)(1,1)(1,1)[3]. It is known that 2-row matrix, pair sequence,

## Installation

Follow the normal UNIX installation procedure, such as:

    ./configure && make
    su
    make install

Basmat runs on Bash on ubuntu on Windows. Just run the following beforehand.

    sudo apt-get install gcc make

When you are using homebrew on macOS, it is easy to install by

    brew install kyodaisuu/math/basmat

## Usage

    basmat [-v ver] [-o opt] [-s seq] [-t stp] [-dh] ini

* ini - Initial variable in the form of BM[n], 
    * BM sequence expression of Bashicu matrix.
    * n: natural number. n=2 if not given.
    * ex. "(0,0)(1,1)[3]"   (Quote to escape brackets in shell)
* ver - Version of Bashicu matrix system. Default = 3.
    * Available versions: 1, 2, 2.1, 2.2, 3
    * See [basmat version](https://googology.wikia.com/wiki/User:Kyodaisuu/BasmatVersion) for detail
* opt - Calculation option.
    * opt = 1: n is constant. (Default)
    * opt = 2: n = n+1 for each loop.
    * opt = 3: n = n*n for each loop.
    * opt = 4: Simulate Hardy. n=n+1 for successor, and copy n-1.
* seq - Maximum length of sequence. Default = 20.
* stp - Maximum step of calculation. Default = 0 (no limit).

*  -d  Show detailed process of calculation.
*  -T  Test for development.
*  -h  Show this help and exit.

Example:

     basmat -d "(0)(1)(2)[3]"
     basmat -o 2 -s 100000 "(0,0)(1,1)[3]"
     basmat -o 4 -s 100000 "(0,0)(1,1)[3]"
     basmat -o 1 -s 13000 -t 100000 "(0,0)(1,1)[3]"

## Version History

See ChangeLog

## Web interface

[Web interface](http://gyafun.jp/ln/basmat.cgi) is available.

The online version of basmat has some limit on the length of initial variable, the maximum length <= 10000, and maximum step of calculation = 1000.

## Authors

* [Bashicu](http://ja.googology.wikia.com/wiki/User:BashicuHyudora) invented versions 1,2,3 and wrote BASIC program
* [Fish](http://googology.wikia.com/wiki/User:Kyodaisuu) wrote this C program using Bashicu's algorithm
* [koteitan](https://twitter.com/koteitan) invented and wrote C program of versions 2.1, 2.2
