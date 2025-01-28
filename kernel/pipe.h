// my pipe.h
#ifndef PIPE_H
#define PIPE_H

#include "types.h"
#include "spinlock.h"

#define PIPESIZE 512

// Function declarations
int pipealloc(struct file **f0, struct file **f1);  
void pipeclose(struct pipe *pi, int writable);      
int pipewrite(struct pipe *pi, uint64 addr, int n); // Write to a pipe
int piperead(struct pipe *pi, uint64 addr, int n);  // Read from a pipe

#endif // PIPE_H