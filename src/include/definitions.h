/*
* This file holds constants, macros, enums, etc.
*/

#ifndef DEFINITIONS_H
#define DEFINITIONS_H

// Size of the first bucket of memory
const int FBS = 8;

// HighestBit function implemented with assembly
inline int HighestBit(int n)
{
   int msb;
   asm("bsrl %1,%0" : "=r"(msb) : "r"(n)); // works on x86
   return msb;
}

const int hibitFBS = HighestBit(FBS);

#endif
