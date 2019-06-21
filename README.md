# rssSearchEngine

A mini search engine with rss.

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
simhash  | Deduplication | Duplicate removal
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
