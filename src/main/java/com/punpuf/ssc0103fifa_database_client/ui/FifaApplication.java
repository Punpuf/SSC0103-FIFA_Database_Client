package com.punpuf.ssc0103fifa_database_client.ui;

import com.jfoenix.controls.JFXButton;
import com.jfoenix.controls.JFXSnackbar;
import com.jfoenix.controls.JFXSnackbarLayout;
import com.punpuf.ssc0103fifa_database_client.repo.PlayerDataService;
import com.punpuf.ssc0103fifa_database_client.repo.PlayerDataServiceImpl;
import com.punpuf.ssc0103fifa_database_client.vo.Player;
import javafx.application.Application;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.Label;
import javafx.scene.control.ListCell;
import javafx.scene.control.ListView;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.*;
import javafx.stage.FileChooser;
import javafx.stage.Stage;
import javafx.util.Callback;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.util.ArrayList;
import java.util.Objects;

public class FifaApplication extends Application {
    private static final String projectRoot = System.getProperty("user.dir");
    private static final String resourcePathBase = "/com/punpuf/ssc0103fifa_database_client";
    private final PlayerDataService playerDataService = new PlayerDataServiceImpl();
    private Stage primaryStage;

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
        primaryStage = stage;
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

        // Title label
        Label titleLabel = new Label("FIFA Player Selection");
        titleLabel.setStyle("-fx-font-family: 'Monospaced'; -fx-font-size: 24px;");

        // Centered layout with text and button
        VBox centerLayout = new VBox(10);
        centerLayout.setAlignment(Pos.CENTER);
        centerLayout.getChildren().addAll(new Label("Visualize jogares de Futebol da FIFA!"), openButton);

        // Root layout
        VBox rootLayout = new VBox(20);
        rootLayout.setAlignment(Pos.CENTER);
        rootLayout.setPadding(new Insets(20));
        rootLayout.getChildren().addAll(titleLabel, centerLayout);

        // Define current scene
        Scene scene = new Scene(rootLayout, 600, 600);
        String decoratorPath = Objects.requireNonNull(getClass().getResource(resourcePathBase + "/std_stage_decoration.css")).toExternalForm();
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
                playerDataService.setPlayers(players);
                showPlayerListScreen();
            } else {
                showSnackbar("Um arquivo não suportado foi aberto. Tente novamente.");
            }

        } catch (Exception ex) {
            System.out.println("Exception occurred while opening file: " + ex);
            showSnackbar("Um arquivo não suportado foi aberto. Tente novamente.");
        }
    }

    private void showSnackbar(String message) {
        JFXSnackbar snackbar = new JFXSnackbar((Pane) primaryStage.getScene().getRoot());
        snackbar.enqueue(new JFXSnackbar.SnackbarEvent(new JFXSnackbarLayout(message)));
    }

    private void showPlayerListScreen() {
        // Create an AnchorPane as the toolbar container
        AnchorPane toolbar = new AnchorPane();
        toolbar.setStyle("-fx-background-color: #FFFFFF; -fx-padding: 10px;");

        // Center label
        Label toolbarTitle = new Label("Player List");
        toolbarTitle.setStyle("-fx-text-fill: black; -fx-font-size: 18px;");
        AnchorPane.setTopAnchor(toolbarTitle, 10.0);
        AnchorPane.setBottomAnchor(toolbarTitle, 10.0);
        AnchorPane.setLeftAnchor(toolbarTitle, 20.0);
        AnchorPane.setRightAnchor(toolbarTitle, 32.0);

        // Upload button with icon
        Image uploadIcon = new Image(Objects.requireNonNull(getClass().getResourceAsStream(resourcePathBase + "/icons/ic_upload.png")));
        ImageView uploadIconView = new ImageView(uploadIcon);
        uploadIconView.setFitWidth(16);
        uploadIconView.setFitHeight(16);

        JFXButton uploadButton = new JFXButton("Upload", uploadIconView);
        uploadButton.setStyle("-fx-background-color: transparent; -fx-text-fill: black; -fx-font-size: 14px;");

        // Anchor the button to the right
        AnchorPane.setTopAnchor(uploadButton, 10.0);
        AnchorPane.setBottomAnchor(uploadButton, 10.0);
        AnchorPane.setRightAnchor(uploadButton, 16.0); // 16px padding on the right

// Add the label and button to the AnchorPane
        toolbar.getChildren().addAll(toolbarTitle, uploadButton);


        // ListView for players
        ListView<Player> listView = new ListView<>();
        listView.getItems().addAll(playerDataService.getPlayers());

        listView.setCellFactory(new Callback<>() {
            @Override
            public ListCell<Player> call(ListView<Player> param) {
                return new ListCell<>() {
                    @Override
                    protected void updateItem(Player player, boolean empty) {
                        super.updateItem(player, empty);
                        if (empty || player == null) {
                            setText(null);
                            setGraphic(null);
                        } else {
                            String displayText = player.getNomeJogador();
                            if (player.getIdade() != -1) {
                                displayText += ", " + player.getIdade();
                            }
                            setText(displayText);
                            setStyle("-fx-font-size: 16px; -fx-background-color: linear-gradient(to right, #00ff99, #0099ff);");

                            // Add ripple effect
                            addEventFilter(MouseEvent.MOUSE_PRESSED, event -> getStyleClass().add("ripple"));

                            // Divider
                            setBorder(new Border(new BorderStroke(null, null, null,
                                    new BorderWidths(0, 0, 1, 0), new Insets(0, 16, 0, 16))));
                        }
                    }
                };
            }
        });

        listView.setOnMouseClicked(event -> {
            if (event.getClickCount() == 2) {
                Player selectedPlayer = listView.getSelectionModel().getSelectedItem();
                if (selectedPlayer != null) {
                    showPlayerDetailsDialog(selectedPlayer);
                }
            }
        });

        VBox playerListLayout = new VBox(toolbar, listView);
        VBox.setVgrow(listView, Priority.ALWAYS);
        Scene playerListScene = new Scene(playerListLayout, 600, 600);
        primaryStage.setScene(playerListScene);
    }

    private void showPlayerDetailsDialog(Player player) {
        // Player details layout with icons (placeholders for now)
        Label playerId = new Label("ID: " + player.getId());
        // Replace with actual icon
        playerId.setGraphic(new ImageView(new Image("path_to_id_icon.png")));

        Label playerName = new Label(player.getNomeJogador());
        playerName.setGraphic(new ImageView(new Image("path_to_name_icon.png")));

        Label playerAge = new Label(String.valueOf(player.getIdade()));
        if (player.getIdade() == -1) playerAge.setVisible(false);
        playerAge.setGraphic(new ImageView(new Image("path_to_age_icon.png")));

        Label playerNationality = new Label(player.getNacionalidade());
        playerNationality.setGraphic(new ImageView(new Image("path_to_nationality_icon.png")));

        Label playerClub = new Label(player.getNomeClube());
        playerClub.setGraphic(new ImageView(new Image("path_to_club_icon.png")));

        VBox detailsVBox = new VBox(10, playerId, playerName, playerAge, playerNationality, playerClub);
        detailsVBox.setPadding(new Insets(20));
        detailsVBox.setStyle("-fx-background-color: linear-gradient(to right, #008080, #70db70);");

        // Edit Mode Toggle
        JFXButton editButton = new JFXButton("Edit");
        editButton.setOnAction(event -> switchToEditMode(detailsVBox, player));

        detailsVBox.getChildren().add(editButton);

        Scene dialogScene = new Scene(detailsVBox, 300, 400);
        Stage dialogStage = new Stage();
        dialogStage.setTitle("Player Details");
        dialogStage.setScene(dialogScene);
        dialogStage.show();
    }

    private void switchToEditMode(VBox detailsVBox, Player player) {
        detailsVBox.getChildren().clear();

        Label editTitle = new Label("Edit Player");
        editTitle.setStyle("-fx-font-size: 18px; -fx-font-weight: bold;");

        FilledTextField idField = new FilledTextField("ID", String.valueOf(player.getId()));
        FilledTextField nameField = new FilledTextField("Name", player.getNomeJogador());
        FilledTextField ageField = new FilledTextField("Age", String.valueOf(player.getIdade()));
        FilledTextField nationalityField = new FilledTextField("Nationality", player.getNacionalidade());
        FilledTextField clubField = new FilledTextField("Club", player.getNomeClube());

        JFXButton saveButton = new JFXButton("Save");
        saveButton.setOnAction(event -> {
            player.setNomeJogador(nameField.getText());
            player.setIdade(Integer.parseInt(ageField.getText()));
            player.setNacionalidade(nationalityField.getText());
            player.setNomeClube(clubField.getText());
            playerDataService.updatePlayer(player.getId(), player);
            showSnackbar("Player updated successfully!");
        });

        detailsVBox.getChildren().addAll(editTitle, idField, nameField, ageField, nationalityField, clubField, saveButton);
    }
}
