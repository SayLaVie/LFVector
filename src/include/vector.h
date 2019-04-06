/*
   LFVector from Damian Dechev's 2006 paper

   author: Michael McCarver

   ToDo:
   - Implement better memory management than new/delete
*/

#ifndef VECTOR_H
#define VECTOR_H

#include <atomic>
#include <iostream>
#include <cstdlib>
#include <cmath>
#include "node.h"
#include "desc.h"

using namespace std;

template <class T>
class Vector
{
private:
   int numBucketsAllocated;
   atomic<Node<T>*> *vdata;
   atomic<Desc<T>*> desc;

   void allocBucket(int bucket)
   {
      int bucketSize = pow(2, hibitFBS + bucket);

      Node<T> *tmpMem = new Node<T>[bucketSize];
      Node<T> *nullVal = NULL;

      // CAS to insert new mem into vdata
      if (!vdata[bucket].compare_exchange_strong(nullVal, tmpMem))
      {
         delete [] tmpMem;
      } else
      {
         numBucketsAllocated++;
      }
   };

   // Reserve memory to accomodate a vector of size. Assume it won't receive negative values.
   void reserve(int size)
   {
      int i = HighestBit(this->vectorSize + FBS - 1) - hibitFBS;

      if (i < 0)
      {
         i = 0;
      }

      // Allocate memory for buckets until memory can accomodate size
      while (i < (HighestBit(size + FBS - 1) - hibitFBS));
      {
         i += 1;
         allocBucket(i);
      }
   };

   // Read the value of element i
   Node<T>* at(int i)
   {
      int pos = i + FBS;
      int hibit = HighestBit(pos);
      int idx = pos ^ (int)pow(2, hibit);

      // Return by reference
      return &vdata[hibit - hibitFBS][idx];
   };

   void completeWrite(WriteDesc<T> *writeOp)
   {
      // If there's a pending write operation, attempt to complete it
      if (writeOp && writeOp->pending)
      {
         at(writeOp->pos)->value.compare_exchange_strong(writeOp->v_old, writeOp->v_new);

         // The operation has been completed, either by the above CAS or by a helping thread
         writeOp->pending = false;
      }
   };


public:
   Vector()
   {
      vdata = new atomic<Node<T>*>[16]; // Initialize with 16 buckets (allows total memory of ~ FBS^16)
      vdata[0] = new Node<T>[FBS]; // Create first bucket of memory

      numBucketsAllocated = 1;

      desc = new Desc<T>();
   };


   ~Vector()
   {
      for (int i = 0; i < numBucketsAllocated; i++)
      {
         delete[] vdata[i];
      }

      delete[] vdata;
   };

   T popback()
   {
      Desc<T> *current;
      Desc<T> *next = NULL;
      T elem;

      do
      {
         current = this->desc;

         // If next != NULL, we are trying again and must release the memory from before
         if (next != NULL)
         {
            delete next;
         }

         completeWrite(current->writeOp);

         // If empty vector, return -1
         // Only really makes sense if T is int
         if (current->size == 0)
         {
            return -1;
         }

         elem = read(current->size - 1);

         next = new Desc<T>(current->size - 1, NULL);

      } while (!this->desc.compare_exchange_weak(current, next));

      return elem;
   };

   void pushback(T elem)
   {
      Desc<T> *current;
      Desc<T> *next = NULL;
      T *newElem = new T(elem);

      do
      {
         current = this->desc;

         // If next is not null, then we are attempting again and must free the memory allocated before
         if (next != NULL)
         {
            delete next;
         }

         completeWrite(current->writeOp);

         int bucket = HighestBit(current->size + FBS) - hibitFBS;

         if (vdata[bucket] == NULL)
         {
            allocBucket(bucket);
         }

         WriteDesc<T> *newWriteOp = new WriteDesc<T>(at(current->size)->value, newElem, current->size);
         next = new Desc<T>(current->size + 1, newWriteOp);

      } while (!this->desc.compare_exchange_weak(current, next));

      completeWrite(this->desc.load()->writeOp);
   };

   // Read element at i and return dereferenced Node<T>->value
   T read(int i)
   {
      return *(at(i)->value);
   };

   // Write to the element at position i
   void write(int i, T elem)
   {
      *(at(i)->value) = elem;
   };

   // Linearizes on the read of this->desc
   int size()
   {
      Desc<T> *current = this->desc;

      if (current->writeOp != NULL && current->writeOp->pending)
      {
         return current->size - 1;
      }

      return current->size;
   };

   // For printing out contents of vector from single thread (not thread-safe)
   void print()
   {
      for (int i = 0; i < this->desc.load()->size; i++)
      {
         std::cout << read(i) << endl;
      }
   };
};

#endif
