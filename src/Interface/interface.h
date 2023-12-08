#ifndef INTERFACE
#define INTERFACE

typedef struct ThreadParameters
{
  int auto_rotate;
  char *filename;
  int step_index;
  double angle;
  char* filename_resolved;
  void (*func_ptr)(void);
} ThreadParameters;

void* BAME(void* data);

#endif