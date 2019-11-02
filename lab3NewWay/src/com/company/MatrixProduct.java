package com.company;

public class MatrixProduct implements Runnable{
    private int matrixSize = 5000;
    private int[][] a = new int[matrixSize][matrixSize];
    private int[][] b = new int[matrixSize][matrixSize];
    private int[][] c = new int[matrixSize][matrixSize];
    private int numberOfLinesA, numberOfColumnsALinesB, numberOfColumnsB, maxNumberOfElementsPerThread;

    MatrixProduct(int numberOfLinesA, int numberOfColumnsALinesB, int numberOfColumnsB, int maxNumberOfElementsPerThread){
        this.numberOfLinesA = numberOfLinesA;
        this.numberOfColumnsALinesB = numberOfColumnsALinesB;
        this.numberOfColumnsB = numberOfColumnsB;
        this.maxNumberOfElementsPerThread = maxNumberOfElementsPerThread;
        initialiseMatrix(a, numberOfLinesA, numberOfColumnsALinesB);
        initialiseMatrix(b, numberOfColumnsALinesB, numberOfColumnsB);
    }
    private void product(int line, int column)
    {
        c[line][column] = 0;
        for (int i=0; i<numberOfColumnsALinesB; i++)
        {
            c[line][column] += a[line][i] * b[i][column];
        }
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
        int startsFrom = threadId * maxNumberOfElementsPerThread;
        int lineStart = startsFrom / numberOfColumnsB;
        int columnStart = startsFrom % numberOfColumnsB;
        int counter = 0;
        for (int i=lineStart; i<numberOfLinesA && counter < maxNumberOfElementsPerThread; i++)
        {
            for (int j=columnStart; j<numberOfColumnsB && counter < maxNumberOfElementsPerThread; j++)
            {
                product(i, j);
                counter++;
            }
            columnStart = 0;
        }
    }
}
