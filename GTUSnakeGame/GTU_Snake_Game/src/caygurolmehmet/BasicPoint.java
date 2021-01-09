package caygurolmehmet;

public class BasicPoint extends JobDecorator{
    public BasicPoint(Score score) {
        super(score);
    }

    @Override
    public double addScore() {
        return 1 * defaultScore + score.addScore();
    }
}
