#ifndef _FPSOUT_CLASS
#define _FPSOUT_CLASS

#include <stdio.h>

class FPSout
{
 public:

  FPSout(char *filename);
  ~FPSout();

  int isOpen();

  void printset(int length, int *iset);
  void printSet(int length, int *iset, int support);
  void close();

 private:

  FILE *out;
};

#endif

