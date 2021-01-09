package caygurolmehmet;

/**
 * Created by Mehmet Gürol Çay on 28/10/2017.
 */

public class ForeignLan extends JobDecorator {


    public ForeignLan(Score score) {
        super(score);
    }


    @Override
    public double addScore() {
        return 2 * defaultScore + score.addScore();
    }
}
