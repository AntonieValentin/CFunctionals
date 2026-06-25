# C Functionals

This project implements a small functional-style utility layer in C, built on top of generic homogeneous arrays. The goal is to recreate common higher-order operations such as `map`, `filter`, `reduce`, and `for_each`, while still working with manual memory management and `void *` data.

The project is a good exercise in generic programming in C, function pointers, variadic functions, and disciplined ownership of dynamically allocated memory.

## Core Idea

The main data structure is `array_t`, a generic list-like container that stores:

* a pointer to a contiguous memory block;
* the size of one element;
* the number of elements;
* an optional destructor for freeing complex elements.

Using this structure, the project simulates functional lists while keeping the implementation close to how memory actually works in C.

## Implemented Functionals

The project includes the basic functional operations:

* `for_each` - applies a function to every element;
* `map` - builds a new list from an existing one;
* `filter` - keeps only the elements that satisfy a condition;
* `reduce` - folds a list into an accumulator.

It also includes versions that work on multiple lists at the same time:

* `for_each_multiple`;
* `map_multiple`;
* `reduce_multiple`.

These variants use variadic functions and process the input lists in parallel, stopping at the length of the shortest list.

## Practice Tasks

The functional utilities are then used to solve several tasks, such as:

* reversing a list of integers;
* creating a list of structured numbers from integer and fractional parts;
* extracting the names of students who passed;
* checking sums against expected values;
* keeping strings from even indexes;
* generating a square matrix represented as a list of lists.

The tasks are meant to be solved using the implemented functionals, not with regular loops inside the task logic.

## Build and Run

The project can be built using the Makefile setup.

A typical local workflow is:

```bash id="buuvgs"
make
```

## Notes

Most of the difficulty comes from combining a functional programming style with C's low-level memory model. Functions like `map` and `filter` create new arrays and free old ones when required, so ownership rules are an important part of the implementation.

The project avoids global variables and keeps the task solutions based on the reusable functional operations.
