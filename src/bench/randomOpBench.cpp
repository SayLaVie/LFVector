#include <iostream>
#include "vector.h"
#include <thread>
#include <chrono>
#include <random>
#include <functional>
#include <atomic>

using namespace std;

atomic_int numberOfOperations(100000);

default_random_engine generator;
discrete_distribution<int> distribution{9, 1, 90}; // Set the probabilities here -- 1, 1, 1 is even odds. Order is push, pop, size
auto generateOperation = std::bind (distribution, generator);

Vector<int> sharedVector;

void initVector()
{
   for (int i = 0; i < numberOfOperations / 2; i++)
   {
      sharedVector.pushback(i);
   }
}

void runBench(int threadID)
{
   while (numberOfOperations > 0)
   {
      switch(generateOperation())
      {
         case 0:
            sharedVector.pushback(1);
            break;
         case 1:
            sharedVector.popback();
            break;
         case 2:
            sharedVector.read(0);
            break;
      }

      numberOfOperations--;
   }
}

int main()
{
   std::chrono::high_resolution_clock::time_point begin, end;
   std::chrono::duration<double> executionTime;

   begin = std::chrono::high_resolution_clock::now();

   initVector();

   end = std::chrono::high_resolution_clock::now();
   executionTime = std::chrono::duration_cast<std::chrono::duration<double>>(end - begin);

   cout << "Time to initialize: " << executionTime.count() << endl;

   int numThreads = 16;

   thread threadList[numThreads];

   begin = std::chrono::high_resolution_clock::now();

   for (int i = 0; i < numThreads; i++)
   {
      threadList[i] = thread(runBench, i);
   }

   for (int i = 0; i < numThreads; i++)
   {
      threadList[i].join();
   }

   end = std::chrono::high_resolution_clock::now();
   executionTime = std::chrono::duration_cast<std::chrono::duration<double>>(end - begin);

   cout << "Threads: " << numThreads << "\ttime: " << executionTime.count() << endl;

   cout << "Vector size: " << sharedVector.size() << endl;
}
