#include <iostream>
#include <thread>
#include <cstdlib>
#include <chrono>
#include <future>
#include <vector>
#include "ThreadPool.h"

using namespace std;

const int NORMAL_WAY = 1;
const int ASYNC_WAY = 2;
const int POOL_WAY = 3;

int numberOfLines, numberOfColumns;
int a[5000][5000], b[5000][5000], c[5000][5000];


void sum(int a, int b, int &c)
{
    c = a + b;
}

void sumGranularity(int a[][5000], int b[][5000], int c[][5000], int numberOfLines, int numberOfColumns, int maxNumberOfElementsPerThread, int threadId)
{
    int counter = 0;
    int startsFrom = threadId * maxNumberOfElementsPerThread;
    int lineStart = startsFrom / numberOfColumns;
    int columnStart = startsFrom % numberOfColumns;
    for (int i=lineStart; i<numberOfLines && counter < maxNumberOfElementsPerThread; i++)
    {
        for (int j=columnStart; j<numberOfColumns && counter < maxNumberOfElementsPerThread; j++)
        {
            c[i][j] = a[i][j] + b[i][j];
            counter++;
        }
        columnStart = 0;
    }
}

void initialiseMatrix(int matrix[][5000], int numberOfLines, int numberOfColumns)
{
    for (int i=0; i<numberOfLines; i++)
    {
        for (int j=0; j<numberOfColumns; j++)
        {
            matrix[i][j] = rand() % 10;
        }
    }
}

double mainFunc(int numberOfLines, int numberOfColumns, int numberOfThreads, int theWay)
{
    initialiseMatrix(a, numberOfLines, numberOfColumns);

    initialiseMatrix(b, numberOfLines, numberOfColumns);
    int maxNumberOfElementsPerThread;
    int numberOfElements = numberOfLines * numberOfColumns;
    thread threads[numberOfThreads + 1];
    
    vector<future<void>> futures;
    ThreadPool pool(numberOfThreads);
    
    if (numberOfElements % numberOfThreads == 0)
        {
            maxNumberOfElementsPerThread = numberOfElements / numberOfThreads;
        }
        else
        {
            maxNumberOfElementsPerThread = numberOfElements / numberOfThreads + 1;
        }

    auto start4 = chrono::high_resolution_clock::now();
    for (int i=0; i<numberOfThreads; i++)
    {
        if(theWay == POOL_WAY)
            futures.push_back(pool.enqueue(sumGranularity, a, b, c, numberOfLines, numberOfColumns, maxNumberOfElementsPerThread, i));
        else if(theWay == ASYNC_WAY)
            futures.push_back(async(sumGranularity, a, b, c, numberOfLines, numberOfColumns, maxNumberOfElementsPerThread, i));
        else if(theWay == NORMAL_WAY)
            threads[i] = thread(sumGranularity, a, b, c, numberOfLines, numberOfColumns, maxNumberOfElementsPerThread, i);
            
    }

    for (int i=0; i<numberOfThreads; i++)
    {
        if(theWay == POOL_WAY || theWay == ASYNC_WAY)
            futures[i].get();
        else if(theWay == NORMAL_WAY)
            threads[i].join();
    }
    auto finish4 = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed4 = finish4 - start4;
    
    return elapsed4.count();
}

void statistic(int numberOfLines, int numberOfColumns, int numberOfThreads){
    double nrNormal = 0;
    double nrAsync = 0;
    double nrPool = 0;
    int iterationNumber = 20;
    for(int i = 0; i < iterationNumber; ++i){
        nrNormal += mainFunc(numberOfLines, numberOfColumns, numberOfThreads, NORMAL_WAY);
        nrAsync += mainFunc(numberOfLines, numberOfColumns, numberOfThreads, ASYNC_WAY);
        nrPool += mainFunc(numberOfLines, numberOfColumns, numberOfThreads, POOL_WAY);
    }
    cout<<"------------------------------------------\n";
    cout<<"Async   | "<<numberOfThreads<<"  |  "<<nrAsync/iterationNumber<<"\n";
    cout<<"Pool    | "<<numberOfThreads<<"  |  "<<nrPool/iterationNumber<<"\n";
    cout<<"Normal  | "<<numberOfThreads<<"  |  "<<nrNormal/iterationNumber<<"\n";
}

int main()
{
    cout<<"Way     | Threads | Time\n";
    statistic(5000, 5000, 1);
    statistic(5000, 5000, 2);
    statistic(5000, 5000, 4);
    return 0;
}
