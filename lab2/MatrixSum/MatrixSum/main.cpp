#include <iostream>
#include <thread>
#include <cstdlib>
#include <chrono>

using namespace std;

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

double mainFunc(int numberOfLines, int numberOfColumns, int numberOfThreads)
{
    initialiseMatrix(a, numberOfLines, numberOfColumns);

    initialiseMatrix(b, numberOfLines, numberOfColumns);
    int maxNumberOfElementsPerThread;
    int numberOfElements = numberOfLines * numberOfColumns;
    thread threads[numberOfThreads + 1];

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
        threads[i] = thread(sumGranularity, a, b, c, numberOfLines, numberOfColumns, maxNumberOfElementsPerThread, i);
    }

    for (int i=0; i<numberOfThreads; i++)
    {
        threads[i].join();
    }
    auto finish4 = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed4 = finish4 - start4;
    
    return elapsed4.count();
}

void statistic(int numberOfLines, int numberOfColumns, int numberOfThreads){
    double nr = 0;
    for(int i = 0; i < 20; ++i)
        nr += mainFunc(numberOfLines, numberOfColumns, numberOfThreads);
    cout<<"------------------------------------------\n";
    cout<<numberOfLines<<"   |  "<<numberOfColumns<<"   |  "<<numberOfThreads<<"  |  "<<nr/20<<"\n";
}

int main()
{
    cout<<"Lines | Columns | Threads | Time\n";
//    statistic(3, 5, 2);
//    statistic(3, 5, 2);
    statistic(5000, 5000, 1);
    statistic(5000, 5000, 2);
    statistic(5000, 5000, 4);
//    statistic(5000, 5000, 8);
//    statistic(5000, 5000, 10);
//    statistic(5000, 5000, 25);
//    mainFunc(5000, 5000, 80);
    return 0;
}
