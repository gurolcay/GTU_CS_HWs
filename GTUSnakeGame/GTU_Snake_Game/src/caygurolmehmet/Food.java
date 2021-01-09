package caygurolmehmet;

/**
 * Created by Mehmet Gürol Çay on 25/10/2017.
 */

import javafx.scene.layout.Pane;
import javafx.scene.paint.Color;
import javafx.scene.shape.Rectangle;

import static caygurolmehmet.GameScreenController.FOOD_BLOCK_SIZE;

public class Food extends Pane{
    private Rectangle food;

    public Food(double x, double y, Color color) {
        System.out.println("food is created");
        this.setTranslateX(x);
        this.setTranslateY(y);
        food = new Rectangle(FOOD_BLOCK_SIZE, FOOD_BLOCK_SIZE);
        food.setFill(color);

        super.getChildren().addAll(food);
    }

    public void setFoodColor(Color color) {
        food.setFill(color);
    }

}
