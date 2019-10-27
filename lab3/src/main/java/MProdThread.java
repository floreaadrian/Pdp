
public class MProdThread implements Runnable {
    protected Matrix matrix1, matrix2, matrix3;
    protected int iStart, jStart, iStop, jStop;

    public MProdThread(Matrix matrix1, Matrix matrix2, Matrix matrix3, int iStart, int jStart, int iStop, int jStop) {
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
                if (i == iStart) {
                    ceva(i, j, j >= jStart);
                } else if (i == iStop) {
                    ceva(i, j, j <= jStop);
                } else {
                    int res = 0;
                    for (int index = 0; index < matrix1.getNoColumns(); ++index) {
                        res += (matrix1.getMatrix()[i][index] * matrix2.getMatrix()[index][j]);
                    }
                    matrix3.getMatrix()[i][j] = res;
                }
            }
        }
    }

    private void ceva(int i, int j, boolean b) {
        if (b) {
            int res = 0;
            for (int index = 0; index < matrix1.getNoColumns(); ++index) {
                res += (matrix1.getMatrix()[i][index] * matrix2.getMatrix()[index][j]);
            }
            matrix3.getMatrix()[i][j] = res;
        }
    }
}