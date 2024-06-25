package com.punpuf.ssc0103fifa_database_client.ui;

import com.jfoenix.controls.JFXButton;
import com.jfoenix.controls.JFXSnackbar;
import com.jfoenix.controls.JFXSnackbarLayout;
import com.punpuf.ssc0103fifa_database_client.repo.LocalDataServiceImpl;
import com.punpuf.ssc0103fifa_database_client.repo.PlayerDataService;
import com.punpuf.ssc0103fifa_database_client.utils.Paths;
import com.punpuf.ssc0103fifa_database_client.vo.Player;
import javafx.application.Application;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.ListView;
import javafx.scene.control.TextField;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.input.Clipboard;
import javafx.scene.input.ClipboardContent;
import javafx.scene.layout.HBox;
import javafx.scene.layout.Priority;
import javafx.scene.layout.VBox;
import javafx.stage.FileChooser;
import javafx.stage.Stage;

import java.io.File;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.List;
import java.util.Objects;

public class MainScreen extends Application {

    private final static int ICON_WIDTH_PX = 16;
    private final PlayerDataService playerDataService;
    private Stage primaryStage;
    private ListView<Player> playerListView;

    public MainScreen() {
        try {
            playerDataService = new LocalDataServiceImpl();
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }

    /**
     * Configures the file chooser for selecting a CSV file.
     *
     * @param fileChooser The file chooser to configure.
     */
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
    public void start(Stage primaryStage) {
        this.primaryStage = primaryStage;
        primaryStage.setTitle("FIFA Player Selection");

        // Custom icon to stage - shows in Windows current running application
        File imageFile = new File(Paths.projectRoot + "\\src\\ic_sports_and_outdoors.png");
        Image icon = new Image(imageFile.toURI().toString());
        primaryStage.getIcons().add(icon);

        // Make window non-resizable
        primaryStage.setResizable(false);

        // Create toolbar
        HBox toolbar = new HBox();
        toolbar.setStyle("-fx-background-color: #009688; -fx-padding: 10px;");
        toolbar.setAlignment(Pos.CENTER_LEFT);

        // Copy button with icon
        ImageView copyIconView = new ImageView(Paths.getIconImage("ic_copy"));
        copyIconView.setFitWidth(ICON_WIDTH_PX);
        copyIconView.setPreserveRatio(true);
        JFXButton copyButton = new JFXButton("Copy", copyIconView);
        copyButton.setStyle("-fx-background-color: transparent; -fx-text-fill: black; -fx-font-size: 14px;");
        copyButton.setOnAction(event -> handleCopyToClipboard());

        // Upload button with icon
        final FileChooser fileChooser = new FileChooser();
        ImageView uploadIconView = new ImageView(Paths.getIconImage("ic_upload"));
        uploadIconView.setFitWidth(ICON_WIDTH_PX);
        uploadIconView.setPreserveRatio(true);
        JFXButton uploadButton = new JFXButton("Upload", uploadIconView);
        uploadButton.setStyle("-fx-background-color: transparent; -fx-text-fill: black; -fx-font-size: 14px;");
        uploadButton.setOnAction(event -> handleFileUpload(primaryStage, fileChooser));

        // Text fields for filtering
        TextField nameField = new TextField();
        nameField.setPromptText("Nome");
        TextField ageField = new TextField();
        ageField.setPromptText("Idade");
        TextField nationalityField = new TextField();
        nationalityField.setPromptText("País");
        TextField clubField = new TextField();
        clubField.setPromptText("Clube");

        nameField.textProperty().addListener((observable, oldValue, newValue) -> filterPlayers(nameField, ageField, nationalityField, clubField));
        ageField.textProperty().addListener((observable, oldValue, newValue) -> filterPlayers(nameField, ageField, nationalityField, clubField));
        nationalityField.textProperty().addListener((observable, oldValue, newValue) -> filterPlayers(nameField, ageField, nationalityField, clubField));
        clubField.textProperty().addListener((observable, oldValue, newValue) -> filterPlayers(nameField, ageField, nationalityField, clubField));

        // Add elements to toolbar
        toolbar.getChildren().addAll(uploadButton, copyButton, nameField, ageField, nationalityField, clubField);
        HBox.setHgrow(nameField, Priority.ALWAYS);
        HBox.setHgrow(ageField, Priority.ALWAYS);
        HBox.setHgrow(nationalityField, Priority.ALWAYS);
        HBox.setHgrow(clubField, Priority.ALWAYS);

        // Create ListView for players
        playerListView = new ListView<>();
        playerListView.setPrefHeight(400);
        playerListView.setCellFactory(param -> new PlayerListCell()); // Custom cell factory for player items

        // Add item click handler to open player details dialog
        playerListView.setOnMouseClicked(event -> {
            if (event.getClickCount() == 2) { // Double-click to open dialog
                Player selectedPlayer = playerListView.getSelectionModel().getSelectedItem();
                if (selectedPlayer != null) {
                    openPlayerDetailDialog(primaryStage, selectedPlayer);
                }
            }
        });

        // Populate ListView with player data
        ObservableList<Player> players = FXCollections.observableArrayList(playerDataService.getPlayers());
        playerListView.setItems(players);

        // Layout for main screen
        VBox mainLayout = new VBox(toolbar, playerListView);
        mainLayout.setPadding(new Insets(0));
        Scene scene = new Scene(mainLayout, 800, 400);
        scene.getStylesheets().add(Objects.requireNonNull(
                getClass().getResource(Paths.resourcePathBase + "/window_toolbar.css")).toExternalForm());
        primaryStage.setScene(scene);
        primaryStage.show();
    }

    /**
     * Handles the file upload action.
     *
     * @param primaryStage The main stage of the application.
     * @param fileChooser  The file chooser for selecting the CSV file.
     */
    private void handleFileUpload(Stage primaryStage, FileChooser fileChooser) {
        configureFileChooser(fileChooser);
        File file = fileChooser.showOpenDialog(primaryStage);
        if (file != null) {
            openFile(file);
        }
    }

    /**
     * Opens and processes the selected CSV file.
     *
     * @param file The selected file.
     */
    private void openFile(File file) {
        try {
            String filePath = file.getAbsolutePath();
            String fileContents = Files.readString(Path.of(filePath));

            playerDataService.setPlayers(fileContents);
            List<Player> players = playerDataService.getPlayers();

            if (!players.isEmpty()) {
                playerListView.setItems(FXCollections.observableArrayList(playerDataService.getPlayers()));
            } else {
                showSnackbar("Um arquivo não suportado foi aberto. Tente novamente.");
            }

        } catch (Exception ex) {
            System.out.println("Exception occurred while opening file: " + ex);
            showSnackbar("Um arquivo não suportado foi aberto. Tente novamente.");
        }
    }

    /**
     * Handles the action of copying the player list to the clipboard.
     */
    private void handleCopyToClipboard() {
        List<Player> playerList = playerListView.getItems();
        if (playerList == null || playerList.isEmpty()) {
            showSnackbar("Lista de jogadores está vazia. Tente novamente.");
            return;
        }

        // Convert player list to string
        StringBuilder playerListStr = new StringBuilder();
        for (Player player : playerList) {
            playerListStr.append(player.toString()).append("\n");
        }

        // Copy to clipboard
        Clipboard clipboard = Clipboard.getSystemClipboard();
        ClipboardContent content = new ClipboardContent();
        content.putString(playerListStr.toString());
        clipboard.setContent(content);

        showSnackbar("Lista de jogadores copiada para o clipboard.");
    }

    /**
     * Filters the players list based on the input criteria from the text fields.
     *
     * @param nameField        The text field for player name.
     * @param ageField         The text field for player age.
     * @param nationalityField The text field for player nationality.
     * @param clubField        The text field for player club name.
     */
    private void filterPlayers(TextField nameField, TextField ageField, TextField nationalityField, TextField clubField) {
        String name = nameField.getText().trim();
        String ageText = ageField.getText().trim();
        String nationality = nationalityField.getText().trim();
        String club = clubField.getText().trim();

        int age = ageText.isEmpty() ? -1 : Integer.parseInt(ageText);

        List<Player> filteredPlayers = playerDataService.getPlayers(name, age, club, nationality);
        playerListView.setItems(FXCollections.observableArrayList(filteredPlayers));
    }

    /**
     * Shows a snackbar with a given message.
     *
     * @param message The message to display in the snackbar.
     */
    private void showSnackbar(String message) {
        JFXSnackbar snackbar = new JFXSnackbar((VBox) primaryStage.getScene().getRoot());
        snackbar.enqueue(new JFXSnackbar.SnackbarEvent(new JFXSnackbarLayout(message)));
    }

    /**
     * Opens the player detail dialog.
     *
     * @param primaryStage The main stage of the application.
     * @param player       The selected player to display details for.
     */
    private void openPlayerDetailDialog(Stage primaryStage, Player player) {
        PlayerDetailDialog dialog = new PlayerDetailDialog(primaryStage, player, playerDataService);
        dialog.setResizable(false);
        dialog.showAndWait();
        playerListView.getItems().clear();
        playerListView.getItems().addAll(playerDataService.getPlayers());
    }
}
