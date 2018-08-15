# Web interface of Bashicu Matrix Calculator

This directory contains source code of the cgi program at
http://gyafun.jp/ln/basmat.cgi

## System requirement

* Apache with cgi (mod_cgi) enabled.
* Perl
* C program should be compiled on server

## How to set up

* Install basmat on your server.
* Put basmat.cgi and help.txt on your server.
* Edit the first line of basmat.cgi to specify the full path of perl.
* Edit the parameters $url, $basmat and $help in the initial setting section of basmat.cgi.
* Make basmat.cgi executable as cgi. Refer [apache tutorial](https://httpd.apache.org/docs/current/en/howto/cgi.html).

## Pull request

Send pull request when you find something to be fixed. It may be reflected on http://gyafun.jp/ln/basmat.cgi
