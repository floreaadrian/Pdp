package com.company;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class MatrixSum implements Runnable {
    private int matrixSize = 5000;
    private int[][] a = new int[matrixSize][matrixSize];
    private int[][] b = new int[matrixSize][matrixSize];
    private int[][] c = new int[matrixSize][matrixSize];
    private int numberOfLines, numberOfColumns, maxNumberOfElementsPerThread;

    MatrixSum(int numberOfLines, int numberOfColumns, int maxNumberOfElementsPerThread){
        this.numberOfColumns = numberOfColumns;
        this.numberOfLines = numberOfLines;
        this.maxNumberOfElementsPerThread = maxNumberOfElementsPerThread;
        initialiseMatrix(a, numberOfLines, numberOfColumns);
        initialiseMatrix(b, numberOfLines, numberOfColumns);
    }

    void sum(int a, int b, int c)
    {
        c = a + b;
    }

     private void initialiseMatrix(int matrix[][], int numberOfLines, int numberOfColumns)
    {
        for (int i=0; i<numberOfLines; i++)
        {
            for (int j=0; j<numberOfColumns; j++)
            {

                int max = 9;
                int min = 0;
                int rand = (int)(Math.random()*((max-min)+1))+min;
                matrix[i][j] = rand;
            }
        }
    }

    @Override
    public void run() {
        int threadId = (int)Thread.currentThread().getId();
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
}
