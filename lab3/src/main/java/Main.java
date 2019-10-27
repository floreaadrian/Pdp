import java.io.*;

public class Main extends FileUtils {
    public static void main(String[] args) throws InterruptedException, IOException {
        int n, m;
        int noThreads = 5;
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        System.out.println("n = ");
        n = Integer.parseInt(br.readLine());
        System.out.println("m = ");
        m = Integer.parseInt(br.readLine());

        putInFile(n, m, "matrixA.txt");
        putInFile(n, m, "matrixB.txt");

        int[][] a = new int[n][m];
        int[][] b = new int[n][m];

        readArrayFromFile(n, m, a, "matrixA.txt");
        readArrayFromFile(n, m, b, "matrixB.txt");

        Matrix matrix1 = new Matrix(n, m, a);
        Matrix matrix2 = new Matrix(n, m, b);

        Matrix resultMatrix = Matrix.multiply(matrix1, matrix2, noThreads);
        //Matrix resultMatrix = Matrix.add(matrix1, matrix2, noThreads);

        resultMatrix.printMatrix();
    }
}
