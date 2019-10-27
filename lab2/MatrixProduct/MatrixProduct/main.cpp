#include <iostream>
#include <thread>
#include <cstdlib>
#include <chrono>

using namespace std;

int numberOfLinesA, numberOfColumnsALinesB, numberOfColumnsB;
int a[1000][1000], b[1000][1000], c[1000][1000];

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

double mainFunc(int numberOfLinesA, int numberOfColumnsALinesB, int numberOfColumnsB, int numberOfThreads)
{
    initialiseMatrix(a, numberOfLinesA, numberOfColumnsALinesB);
    initialiseMatrix(b, numberOfColumnsALinesB, numberOfColumnsB);

    int numberOfElements = numberOfLinesA * numberOfColumnsB;
    int maxNumberOfElementsPerThread;
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
        threads[i] = thread(productGranularity, a, b, c, numberOfLinesA, numberOfColumnsB, numberOfColumnsALinesB, maxNumberOfElementsPerThread, i);
    }

    for (int i=0; i<numberOfThreads; i++)
    {
        threads[i].join();
    }
    auto finish4 = chrono::high_resolution_clock::now();

    chrono::duration<double> elapsed4 = finish4 - start4;
    
    return elapsed4.count();
}

void statistics(int numberOfLinesA, int numberOfColumnsALinesB, int numberOfColumnsB, int numberOfThreads){
    double nr = 0;
    for(int i = 0; i < 20; ++i)
        nr += mainFunc(numberOfLinesA, numberOfColumnsALinesB, numberOfColumnsB, numberOfThreads);
    cout<<"------------------------------------------\n";
    cout<<numberOfThreads<<"   |    "<<nr/20<<"\n";
}


int main()
{
    cout<<"Threads | Time\n";
    statistics(700, 700, 700, 1);
    statistics(700, 700, 700, 2);
    statistics(700, 700, 700, 4);
//    statistics(700, 700, 700, 25);
//    statistics(200, 200, 200, 114);
    return 0;
}
