//
//  mem.c
//  bits
//
//  Created by Sean Allred on 6/28/12.
//  Copyright (c) 2012 St. Mary's College of Maryland. All rights reserved.
//
//  Contains direct memory manipulators and printers of various types.
//

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h> // needed for memcpy
#include <math.h> // needed for ceil, log2 (min_bits)
#include "mem.h"

size_t null_term_arr_len(void **arr) {
    size_t n = 0;
    void *this = arr[n];
    while (this != NULL) {
        this = arr[n++];
    }
    return n - 1; // because arr[n] is NULL - I don't care about the null terminator
}

void null_term_arr_free(void **arr) {
    int n = 0;
    while (arr[n] != NULL) {
        free(arr[n++]);
    }
    free(arr);
}

String * split_str(String s, char delim) {
    size_t len = strlen(s);
    String *ret = malloc(len * sizeof(String));
    int current_component_index = 0;
    int begin = 0;
    int i = 0;
    while (i <= len) {
        if (s[i] == delim) {
            ret[current_component_index] = malloc((i - begin) * sizeof(char));
            strncpy(ret[current_component_index], s + begin, i - begin);
            begin = i + 1;
            current_component_index++;
        }
        i++;
    }
    
    ret[current_component_index] = malloc((i - begin) * sizeof(char));
    strncpy(ret[current_component_index], s + begin, i - begin);
    begin = i + 1;
    current_component_index++;
    
    ret[current_component_index] = NULL;
    return ret;
}

void escape_string(String s) {
    for (int i = 0; i < strlen(s); i++) {
        if (s[i] == '\"' || s[i] == '\'') {
            // TODO: do something here I guess?
        }
    }
}

size_t smart_index(int idx, size_t max) {
    assert(abs(idx) <= max);
    return idx < 0 ? max + idx : idx;
}

void   write_byte    (Address addr, Byte b) {
	*((char*)addr) = b;
}
void   write_double  (Address addr, double value) {
	*((double*)addr) = value;
}
void   write_float   (Address addr, float value) {
	*((float*)addr) = value;
}
void   write_integer (Address addr, int value) {
	*((int*)addr) = value;
}

Byte   read_byte     (Address addr) {
	return *((char*)addr);
}
double read_double   (Address addr) {
	return *((double*)addr);
}
float  read_float    (Address addr) {
	return *((float*)addr);
}
int    read_integer  (Address addr) {
	return *((int*)addr);
}

void print_int(int i) {
	char *bin = malloc(8*sizeof(int));
	get_long_bits(bin, i, 8*sizeof(int));
	printf("%s %u\n", bin, i);
	free(bin);
}

Word read_word(void *start, size_t offset, size_t wordSize) {
	assert(wordSize > 0 && wordSize <= 8*sizeof(Word)); // make sure it can be stored in a WORD
	start += offset / (sizeof(Word)*8); // simplify the desired address
	offset = offset % (sizeof(Word)*8); // into lowest terms possible
	
	Word ret = *((Word*)start);
    
	ret = ret << offset;
	ret = ret >> (sizeof(Word)*8 - wordSize);
	return ret;
}

char itoc(unsigned int a) {
    // the characters 0-9 are guaranteed to be consecutive and naturally ordered
	return (char)(((int)'0'+a%10));
}

void  get_bits(char* buffer, Byte b) {
	for (int i = 0; i < 8; i++) { 
		buffer[i] = itoc(get_bit(&b, 7 - i));
	}
	buffer[8] = '\0';
}

Byte get_bit(void *addr, Byte offset) {
	assert(addr != NULL && offset < sizeof(long)*8);
	return (*((long*)addr) & (1 << offset)) >> offset;
}

void  get_long_bits(char* buffer, long value, unsigned int number_of_bits) {
	for (int i = 0; i < number_of_bits; i++) {
		buffer[i] = itoc(get_bit(&value, number_of_bits-1-i));
	}
	buffer[number_of_bits] = '\0';
}

void __fprint_memory_prefix(Stream f, Address a, size_t length) {
    fprintf(f, "[%p]-[%p]\n", a, a+length-1);
}

void __sprint_memory_prefix(String s, Address a, size_t length) {
    sprintf(s, "[%p]-[%p]\n", a, a+length-1);
}

void fprint_memory(Stream f, Address addr, size_t length, MemoryPrintFormat format) {
    Address last_byte = addr + length - 1;
	switch (format) {
		case PM_PLAIN_HEX:
        {
            for (Address this_byte = addr; this_byte < last_byte; this_byte++) {
                fprintf(f, "%02X ", read_byte(this_byte));
            }
            fprintf(f, "%02X", read_byte(last_byte));
        }
			break;
            
        case PM_PLAIN_BYTES:
        {
            for (Address this_byte = addr; this_byte < last_byte; this_byte++) {
                fprintf(f, "%02d ", read_byte(this_byte));
            }
            fprintf(f, "%02d", read_byte(last_byte));
        }
            break;
            
        case PM_PLAIN_BITSTRING:
        {
            String bits = new_String(9);
            for (Address this_byte = addr; this_byte < last_byte; this_byte++) {
                get_bits(bits, read_byte(this_byte));
                fprintf(f, "%s ", bits);
            }
            get_bits(bits, read_byte(last_byte));
            fprintf(f, "%s ", bits);
            free(bits);
        }
            break;
            
        case PM_DETAILED_HEX:
        {
            __fprint_memory_prefix(f, addr, length);
            for (Address this_byte = 0; this_byte <= last_byte; this_byte++) {
                fprintf(f, "\t%p:%02X\n", this_byte, read_byte(this_byte));
            }
        }
            break;
            
        case PM_DETAILED_BYTES:
        {
            __fprint_memory_prefix(f, addr, length);
            for (Address this_byte = 0; this_byte <= last_byte; this_byte++) {
                fprintf(f, "\t%p:%02d\n", this_byte, read_byte(this_byte));
            }
        }
            break;
            
        case PM_DETAILED_BITSTRING:
        {
            __fprint_memory_prefix(f, addr, length);
            String bits = new_String(9);
            for (Address this_byte = 0; this_byte <= last_byte; this_byte++) {
                get_bits(bits, read_byte(this_byte));
                fprintf(f, "\t%p:%s\n", this_byte, bits);
            }
            free(bits);
        }
            break;
        case PM_PLAIN_RAW:
        {
            fwrite(addr, 1, length, f);
        }
            break;
            
		default:
			fprintf(f, "Format %i is unimplemented.\n", format);
	}
    // force the buffer to be written
    fflush(f);
}

void sprint_memory(String s, Address addr, size_t length, MemoryPrintFormat format) {
    Address last_byte = addr + length - 1;
	switch (format) {
		case PM_PLAIN_HEX:
        {
            for (Address this_byte = addr; this_byte < last_byte; this_byte++) {
                sprintf(s, "%02X ", read_byte(this_byte));
            }
            sprintf(s, "%02X", read_byte(last_byte));
        }
			break;
            
        case PM_PLAIN_BYTES:
        {
            for (Address this_byte = addr; this_byte < last_byte; this_byte++) {
                sprintf(s, "%02d ", read_byte(this_byte));
            }
            sprintf(s, "%02d", read_byte(last_byte));
        }
            break;
            
        case PM_PLAIN_BITSTRING:
        {
            String bits = new_String(9);
            for (Address this_byte = addr; this_byte < last_byte; this_byte++) {
                get_bits(bits, read_byte(this_byte));
                sprintf(s, "%s ", bits);
            }
            get_bits(bits, read_byte(last_byte));
            sprintf(s, "%s ", bits);
            free(bits);
        }
            break;
            
        case PM_DETAILED_HEX:
        {
            __sprint_memory_prefix(s, addr, length);
            for (Address this_byte = 0; this_byte <= last_byte; this_byte++) {
                sprintf(s, "\t%p:%02X\n", this_byte, read_byte(this_byte));
            }
        }
            break;
            
        case PM_DETAILED_BYTES:
        {
            __sprint_memory_prefix(s, addr, length);
            for (Address this_byte = 0; this_byte <= last_byte; this_byte++) {
                sprintf(s, "\t%p:%02d\n", this_byte, read_byte(this_byte));
            }
        }
            break;
            
        case PM_DETAILED_BITSTRING:
        {
            __sprint_memory_prefix(s, addr, length);
            String bits = new_String(9);
            for (Address this_byte = 0; this_byte <= last_byte; this_byte++) {
                get_bits(bits, read_byte(this_byte));
                sprintf(s, "\t%p:%s\n", this_byte, bits);
            }
            free(bits);
        }
            break;
            
		default:
			sprintf(s, "Format %i is unimplemented.\n", format);
	}
}

Byte _rotl(Byte value, int shift) {
    if ((shift &= sizeof(value)*8 - 1) == 0)
		return value;
    return (value << shift) | (value >> (sizeof(value)*8 - shift));
}

Byte _rotr(Byte value, int shift) {
    if ((shift &= sizeof(value)*8 - 1) == 0)
		return value;
    return (value >> shift) | (value << (sizeof(value)*8 - shift));
}

Byte min_bits(unsigned long int v) {
	return ceil(log2(v+1));
}
