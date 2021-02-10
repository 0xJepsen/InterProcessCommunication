README
======

This package includes the following files.

|-- Coordinator.c [This is the driver program In which processes are forked
and checker is called in the forked processes. Coordinator takes 5 arguments]
|-- Makefile [A make file that performs make clean and make all]
|-- Checker.c [This is where I have writtern my checker method. Checker takes
3 arguments]
|-- README.txt [This file]

To compile:
    make all
To clean:
    make clean
To run:
    ./coordinator <arguments>
For example;
    ./coordinator 3 3 20 49 102
    ./coordinator 7 32 49 846 22344