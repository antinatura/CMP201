# CMP201: Data Structures and Algorithms 1 

### Assessment Unit 1 - Software Project

> In this assessment, you will demonstrate, using a software project and presentation, your achievement of the module's learning outcomes:
> * Describe abstract and concrete requirements for data structures and algorithms.
> * Describe a range of standard data structures and algorithms, in terms of both functionality and performance characteristics.
> * By reasoning about behaviour and performance, be able to critically select appropriate data structures and algorithms for a given application within a software project.

##### 1. Guidelines

> Your application must:
> * implement two different standard algorithms that solve the same real-world problem;
> * make use of appropriate data structures for the application's needs;
> * allow you to compare the performance of the two algorithms as you vary the size of the input data

##### 2. My implementation

Boyer-Moore-Horspool and Rabin Karp string matching algorithms that seaarch for a pattern in a network traffic log file.
The pattern and the source file can be changed by the user.

1. **source.cpp** contains both algorithms and main.
2. **util.cpp** and **util.h** contain a helper function for reading a file.
3. The **files** directory contains .csv files that were used for benchmarking this project:
    - log files of various lenghts to search for a pattern in,
    - **patterns.csv** with patterns to search for.

***
*Data contained in the .csv files has been edited to not reflect real IP addresses as it is based on a real network traffic log file.*
