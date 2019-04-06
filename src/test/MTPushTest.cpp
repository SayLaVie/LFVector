/*
*  Multi-threaded push test
*  Multiple threads each push numberOfPushes values onto the shared vector.
*  Verify that each thread was successful with every push by moving each
*  element into an stl vector, sorting, and asserting each value in the vector.
*/

#include <iostream>
#include <algorithm>
#include <vector>
#include <thread>
#include <atomic>
#include <cassert>
#include "vector.h"

using namespace std;

Vector<int> sharedVector;
const int numberOfPushes = 100;

void runPushTest()
{
   for (int i = 0; i < numberOfPushes; i++)
   {
      sharedVector.pushback(i);
   }
}

int main()
{
   int numberOfThreads = 16;
   thread pushThreadList[numberOfThreads];

   for (int i = 0; i < numberOfThreads; i++)
   {
      pushThreadList[i] = thread(runPushTest);
   }

   for (int i = 0; i < numberOfThreads; i++)
   {
      pushThreadList[i].join();
   }

   // Assert the correct vector size
   assert(sharedVector.size() == (numberOfPushes * numberOfThreads));

   // We transfer our vector into an STL vector so that we can use sort.
   // We sort the vector and verify that every element is accounted for.
   vector<int> stlVec;

   for (int i = 0; i < sharedVector.size(); i++)
   {
      stlVec.push_back(sharedVector.read(i));
   }

   sort(stlVec.begin(), stlVec.end());

   // Test that there are numberOfThreads copies of each value in the vector
   for (int i = 0; i < stlVec.size() - numberOfThreads; i += numberOfThreads)
   {
      for (int j = i + 1; j < i + numberOfThreads; j++)
      {
         assert(stlVec[j] == stlVec[i]);
      }
   }

   cout << "Test successful.\n";
}
