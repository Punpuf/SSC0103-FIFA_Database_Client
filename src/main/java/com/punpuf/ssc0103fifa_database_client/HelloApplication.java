package com.punpuf.ssc0103fifa_database_client;

import com.jfoenix.controls.JFXButton;
import javafx.application.Application;
import javafx.geometry.Insets;
import javafx.scene.Scene;
import javafx.scene.image.Image;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.Pane;
import javafx.scene.layout.VBox;
import javafx.stage.FileChooser;
import javafx.stage.Stage;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.util.ArrayList;
import java.util.Objects;

public class HelloApplication extends Application {
    private static final String projectRoot = System.getProperty("user.dir");
    //private static final Paint themeGreen300 = Color.rgb(101, 159, 101);

    private static void configureFileChooser(final FileChooser fileChooser) {
        fileChooser.setTitle("Selecionar arquivo de jogadores FIFA...");
        fileChooser.setInitialDirectory(
                new File(System.getProperty("user.home"))
        );
        fileChooser.getExtensionFilters().add(new FileChooser.ExtensionFilter("Arquivos CSV (*.csv)", "*.csv"));
    }

    public static void main(String[] args) {
        launch(args);
    }

    @Override
    public void start(Stage stage) {
        stage.setTitle("FIFA Player Explorer");
        stage.setWidth(600);
        stage.setHeight(600);

        // Custom icon to stage - shows in Windows current running application
        File imageFile = new File(projectRoot + "\\src\\ic_sports_and_outdoors.png");
        Image icon = new Image(imageFile.toURI().toString());
        stage.getIcons().add(icon);

        final FileChooser fileChooser = new FileChooser();

        final JFXButton openButton = new JFXButton("Fazer upload");

        openButton.setOnAction(actionEvent -> {
            configureFileChooser(fileChooser);
            File file = fileChooser.showOpenDialog(stage);
            if (file != null) {
                openFile(file);
            }
        });

        final GridPane inputGridPane = new GridPane();

        GridPane.setConstraints(openButton, 0, 0);
        inputGridPane.setHgap(6);
        inputGridPane.setVgap(6);
        inputGridPane.getChildren().addAll(openButton);

        final Pane rootGroup = new VBox(12);
        rootGroup.getChildren().addAll(inputGridPane);
        rootGroup.setPadding(new Insets(12, 12, 12, 12));

        // Define current scene
        Scene scene = new Scene(rootGroup, 500, 500);
        String decoratorPath = Objects.requireNonNull(getClass().getResource("std_stage_decoration.css")).toExternalForm();
        scene.getStylesheets().add(decoratorPath);

        // Show scene
        stage.setScene(scene);
        stage.show();
    }

    private void openFile(File file) {
        try {
            System.out.println("File has been selected, its path is: " + file.getAbsolutePath());

            String filePath = file.getAbsolutePath();
            BufferedReader reader = new BufferedReader(new FileReader(filePath));
            reader.readLine(); // read header line, and discard it

            String line;
            ArrayList<Player> players = new ArrayList<>();
            while ((line = reader.readLine()) != null) {

                String[] lineProcessedContents = new String[5];
                String[] dividedLine = line.split(",");
                System.arraycopy(dividedLine, 0, lineProcessedContents, 0, dividedLine.length);

                int id = Integer.parseInt(lineProcessedContents[0]);
                int age;
                if (Objects.equals(lineProcessedContents[1], "")) age = -1;
                else age = Integer.parseInt(lineProcessedContents[1]);

                String nomeJogador;
                if (lineProcessedContents[2] == null || Objects.equals(lineProcessedContents[2], "")) nomeJogador = "";
                else nomeJogador = lineProcessedContents[2];

                String nacionalidade;
                if (lineProcessedContents[3] == null || Objects.equals(lineProcessedContents[3], ""))
                    nacionalidade = "";
                else nacionalidade = lineProcessedContents[3];

                String nomeClube;
                if (lineProcessedContents[4] == null || Objects.equals(lineProcessedContents[4], "")) nomeClube = "";
                else nomeClube = lineProcessedContents[4];

                players.add(new Player(id, age, nomeJogador, nacionalidade, nomeClube));
            }

            System.out.println(players.size() + " players added.");
            if (!players.isEmpty()) {
                System.out.println("The first added player is: " + players.get(0));
                System.out.println("The last added player is: " + players.get(players.size() - 1));
            }

        } catch (Exception ex) {
            System.out.println("Exception occurred while opening file: " + ex);
        }
    }
}