//
//  mem.h
//  bits
//
//  Created by Sean Allred on 6/28/12.
//  Copyright (c) 2012 St. Mary's College of Maryland. All rights reserved.
//

#ifndef bits_mem_h
#define bits_mem_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
 *  mem.h
 *  BITS
 *
 *  Created by Sean Allred on 5/18/12.
 *  Copyright 2012 St. Mary's College of Maryland. All rights reserved.
 *
 */


#define SET_CONST(TYPE, CONST, VAL) *(TYPE*)&CONST = VAL;


#define PRINT_MEM__USE_HEADER 1
#define PRINT_MEM__ADDRESS_PREFIX 2
#define PRINT_MEM__LINE_DELIMITED 4
#define PRINT_MEM__USE_DECIMAL 8
#define PRINT_MEM__USE_BINARY 16
#define PRINT_MEM__RAW 32

enum PRINT_MEM_FORMAT {
    PM_PLAIN_HEX = 0,
    PM_PLAIN_BITSTRING = PRINT_MEM__USE_BINARY,
    PM_PLAIN_BYTES = PRINT_MEM__USE_DECIMAL,
    PM_DETAILED_HEX = PRINT_MEM__USE_HEADER | PRINT_MEM__LINE_DELIMITED,
    PM_DETAILED_BYTES = PRINT_MEM__USE_HEADER | PRINT_MEM__LINE_DELIMITED | PRINT_MEM__USE_DECIMAL,
    PM_DETAILED_BITSTRING = PRINT_MEM__USE_HEADER | PRINT_MEM__LINE_DELIMITED | PRINT_MEM__USE_BINARY,
    PM_PLAIN_RAW = PRINT_MEM__RAW,
    PM_DETAILED_RAW = PRINT_MEM__RAW | PRINT_MEM__USE_HEADER | PRINT_MEM__LINE_DELIMITED
};

typedef unsigned int Uint;
typedef unsigned long Ulong;
typedef unsigned char Byte;
typedef void * Address;
typedef char * String;
#define new_String(a) malloc(a * sizeof(char))
typedef unsigned int Word;
typedef FILE* Stream;
typedef enum PRINT_MEM_FORMAT MemoryPrintFormat;
typedef struct __STRUCT___RANGE_T {
    int start;
    size_t length;
} range_t;

size_t null_term_arr_len(void **);
void null_term_arr_free(void **);

String *split_str(String s, char delim);
void escape_string(String s);

// implements negative indices Python-style
size_t smart_index(int idx, size_t max);

/* Prints raw byte data from the memory location #1 up to (#1+#2)
 // Arguments:
 //	#1 : an address to start printing from
 //	#2 : an address to halt printing at (exclusive)
 //	#3 : a series of bitwise flags (prefix:PRINT_MEM__)
 */
void fprint_memory(Stream, Address, size_t, MemoryPrintFormat);
void sprint_memory(String, Address, size_t, MemoryPrintFormat);

/* Fills a C string containing the bits of a given byte
 //	#1 : a buffer to store it in
 //	#2 : the byte to convert
 */
void  get_bits      (String, Byte);
void  get_long_bits (String, long, Uint);
/* Writes data given in #2 starting at the memory location given in #1.
 // Arguments:
 //	#1 : an address to start writing to
 //	#2 : a value to write
 */
void   write_byte    (Address, Byte);
void   write_double  (Address, double);
void   write_float   (Address, float);
void   write_integer (Address, int);

// Reads one value starting at the memory location given in #1.
// Arguments:
//	#1 : an address to start reading from
Byte   read_byte     (Address);
double read_double   (Address);
float  read_float    (Address);
int    read_integer  (Address);
unsigned int read_word (Address start, size_t offset, size_t wordSize);
void print_int(int);

Byte _rotl(Byte value, int shift);

Byte _rotr(Byte value, int shift);

Byte get_bit(Address, Byte);

Byte min_bits(Ulong);

#endif
