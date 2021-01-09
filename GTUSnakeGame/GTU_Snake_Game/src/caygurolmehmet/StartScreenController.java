package caygurolmehmet;

/**
 * Created by Mehmet Gürol Çay on 24/10/2017.
 */
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.stage.Stage;
import java.io.IOException;

import static caygurolmehmet.GameScreenController.PythonType;

public class StartScreenController {

    PythonType pythonType = PythonType.PYTHON;
    private Stage stage;

    @FXML
    private Button newGame;

    @FXML
    private Button load;

    @FXML
    private Button exit;

    @FXML
    private Button python;

    @FXML
    private Button anaconda;

    @FXML
    void selectAnaconda(ActionEvent event) {
        pythonType = PythonType.ANACONDA;
    }

    @FXML
    void selectPython(ActionEvent event) {
        pythonType = PythonType.PYTHON;
    }

    @FXML
    public void exitGame(ActionEvent event) {
        System.exit(0);
    }

    @FXML
    public void loadGame(ActionEvent event) {
        stage = (Stage) load.getScene().getWindow();

        try {
            Parent root = FXMLLoader.load(getClass().getResource("loadScreen.fxml"));
            Scene gameScene = new Scene(root);
            stage.setScene(gameScene);
            stage.show();

        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    @FXML
    public void startGame(ActionEvent event) {
        Scene gameScene;

        GameScreenController gameScreenController = new GameScreenController(pythonType);

        stage = (Stage) newGame.getScene().getWindow();

        try {
            gameScene = new Scene(gameScreenController.createContent());
            gameScreenController.recursKey(gameScene);
            stage.setScene(gameScene);
            stage.show();
            gameScreenController.startGame();
        } catch (Exception e) {
            e.printStackTrace();
        }

    }

}
