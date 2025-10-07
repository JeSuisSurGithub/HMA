# HI3-Manhua-Archiver
HMA is a C program to download manhuas from the official HI3 COMIC, chinese site (https://comic.bh3.com/) and international site (https://manga.honkaiimpact3.com/).

## How to get it
Just go [there](https://github.com/JeSuisSurGithub/HMA/releases) or build it yourself.

## Build
To build you'll [git](https://git-scm.com/) [xmake](https://xmake.io/) and [curl](https://curl.se/) development package.<br/>
Then run these commands:<br/>
&nbsp;&nbsp;&nbsp;&nbsp;`git clone https://github.com/JeSuisSurGithub/HMA`<br/>
&nbsp;&nbsp;&nbsp;&nbsp;`cd HMA`<br/>
&nbsp;&nbsp;&nbsp;&nbsp;`xmake f -m release --toolchain=gcc"`<br/>
&nbsp;&nbsp;&nbsp;&nbsp;`xmake -w -j $(nproc)`<br/>
&nbsp;&nbsp;&nbsp;&nbsp;`sudo xmake install` (optional)<br/>
&nbsp;&nbsp;&nbsp;&nbsp;`sudo xmake uninstall` (to uninstall)<br/>

## Usage
`Usage : hma-cli [OPTIONS]`<br/>
&nbsp;&nbsp;&nbsp;&nbsp;`NO ARGS                 Launches guided mode`<br/>
&nbsp;&nbsp;&nbsp;&nbsp;`-h, --help              Prints out this help`<br/>
&nbsp;&nbsp;&nbsp;&nbsp;`-s, --server[int]       1 = MAINLAND CHINA, 2 = GLOBAL`<br/>
&nbsp;&nbsp;&nbsp;&nbsp;`-b, --book[int]         The 4-digit integer on the HI3 COMIC Official site when you read a specific book i.e: http://....com/book/[BookID]`<br/>
&nbsp;&nbsp;&nbsp;&nbsp;`-f, --first[int]        First chapter to download, optional`<br/>
&nbsp;&nbsp;&nbsp;&nbsp;`-l, --last[int]         Last chapter to download, optional`<br/>
&nbsp;&nbsp;&nbsp;&nbsp;`-n, --quiet             No output messages, optional`<br/>
&nbsp;&nbsp;&nbsp;&nbsp;`-o, --onedirectory      All pages in one directory, optional`<br/>

## Workings
The url of a page uses the following format:<br/>
`[CDN_BASE_URL]/comic/book/[BOOK_ID]/[CHAPTER]/[PAGE].jpg`<br/>
Where:<br/>

* `CDN_BASE_URL` is the CDN's base url(duh), it can be either<br/>
    `https://act-webstatic.hoyoverse.com/manga/static` (global)<br/>
    or<br/>
    `https://act-webstatic.mihoyo.com/new_static_v2` (china)<br/>
+ `BOOK_ID` is an 4 digit integer starting from 1001(and growing up)<br/>
* `CHAPTER` is a non zero padded integer starting at 1<br/>
+ `PAGE` is zero padded 4 digit integer starting at 0001<br/>

Each page downloaded is saved using one of the following path format depending on options passed in the command line:<br/>
`hmaoutput/[SERVER_NAME]/[BOOK_ID]/[CHAPTER]/[CHAPTER+PAGE].jpg`<br/>
or<br/>
`hmaoutput/[SERVER_NAME]_[BOOK_ID]_[CHAPTER+PAGE].jpg`<br/>