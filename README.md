# HI3-Manhua-C-Downloader
HMCD is a C program to download manhuas from the official HI3 COMIC, CN site (https://comic.bh3.com/) and GLOBAL site (https://manga.honkaiimpact3.com/).

## How to get it
Just go [there](https://github.com/JeFaitDesSpaghettis/HMCD/releases) or build it yourself.

## Build
To build you'll git cmake and curl development packages.<br/>
Then run these commands:<br/>
&nbsp;&nbsp;&nbsp;&nbsp;`git clone "https://github.com/JeFaitDesSpaghettis/HMCD"`<br/>
&nbsp;&nbsp;&nbsp;&nbsp;`cd "./HMCD"`<br/>
&nbsp;&nbsp;&nbsp;&nbsp;`cmake -G"MinGW Makefiles" -B"./build" -S"./"`<br/>
&nbsp;&nbsp;&nbsp;&nbsp;`cmake --build "./build" -j8`<br/>
&nbsp;&nbsp;&nbsp;&nbsp;`cmake --build "./build" --target install` (optional)<br/>
&nbsp;&nbsp;&nbsp;&nbsp;`cmake --build "./build" --target uninstall` (to uninstall)<br/>

## Explanation
The url of a page uses the following scheme:<br/>
`[XXXXXXXX]/comic/book/[BOOK_ID]/[CHAPTER]/[PAGE].jpg`<br/>
Where:<br/>

+ `XXXXXXXX` is the cdn, it can be either<br/>
    "https://d2tpbmzklky1cl.cloudfront.net/manga/static" (global)<br/>
    OR<br/>
    "https://comicstatic.bh3.com/new_static_v2" (china)<br/>
+ `BOOK_ID` is an 4 digit integer starting from 1001(and growing up) indicating thebook<br/>
* `CHAPTER` is a non zero padded integer starting at 1 indicating the chapter<br/>
+ `PAGE` is zero padded 4 digit integer starting at 0001<br/>

They will be then downloaded using the following scheme:<br/>
`[OUT_DIR]_[BOOK_ID]/Chapter[CHAPTER]/[CHAPTER][PAGE].jpg`<br/>
Where:<br/>
+ `OUT_DIR` is either "./GBBook" or "./CNBook"<br/>
* `BOOK_ID` is an 4 digit integer starting from 1001(and growing up) indicating the book<br/>
+ `CHAPTER` is a zero padded 2 digit integer starting at 1 indicating the chapter<br/>
* `CHAPTER` is reused again in the page file name for having all pages in one directory<br/>
    without having the page names non conflicting<br/>
* `PAGE` is zero padded 2 digit integer starting

## Why?
It allows you to download the official hi3 manhuas releases in a easy way. \
Also today everything needs internet and that is pretty annoying, one of the biggest problems about HI3 (and games in general) is the fact that it needs constant internet connection, and also game file encryption, if suddenly miHoYo shutdown or this game is blocked you can't do anything \
Sooo i wrote a script to download HI3 Manhuas and download them on whatever device that supports libCURL, Windows, GNU-Linux or Android through Termux. \
So that we can at least keep this part of HI3!
