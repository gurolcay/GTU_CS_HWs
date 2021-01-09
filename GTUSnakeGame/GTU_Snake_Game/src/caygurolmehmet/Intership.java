package caygurolmehmet;


/**
 * Created by Mehmet Gürol Çay on 25/10/2017.
 */

public class Intership extends JobDecorator {

    public Intership(Score score) {
        super(score);
    }

    @Override
    public double addScore() {
        return 1.5 * defaultScore + score.addScore();
    }

}
