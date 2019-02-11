C++ Kd-Tree Library
===================

This is a standalone C++ version of the kd-tree implementation in the
Gamera framework that has been extended by a range search and has
been relicensed by the original author under a BSD style license.
It is written in plain C++98 and does not depend on any third party
libraries.

A kd-tree is a data structure that allows for nearest neighbor queries
in expected O(log(n)) time. The creation time of a kd-tree is O(n log(n)).
This library offers four additional features not commonly found in kd-tree
implementations:

 - kNN search with optional additional search condition (search predicate)
 - support for Euclidean, Manhatten, and Maximum distance and
   their weighted extensions (Euclidean is the default distance)
 - range query
 - nodes can store a pointer to arbitrary data as void*

Details about the design and usage of the library can be found in

> C. Dalitz: *Kd-Trees for Document Layout Analysis.*
> In C. Dalitz (Ed.): "Document Image Analysis with the Gamera Framework."
> Schriftenreihe des Fachbereichs Elektrotechnik und Informatik,
> Hochschule Niederrhein, vol. 8, pp. 39-52, Shaker Verlag (2009)

Please cite this article when using this library in a scientific project.


Usage of the library
--------------------

The library consists of two files:

 - `kdtree.hpp` contains the class declarations
 - `kdtree.cpp` contains the implementation and must be compiled

The sample program `demo_kdtree.cpp` shows how the following operations
are done:

 - creation of a kd-tree
 - k nearest neighbor search
 - k nearest neighbor search with additional search predicate
 - range query

***
**Note:**
The present implementation does *not* use the C++ template mechanism
for storing data points of only a specific dimension defined at compile time.
The dimension is instead determined at *run time* and the data points are
stored as nested vectors. This can slow down memory management considerably
when compiling in debug mode. It is thus advisable, to turn off debug flags
when compiling.

***

Authors & Copyright
-------------------

 - Christoph Dalitz, 2018, <https://lionel.kr.hsnr.de/~dalitz/data/kdtree/>
 - Jens Wilberg, 2018

For licensing information, see the file LICENSE for details.
