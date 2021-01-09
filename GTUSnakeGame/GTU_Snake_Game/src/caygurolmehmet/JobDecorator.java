package caygurolmehmet;

/**
 * Created by Mehmet Gürol Çay on 24/10/2017.
 */
public abstract class JobDecorator extends Score{

    protected Score score;


    public JobDecorator(Score score) {
        this.score = score;
    }
}
