# rssSearchEngine

![](https://github.com/mistydew/rssSearchEngine/workflows/C/C++%20CI/badge.svg)
[![Build Status](https://travis-ci.com/mistydew/rssSearchEngine.svg?branch=master)](https://travis-ci.com/mistydew/rssSearchEngine)

A mini search engine with rss.

## Todo

- [ ] remove json, remake conf file system

## To Build

```
$ git clone https://github.com/mistydew/rssSearchEngine.git
$ cd rssSearchEngine
$ make
$ make install # optional
```

## Dependencies

These dependencies are required:

Library  | Purpose       | Description
---------|---------------|-------------
libboost | Utility       | Library for threading, data structures, etc.
cppjieba | Segmentation  | Chinese word segmentation
simhash  | Deduplication | Remove duplicate
Redis    | Redis         | Data storage, Cache

## Linux Distribution Specific Instructions

### Ubuntu

#### Dependency Build Instructions

Build requirements:

##### Redis

```
$ sudo apt install redis-server
```

## Usage

```
$ ./createlib ~/rssSearchEngine/createlib.conf
$ ./searchengined ~/rssSearchEngine/searchengine.conf
```

## License

rssSearchEngine is released under the terms of the MIT license.
See [LICENSE](LICENSE) for more information or see https://opensource.org/licenses/MIT.
