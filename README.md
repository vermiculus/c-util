# Purpose
This repository contains utility classes and data structures in C. I find that I usually like to write and customize my own tools for my use - many tools out there are far too bloated with things I don't have a need for.

These C classes (yes, classes) are the bare bones that I see as absolutely necessary for it. You are free to customize them as you see fit, but give me credit where it is due (feel free to point out your improvements/additions).

# Layout
## Heirarchy
While a file system has not been made for this repository yet, it will be made.

## Source
Each class is used in the following manner:

	// You only need to include the header file.
	#include "relative/path/to/ClassName.h"
	
	// This declares an instance of ClassName called `instance`
	ClassNameRef instance;
	
	// This creates the space needed to store instance data
	// Note class methods are stored in the struct `ClassName`
	instance = ClassName.alloc();
	
	// This initializes necessary members of the instance
	ClassName.init(instance);
	
	// All class methods have the instance as the first argument
	ClassName.doMethod(instance, arg1, arg2);
	
	// This frees **all** space used by the instance, including itself
	ClassName.dealloc(instance);

In the source, note the implementation of object orientism:

	typedef struct __STRUCT__CLASSNAME__MODEL * ClassNameRef;
	
	struct __STRUCT__CLASSNAME__METHODS {…} ClassName;

Note that no data members of any object are truly private. However, an initial `_` will usually suffice.
