# RSSearch

![](https://github.com/mistydew/RSSearch/workflows/C/C++%20CI/badge.svg)

A RSS search engine with spell correction mechanism.

## Todo

- [ ] remove dir: data, depends, dict, lib and res
- [ ] code refactoring: config system (remove json)

## To Build

```
$ git clone https://github.com/mistydew/RSSearch.git
$ cd RSSearch
$ make
$ make install # optional
```

## Dependencies

These dependencies are required:

Library  | Purpose       | Description
---------|---------------|-------------
Boost    | Utility       | Library for threading, data structures, etc.
CppJieba | Segmentation  | Chinese word segmentation
Simhash  | Deduplication | Remove duplicate
Redis    | Database      | Data storage, Cache
TinyXML2 | XML parsing   | XML parser

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
$ ./createlib ~/RSSearch/createlib.conf
$ ./searchengined ~/RSSearch/searchengine.conf
```

## License

RSSearch is released under the terms of the MIT license.
See [LICENSE](LICENSE) for more information or see https://opensource.org/licenses/MIT.
