package caygurolmehmet;

/**
 * Created by Mehmet Gürol Çay on 25/10/2017.
 */
import javafx.scene.layout.Pane;
import javafx.scene.paint.Color;
import javafx.scene.shape.Rectangle;

import static caygurolmehmet.GameScreenController.PYTHON_BLOCK_SIZE;

public class Body extends Pane{

    private Rectangle rectangle;

    public Body(double x, double y, Color color) {
        this.setTranslateX(x);
        this.setTranslateY(y);
        this.rectangle = new Rectangle(PYTHON_BLOCK_SIZE, PYTHON_BLOCK_SIZE);
        this.rectangle.setFill(color);

        super.getChildren().addAll(rectangle);
    }
}
