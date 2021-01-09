/**
 * Created by Mehmet Gürol Çay on 24/10/2017.
 */
package caygurolmehmet;

import javafx.animation.KeyFrame;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.control.Label;
import javafx.scene.control.MenuItem;
import javafx.scene.layout.Pane;
import javafx.scene.paint.Color;
import javafx.animation.Timeline;
import javafx.collections.ObservableList;
import javafx.scene.Group;
import javafx.scene.Node;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;
import javafx.util.Duration;

import java.util.ArrayList;
import java.util.ListIterator;

import static java.lang.Math.abs;

public class GameScreenController extends Parent {

    private static final Timeline timeline = new Timeline();
    public static int PYTHON_BLOCK_SIZE = 20; // part of snake
    public static int FOOD_BLOCK_SIZE = 20; // part of food
    public static final int STAMINA_BASE_RAISE_VAL = 20;
    public enum Direction {
        UP, DOWN, LEFT, RIGHT
    }

    public enum PythonType {
        PYTHON, ANACONDA
    }

    private Direction direction = Direction.RIGHT;
    private PythonType pythonType = PythonType.PYTHON;


    private ArrayList<Double> allScores = new ArrayList<Double>();
    private ObservableList<Node> snake;
    private boolean running = false;  // snake
    private boolean moved = false; // application
    private boolean isEndless = true;
    private Food food;
    private Score score;

    private double currentScore = 0;
    private int stamina = 100;

    private Label currentScoreVal;
    private Label timeVal;
    private double getRootX;
    private double getRootY;

    // TODO: should get automatically
    private double gameAreaWidth = 600;
    private double gameAreaHeight = 340;

    private double snakeSpeed = 0.15;

    @FXML
    void pauseGame(ActionEvent event) {
        if (this.running == true) {
            this.running = false;
            timeline.pause();
        } else {
            this.running = true;
            timeline.play();
        }
    }

    @FXML
    void saveGame(ActionEvent event) {

    }

    @FXML
    private MenuItem closeButton;

    @FXML
    void closeGame(ActionEvent event){
        System.exit(0);
    }

    public GameScreenController(PythonType type) {
        pythonType = type;
    }

    public GameScreenController() {
    }

    public void startGame() {
        direction = Direction.RIGHT;
        stamina = 100;
        currentScoreVal.setText("" + currentScore);

        if (pythonType == PythonType.ANACONDA)
            PYTHON_BLOCK_SIZE = PYTHON_BLOCK_SIZE * 2;

        Body head = new Body(100, 100, Color.BLACK);

        snake.add(head);
        timeline.play();
        this.running = true;
    }

    public Parent createContent() throws Exception {
        Parent gameRoot = FXMLLoader.load(getClass().getResource("gameScreen.fxml"));
        Pane gamePane = (Pane) gameRoot.lookup("#gameArea");
        // TODO : if previous scores exist load here

        getRootX = gamePane.getTranslateX();
        getRootY = gamePane.getTranslateY();

        Group snakeBody = new Group();
        snake = snakeBody.getChildren();

        currentScoreVal = (Label) gameRoot.lookup("#scoreArea");
        timeVal = (Label) gameRoot.lookup("#timeArea");

        food = new Food(getRootX, getRootY, Color.BLUE);
        score = new Point();
        score = new BasicPoint(score);
        foodRand(food);

        KeyFrame frame = new KeyFrame(Duration.seconds(snakeSpeed), event -> {
            if (!this.running) {
                return;
            }

            boolean toRemove = snake.size() > 1;
            Node tail = toRemove ? snake.remove(snake.size() - 1) : snake.get(0);

            double tailX = tail.getTranslateX();
            double tailY = tail.getTranslateY();

            switch (direction) {
                case RIGHT:
                    tail.setTranslateX(snake.get(0).getTranslateX() + FOOD_BLOCK_SIZE);
                    tail.setTranslateY(snake.get(0).getTranslateY());
                    break;
                case UP:
                    tail.setTranslateX(snake.get(0).getTranslateX());
                    tail.setTranslateY(snake.get(0).getTranslateY() - FOOD_BLOCK_SIZE);
                    break;
                case DOWN:
                    tail.setTranslateX(snake.get(0).getTranslateX());
                    tail.setTranslateY(snake.get(0).getTranslateY() + FOOD_BLOCK_SIZE);
                    break;
                case LEFT:
                    tail.setTranslateX(snake.get(0).getTranslateX() - FOOD_BLOCK_SIZE);
                    tail.setTranslateY(snake.get(0).getTranslateY());
                    break;
            }

            moved = true;

            if (toRemove)
                snake.add(0, tail);

            for (Node rect : snake) {
                // checking collision for own snake body
                if (rect != tail && tail.getTranslateX() == rect.getTranslateX() && tail.getTranslateY() == rect.getTranslateY()) {
                    allScores.add(currentScore);
                    currentScore = 0;
                    currentScoreVal.setText("" + currentScore);
                    restartGame();
                    break;
                }
            }

            if (isEndless) {
                fieldIsEndless((Body) tail); // pass the wall
            } else {
                fieldNOTEndless((Body) tail, currentScoreVal, food); // stop the wall
            }

            // checking collision for food
            checkFoodCollision(tail, tailX, tailY);

            --stamina;
            timeVal.setText("" + stamina);
            if (stamina == 0) {
                stopGame();
                startGame();
            }

        });

        timeline.getKeyFrames().addAll(frame);
        timeline.setCycleCount(Timeline.INDEFINITE);

        gamePane.getChildren().addAll(food, snakeBody);

        return gameRoot;
    }

    private void checkFoodCollision(Node tail, double tailX, double tailY) {
        if (pythonType == PythonType.PYTHON) {
            if (tail.getTranslateX() == food.getTranslateX() && tail.getTranslateY() == food.getTranslateY()) {
                eatFood(tailX, tailY);
            }
        } else {
            System.out.println("head x: " + tail.getTranslateX());
            System.out.println("head y: " + tail.getTranslateY());
            System.out.println("food x: " + food.getTranslateX());
            System.out.println("food y: " + food.getTranslateY());

            // TODO: this check is wrong completely, fix it
            if (abs(tail.getTranslateX() - food.getTranslateX()) - FOOD_BLOCK_SIZE == 0
                    || tail.getTranslateX() == food.getTranslateX()
                    &&
                    abs(tail.getTranslateY() - food.getTranslateY()) - FOOD_BLOCK_SIZE == 0
                    || tail.getTranslateY() == food.getTranslateY()) {
                eatFood(tailX, tailY);
            }

        }
    }

    private void foodTypeRand() {
        int random = (int) Math.ceil(Math.random() * 100);
        if (random % 4 == 0) {
            food.setFoodColor(Color.BLUE);
            score = new Point();
            score = new BasicPoint(score);
        } else if (random % 4 == 1) {
            food.setFoodColor(Color.PINK);
            score = new Point();
            score = new Intership(score);
        } else if (random % 4 == 2) {
            food.setFoodColor(Color.RED);
            score = new Point();
            score = new ForeignLan(score);
        } else if (random % 4 == 3) {
            food.setFoodColor(Color.PURPLE);
            score = new Point();
            score = new TrainingCer(score);
        }
    }

    private void eatFood(double tailX, double tailY) {
        foodTypeRand();
        foodRand(food);
        currentScore += score.addScore();
        stamina += STAMINA_BASE_RAISE_VAL;
        currentScoreVal.setText("" + currentScore);
        // TODO: add the jobs here, after eating 5 jobs score multiply according to eat job
        Body rect = new Body(tailX, tailY, Color.BLACK);
        snake.add(rect);
    }

    private void restartGame() {
        currentScore = 0;
        stopGame();
        startGame();
    }

    private void stopGame() {
        this.running = false;
        timeline.stop();
        snake.clear();
    }

    private void foodRand(Food food) {
        food.setTranslateX((int) (Math.random() * (gameAreaWidth - FOOD_BLOCK_SIZE)) / FOOD_BLOCK_SIZE * FOOD_BLOCK_SIZE);
        food.setTranslateY((int) (Math.random() * (gameAreaHeight - FOOD_BLOCK_SIZE)) / FOOD_BLOCK_SIZE * FOOD_BLOCK_SIZE);
        foodReset(snake, food);
    }

    private void foodReset(ObservableList<Node> snake, Food food) {
        boolean flag = true;
        while (flag) {
            flag = false;
            ListIterator<Node> iterator = snake.listIterator();
            while (iterator.hasNext()) {
                Node x = iterator.next();
                boolean match = x.getTranslateX() == food.getTranslateX() && x.getTranslateY() == food.getTranslateY();
                if (match) {
                    foodRand(food);
                    while (iterator.hasPrevious()) {
                        iterator.previous();
                    }
                }
            }
        }
    }

    // burasi duvardan gecmesini sagliyor.
    private void fieldIsEndless(Body tail) {

        // to the left screeen
        if (tail.getTranslateX() < 0)
            tail.setTranslateX(gameAreaWidth - PYTHON_BLOCK_SIZE);

        // right screen
        if (tail.getTranslateX() >= gameAreaWidth)
            tail.setTranslateX(0.0);

        //top screen
        if (tail.getTranslateY() < 0)
            tail.setTranslateY(gameAreaHeight - PYTHON_BLOCK_SIZE);

        //down screen
        if (tail.getTranslateY() >= gameAreaHeight)
            tail.setTranslateY(0.0);
    }

    // burasi duvardan gecmesin diye
    private void fieldNOTEndless(Body tail, Label scoresVal, Food food) {
        if (tail.getTranslateX() < 0 || tail.getTranslateX() >= gameAreaWidth || tail.getTranslateY() < 0 || tail.getTranslateY() >= gameAreaHeight) {
            allScores.add(currentScore);
            currentScore = 0;
            scoresVal.setText("" + currentScore);
            restartGame();
            foodRand(food);
        }
    }

    public void recursKey(Scene scene) {
        scene.setOnKeyPressed(event -> {
            if (!moved)
                return;

            switch (event.getCode()) {
                case UP:
                    if (direction != Direction.DOWN)
                        direction = Direction.UP;
                    break;
                case DOWN:
                    if (direction != Direction.UP)
                        direction = Direction.DOWN;
                    break;
                case LEFT:
                    if (direction != Direction.RIGHT)
                        direction = Direction.LEFT;
                    break;
                case RIGHT:
                    if (direction != Direction.LEFT)
                        direction = Direction.RIGHT;
                    break;
            }

            moved = false;
        });
    }
}
