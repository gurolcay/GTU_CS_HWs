package caygurolmehmet;

public class Point extends Score {

    private double point;

    public Point() {
        point = 10;
    }

    @Override
    protected double addScore() {
        return point;
    }
}
