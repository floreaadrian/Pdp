#include <iostream>
#include <thread>
#include <cstdlib>
#include <chrono>
#include <vector>
#include <future>
#include "ThreadPool.h"

using namespace std;

const int NORMAL_WAY = 1;
const int ASYNC_WAY = 2;
const int POOL_WAY = 3;

int numberOfLinesA, numberOfColumnsALinesB, numberOfColumnsB;
int a[1000][1000], b[1000][1000], c[1000][1000];
bool itsDone[1000];

void product(int a[][1000], int b[][1000], int c[][1000], int numberOfLinesAndColumns, int line, int column)
{
    c[line][column] = 0;
    for (int i=0; i<numberOfLinesAndColumns; i++)
    {
        c[line][column] += a[line][i] * b[i][column];
    }
}

void productGranularity(int a[][1000], int b[][1000], int c[][1000], int numberOfLinesA, int numberOfColumnsB, int numberOfColumnsALinesB, int maxNumberOfElementsPerThread, int threadId)
{
    int startsFrom = threadId * maxNumberOfElementsPerThread;
    int lineStart = startsFrom / numberOfColumnsB;
    int columnStart = startsFrom % numberOfColumnsB;
    int counter = 0;
    for (int i=lineStart; i<numberOfLinesA && counter < maxNumberOfElementsPerThread; i++)
    {
        for (int j=columnStart; j<numberOfColumnsB && counter < maxNumberOfElementsPerThread; j++)
        {
            product(a, b, c, numberOfColumnsALinesB, i, j);
            counter++;
        }
        columnStart = 0;
    }
    itsDone[threadId] = true;
}

void initialiseMatrix(int matrix[][1000], int numberOfLines, int numberOfColumns)
{
    for (int i=0; i<numberOfLines; i++)
    {
        for (int j=0; j<numberOfColumns; j++)
        {
            matrix[i][j] = rand() % 10;
        }
    }
}

double mainFunc(int numberOfLinesA, int numberOfColumnsALinesB, int numberOfColumnsB, int numberOfThreads, int theWay)
{
    initialiseMatrix(a, numberOfLinesA, numberOfColumnsALinesB);
    initialiseMatrix(b, numberOfColumnsALinesB, numberOfColumnsB);

    int numberOfElements = numberOfLinesA * numberOfColumnsB;
    int maxNumberOfElementsPerThread;
    thread threads[numberOfThreads + 1];
    vector<future<void>> futures;
    ThreadPool pool(numberOfThreads);
    for(int i = 0; i < numberOfThreads; ++i)
        itsDone[i] = false;
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
            futures.push_back(pool.enqueue(productGranularity, a, b, c, numberOfLinesA, numberOfColumnsB, numberOfColumnsALinesB, maxNumberOfElementsPerThread, i));
        else if(theWay == ASYNC_WAY)
            futures.push_back(async(productGranularity, a, b, c, numberOfLinesA, numberOfColumnsB, numberOfColumnsALinesB, maxNumberOfElementsPerThread, i));
        else if(theWay == NORMAL_WAY)
            threads[i] = thread(productGranularity, a, b, c, numberOfLinesA, numberOfColumnsB, numberOfColumnsALinesB, maxNumberOfElementsPerThread, i);
    }

    for (int i=0; i<numberOfThreads; i++)
    {
        if(theWay == POOL_WAY)
            futures[i].get();
        else if(theWay == NORMAL_WAY)
            threads[i].join();
        else if(theWay == ASYNC_WAY){
            while(true){
                bool toBreak = true;
                for(int i = 0; i < futures.size(); ++i)
                    if(itsDone[i] == false)
                        toBreak = false;
                if(toBreak)
                    break;
            }
        }
    }
    auto finish4 = chrono::high_resolution_clock::now();

    chrono::duration<double> elapsed4 = finish4 - start4;
    
    return elapsed4.count();
}

void statistics(int numberOfLinesA, int numberOfColumnsALinesB, int numberOfColumnsB, int numberOfThreads){
    double nrNormal = 0;
    double nrAsync = 0;
    double nrPool = 0;
    int iterationNumber = 20;
    for(int i = 0; i < iterationNumber; ++i){
        nrNormal += mainFunc(numberOfLinesA, numberOfColumnsALinesB, numberOfColumnsB, numberOfThreads, NORMAL_WAY);
        nrAsync += mainFunc(numberOfLinesA, numberOfColumnsALinesB, numberOfColumnsB, numberOfThreads, ASYNC_WAY);
        nrPool += mainFunc(numberOfLinesA, numberOfColumnsALinesB, numberOfColumnsB, numberOfThreads, POOL_WAY);
    }
    cout<<"------------------------------------------\n";
    cout<<"Async   | "<<numberOfThreads<<"  |  "<<nrAsync/iterationNumber<<"\n";
    cout<<"Pool    | "<<numberOfThreads<<"  |  "<<nrPool/iterationNumber<<"\n";
    cout<<"Normal  | "<<numberOfThreads<<"  |  "<<nrNormal/iterationNumber<<"\n";
}


int main()
{
    cout<<"Way     | Threads | Time\n";
    statistics(700, 700, 700, 1);
    statistics(700, 700, 700, 2);
    statistics(700, 700, 700, 4);
    return 0;
}
