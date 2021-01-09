package caygurolmehmet;

import javafx.scene.paint.Color;

/**
 * Created by Mehmet Gürol Çay on 25/10/2017.
 */

public class TrainingCer extends JobDecorator {

    public TrainingCer(Score score) {
        super(score);
    }

    @Override
    public double addScore() {
        return 3 * defaultScore + score.addScore();
    }
}
