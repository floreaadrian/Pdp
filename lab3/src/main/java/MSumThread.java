
public class MSumThread implements Runnable {
    protected Matrix matrix1, matrix2, matrix3;
    protected int iStart, jStart, iStop, jStop;

    public MSumThread (Matrix matrix1, Matrix matrix2, Matrix matrix3, int iStart, int jStart, int iStop, int jStop) {
        this.matrix1 = matrix1;
        this.matrix2 = matrix2;
        this.matrix3 = matrix3;
        this.iStart = iStart;
        this.iStop = iStop;
        this.jStart = jStart;
        this.jStop = jStop;
    }

    @Override
    public void run() {
        for (int i = iStart; i <= iStop; ++i) {
            for (int j = 0; j < matrix2.getNoColumns(); ++j) {
                try {
                    if (i == iStart) {
                        if (j >= jStart) {
                            matrix3.setMatrix(i, j, matrix1.getMatrix()[i][j] + matrix2.getMatrix()[i][j]);
                        }
                    } else if (i == iStop) {
                        if (j <= jStop) {
                            matrix3.setMatrix(i, j, matrix1.getMatrix()[i][j] + matrix2.getMatrix()[i][j]);
                        }
                    } else {
                        matrix3.setMatrix(i, j, matrix1.getMatrix()[i][j] + matrix2.getMatrix()[i][j]);
                    }
                } catch (Exception e) {
                    System.out.print("");
                }
            }
        }
    }
}