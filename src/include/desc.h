#ifndef DESC_H
#define DESC_H

#include "definitions.h"
#include "writeDesc.h"

template<class T>
struct Desc
{
   WriteDesc<T> *writeOp = NULL;
   int size;

   Desc(int _size = 0, WriteDesc<T>* _writeOp = NULL)
      : size(_size)
      , writeOp(_writeOp)
      {};
};

#endif
