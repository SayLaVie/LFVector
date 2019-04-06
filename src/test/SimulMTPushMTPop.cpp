/*
*  Simultaneously push and pop onto the shared vector.
*  Push threads will push numberOfOperations times,
*  Pop threads will pop until they see an empty vector.
*  Trying to trigger segfaults.
*/

#include <iostream>
#include "vector.h"
#include <thread>
#include <atomic>

using namespace std;

Vector<int> sharedVector;
const int numberOfOperations = 10000;

void runPushTest()
{
   for (int i = 0; i < numberOfOperations; i++)
   {
      sharedVector.pushback(i);
   }
}

void runPopTest()
{
   while (sharedVector.size() > 0)
   {
      sharedVector.popback();
   }
}

int main()
{
   // Actual number of threads used here will be numberOfThreads * 2
   int numberOfThreads = 16;
   thread pushThreadList[numberOfThreads];
   thread popThreadList[numberOfThreads];

   // Create numberOfThreads workers for the push test and also the pop test
   for (int i = 0; i < numberOfThreads; i++)
   {
      pushThreadList[i] = thread(runPushTest);
      popThreadList[i] = thread(runPopTest);
   }

   for (int i = 0; i < numberOfThreads; i++)
   {
      pushThreadList[i].join();
      popThreadList[i].join();
   }

   cout << "Test successful.\n";
}
