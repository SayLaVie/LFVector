/*
*  Single-thread Operations test
*  A single thread will push 100 operations onto the vector.
*  Assert that the vector is the correct size.
*  Attempt to read each element of the vector, assert correct value.
*  Attempt to write each element of the vector, assert correct value.
*  Pop each element of the vector, assert correct value.
*/

#include <iostream>
#include <cassert>
#include "vector.h"

using namespace std;

int main()
{
   Vector<int> testVector;
   int numOps = 10000;

   // Pushback numOps elements into vector
   for (int i = 0; i < numOps; i++)
   {
      testVector.pushback(i);
   }

   // Assert the correct size
   assert(testVector.size() == numOps);

   // Assert that the correct values are being read from vector
   for (int i = 0; i < numOps; i++)
   {
      assert(testVector.read(i) == i);
   }

   // Reverse the elements of the vector with 'write' operation,
   // Assert the new values are correct.
   for (int i = 0; i < numOps; i++)
   {
      testVector.write(i, numOps - i - 1);
      assert(testVector.read(i) == (numOps - i - 1));
   }

   // Pop the vector until empty, and assert each value is correct
   for (int i = 0; i < numOps; i++)
   {
      assert(testVector.popback() == i);
   }

   cout << "Test successful.\n";
}
