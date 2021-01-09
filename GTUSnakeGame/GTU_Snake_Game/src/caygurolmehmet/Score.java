package caygurolmehmet;

/**
 * Created by Mehmet Gürol Çay on 28/10/2017.
 */

public abstract class Score {

    private double score = 0.0;
    protected double defaultScore = 10;

    public double getScore() {
        return score;
    }

    protected abstract double addScore();
}
