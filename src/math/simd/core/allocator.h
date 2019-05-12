

#ifndef OFX_SIMDFLOATS_ALLOCATOR_H_INCLUDED
#define OFX_SIMDFLOATS_ALLOCATOR_H_INCLUDED

#include <stdlib.h>
#include "flags.h"
#include "../vectors/vect_ops.h"

// allocate array of memory-aligned floats to the given pointers, also set all the floats to zero
// set the pointer to nullptr if it can't get the the memory
void ofx_allocate_aligned(float* &toAllocate, int len);

// free the memory and set the pointer to nullptr
void ofx_deallocate_aligned(float* &toDelete );


#endif  // OFX_SIMDFLOATS_ALLOCATOR_H_INCLUDED
