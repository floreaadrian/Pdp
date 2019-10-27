import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class Matrix {
    private int[][] matrix;
    private int rows, columns;

    public Matrix(int rows, int columns) {
        this.rows = rows;
        this.columns = columns;
        matrix = new int[rows][columns];
    }

    public Matrix(int rows, int columns, int[][] matrix) {
        this.rows = rows;
        this.columns = columns;
        this.matrix = matrix;
    }

    public int getNoRows() {
        return rows;
    }

    public int getNoColumns() {
        return columns;
    }

    public int[][] getMatrix() {
        return matrix;
    }

    public void setMatrix(int i, int j, int sum) {
        matrix[i][j] = sum;
    }

    public void printMatrix() {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < columns; ++j) {
                System.out.print(matrix[i][j] + " ");
            }
            System.out.println();
        }
    }

    public static Matrix add(Matrix matrix1, Matrix matrix2, int noThreads) throws InterruptedException {
        int operationsThread = (matrix1.getNoRows() * matrix1.getNoColumns()) / noThreads;
        int rest = (matrix1.getNoRows() * matrix1.getNoColumns()) % noThreads;
        //Thread[] threads = new Thread[noThreads + 2];
        Matrix resultMatrix = new Matrix(matrix1.rows, matrix1.columns);

        ExecutorService service = Executors.newFixedThreadPool(noThreads);

        long start = System.currentTimeMillis();

        int iStart = 0, jStart = 0, iStop = 0, jStop = 0;
        for (int i = 1; i <= noThreads; ++i) {
            int operationsFinal;
            if (rest > 0) {
                operationsFinal = operationsThread + 1;
                rest--;
            } else {
                operationsFinal = operationsThread;
            }

            while (operationsFinal != 0) {
                if (jStop == matrix1.getNoColumns()) {
                    jStop = 0;
                    ++iStop;
                }
                jStop++;
                operationsFinal--;
            }

            final int  ii = iStart, iii = iStop, jj = jStart, jjj = jStop;

            service.execute(new MSumThread(matrix1, matrix2, resultMatrix, iStart, jStart, iStop, jStop) {});
            service.execute(new MSumThread(matrix1, matrix2, resultMatrix, ii, jj, iii, jjj) {});

            iStart = iStop;
            jStart = jStop + 1;
            if (jStart == matrix1.columns) {
                jStart = 0;
                ++ iStart;
            }
        }

        long stop = System.currentTimeMillis();
        System.out.println("TIME: " + (stop - start));
        //System.out.println(start);
        //System.out.println(stop);

        //printMatrix(resultMatrix);
        return resultMatrix;
    }

    private static void printMatrix(Matrix resultMatrix) {
        System.out.println("The resulting matrix is: ");
        for (int i = 0; i < resultMatrix.getNoRows(); ++i) {
            for (int j = 0; j < resultMatrix.getNoColumns(); ++j) {
                System.out.print(resultMatrix.getMatrix()[i][j] + " ");
            }
            System.out.println();
        }
    }

    public static Matrix multiply(Matrix matrix1, Matrix matrix2, int noThreads) throws InterruptedException {
        int operationsThread = (matrix2.rows * matrix2.columns) / noThreads;
        int rest = (matrix2.rows * matrix2.columns) % noThreads;
        Matrix resultMatrix = new Matrix(matrix1.rows, matrix2.columns);

        ExecutorService service = Executors.newFixedThreadPool(noThreads);

        long start = System.currentTimeMillis();

        int iStart = 0, jStart = 0, iStop = 0, jStop = 0;
        for (int i = 1; i <= noThreads; ++i) {
            int operationsFinal;
            if (rest > 0) {
                operationsFinal = operationsThread + 1;
                rest--;
            } else {
                operationsFinal = operationsThread;
            }

            while (operationsFinal != 0) {
                if (jStop == matrix1.columns) {
                    jStop = 0;
                    ++iStop;
                }
                jStop++;
                operationsFinal--;
            }

            final int  ii = iStart, iii = iStop, jj = jStart, jjj = jStop;

            service.execute(new MProdThread(matrix1, matrix2, resultMatrix, iStart, jStart, iStop, jStop) {});
            service.execute(new MProdThread(matrix1, matrix2, resultMatrix, ii, jj, iii, jjj) {});

            iStart = iStop;
            jStart = jStop + 1;
            if(jStart == matrix2.columns) {
                jStart = 0;
                ++iStart;
            }
        }
        long stop = System.currentTimeMillis();
        System.out.println("TIME: " + (stop - start));
        //System.out.println(start);
        //System.out.println(stop);
        return resultMatrix;
    }
}