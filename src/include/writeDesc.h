#ifndef WRITEDESC_H
#define WRITEDESC_H

#include <cstdlib>
#include "node.h"

template<class T>
struct WriteDesc
{
   bool pending = false;
   int pos;

   T* v_old;
   T* v_new;

   WriteDesc(T* _v_old, T* _v_new, int _pos)
      : v_old(_v_old)
      , v_new(_v_new)
      , pos(_pos)
      , pending(true)
      {};
   };

#endif
