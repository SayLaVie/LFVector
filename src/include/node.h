#ifndef NODE_H
#define NODE_H

#include <cstdlib>
#include <cstdint>
#include <atomic>

template <class T>
class Node
{
public:
   std::atomic<T*> value;

   Node(): value(NULL){};

   Node(T elem)
   {
      value = new T(elem);
   };

   ~Node()
   {
      if (value)
      {
         delete value;
      }
   };
};

#endif
