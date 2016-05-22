# libxml


A C++ XML library coded in a standard-like way

## Purpose

The purpose of this library os to provide an out-of-the-box, straightforward XML library written in C++11.
Its interface is (supposed to be) close to the standard C++ libraries.

## Install

Run the following commands to install the library :

```
$ git clone https://github.com/TheoGoudout/libxml
$ cd libxml
$ mkdir build
$ cd build
$ cmake ..
$ make
$ make install
```

By default, the library, its headers, manual and example programs will be installed. You can configure `cmake` to change that behaviour. Using `ccmake` will give you all the available options for your build.