package com.punpuf.ssc0103fifa_database_client.ui;

import com.jfoenix.controls.JFXButton;
import com.punpuf.ssc0103fifa_database_client.repo.PlayerDataService;
import com.punpuf.ssc0103fifa_database_client.utils.CountryCodeFinder;
import com.punpuf.ssc0103fifa_database_client.utils.Paths;
import com.punpuf.ssc0103fifa_database_client.vo.Player;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.Scene;
import javafx.scene.control.Label;
import javafx.scene.control.TextField;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.layout.HBox;
import javafx.scene.layout.Priority;
import javafx.scene.layout.VBox;
import javafx.stage.Modality;
import javafx.stage.Stage;
import javafx.stage.StageStyle;

/**
 * This class represents a dialog window that displays details about a player.
 * It allows viewing and editing of player information.
 */
public class PlayerDetailDialog extends Stage {

    private static final int ICON_WIDTH_PX = 16; // Icon width in pixels
    private final PlayerDataService playerDataService; // Service for managing player data
    private final VBox editModeContent; // Layout for edit mode
    private final VBox mainContent; // Main layout
    private boolean isEditMode = false; // Indicates whether the dialog is in edit mode
    private Player player; // The player data to display and edit
    // Toolbar components
    private Label toolbarTitle; // Title of the toolbar
    private JFXButton deleteButton; // Button to delete current player
    private JFXButton editButton; // Button to toggle edit mode
    // Main content components
    private VBox viewModeContent; // Layout for displaying player details

    /**
     * Constructs a PlayerDetailDialog to display and edit player details.
     *
     * @param owner             The parent stage from which this dialog is shown.
     * @param player            The player whose details are to be displayed and edited.
     * @param playerDataService The data service to manage player information.
     */
    public PlayerDetailDialog(Stage owner, Player player, PlayerDataService playerDataService) {
        // Set up dialog properties
        initOwner(owner);
        initModality(Modality.APPLICATION_MODAL);
        initStyle(StageStyle.UTILITY);
        setWidth(400);
        setHeight(300);

        this.player = player;
        this.playerDataService = playerDataService;

        // Create toolbar
        HBox toolbar = createToolbar();

        // Create main content area
        mainContent = new VBox(10);
        mainContent.setPadding(new Insets(10));
        mainContent.setAlignment(Pos.TOP_LEFT);

        // Create content for view mode
        viewModeContent = createViewModeContent();
        mainContent.getChildren().add(viewModeContent);

        // Create content for edit mode
        editModeContent = createEditModeContent();

        // Set up the main layout
        VBox layout = new VBox(toolbar, mainContent);
        layout.setAlignment(Pos.TOP_LEFT);

        // Set up scene
        Scene scene = new Scene(layout);
        setScene(scene);
    }

    /**
     * Creates the toolbar containing the title and the edit button.
     *
     * @return A configured HBox containing the toolbar components.
     */
    private HBox createToolbar() {
        // Toolbar title
        toolbarTitle = new Label("Player Details");
        toolbarTitle.setStyle("-fx-text-fill: black; -fx-font-size: 14px;");
        toolbarTitle.setMaxWidth(Integer.MAX_VALUE);
        HBox.setHgrow(toolbarTitle, Priority.ALWAYS); // Expand the title, leaving the edit button to the right

        // Edit button with icon
        Image editIcon = Paths.getIconImage("ic_edit");
        ImageView editIconView = new ImageView(editIcon);
        editIconView.setFitWidth(ICON_WIDTH_PX);
        editIconView.setPreserveRatio(true);
        editButton = new JFXButton("", editIconView);
        editButton.setStyle("-fx-background-color: transparent; -fx-text-fill: white;");
        editButton.setOnAction(event -> toggleEditMode());

        // Delete button with icon
        Image deleteIcon = Paths.getIconImage("ic_delete");
        ImageView deleteIconView = new ImageView(deleteIcon);
        deleteIconView.setFitWidth(ICON_WIDTH_PX);
        deleteIconView.setPreserveRatio(true);
        deleteButton = new JFXButton("", deleteIconView);
        deleteButton.setStyle("-fx-background-color: transparent; -fx-text-fill: white;");
        deleteButton.setOnAction(event -> deleteCurrentPlayer());

        // Toolbar layout
        HBox toolbar = new HBox(10.0, toolbarTitle, editButton, deleteButton);
        toolbar.setStyle("-fx-background-color: #009688; -fx-padding: 14px;");
        toolbar.setAlignment(Pos.CENTER_LEFT);

        return toolbar;
    }

    /**
     * Creates the layout for viewing player details.
     *
     * @return A VBox configured to display player details.
     */
    private VBox createViewModeContent() {
        VBox content = new VBox(10);
        content.setAlignment(Pos.TOP_LEFT);

        // Country flag of the player
        ImageView flagView = new ImageView(Paths.getFlagImage(CountryCodeFinder.getCountryCode(player.getCountry())));
        flagView.setFitHeight(50);
        flagView.setPreserveRatio(true);

        // Player name
        Label playerName = new Label(player.getName());
        playerName.setStyle("-fx-font-size: 24px; -fx-font-weight: bold;");

        // Additional player information with icons (placeholders)
        ImageView cakeIconImg = new ImageView(Paths.getIconImage("ic_cake"));
        cakeIconImg.setFitHeight(ICON_WIDTH_PX);
        cakeIconImg.setPreserveRatio(true);
        HBox infoBox1 = new HBox(5, cakeIconImg, new Label("Age: " + player.getAge()));

        ImageView groupIconImg = new ImageView(Paths.getIconImage("ic_groups"));
        groupIconImg.setFitHeight(ICON_WIDTH_PX);
        groupIconImg.setPreserveRatio(true);
        HBox infoBox2 = new HBox(5, groupIconImg, new Label("Club: " + player.getClub()));

        // Add all components to the view mode layout
        content.getChildren().addAll(flagView, playerName, infoBox1, infoBox2);

        return content;
    }

    /**
     * Creates the layout for editing player details.
     *
     * @return A VBox configured with editable fields for player details.
     */
    private VBox createEditModeContent() {
        VBox content = new VBox(10);
        content.setAlignment(Pos.TOP_LEFT);
        content.setPadding(new Insets(10));

        // Editable fields for player details
        TextField playerNameField = new TextField(player.getName());
        playerNameField.setPromptText("Player Name");
        TextField ageField = new TextField(String.valueOf(player.getAge()));
        ageField.setPromptText("Age");
        TextField clubField = new TextField(player.getClub());
        clubField.setPromptText("Club");

        // Save button
        JFXButton saveButton = new JFXButton("Save");
        saveButton.setStyle("-fx-background-color: #009688; -fx-text-fill: white;");
        saveButton.setOnAction(event -> {
            // Update player details and save to data service
            player.setName(playerNameField.getText());
            player.setAge(Integer.parseInt(ageField.getText()));
            player.setClub(clubField.getText());
            playerDataService.updatePlayer(player); // Save changes to the data service
            player = playerDataService.getPlayerById(player.getId());
            toggleEditMode(); // Switch back to view mode
        });

        // Add all components to the edit mode layout
        content.getChildren().addAll(playerNameField, ageField, clubField, saveButton);

        return content;
    }

    /**
     * Toggles between the view and edit modes of the dialog.
     */
    private void toggleEditMode() {
        isEditMode = !isEditMode;
        if (isEditMode) {
            toolbarTitle.setText("Editing Details");
            toolbarTitle.setStyle("-fx-text-fill: black; -fx-font-size: 14px;");
            editButton.setVisible(false);

            mainContent.getChildren().clear();
            mainContent.getChildren().add(editModeContent);
        } else {
            toolbarTitle.setText("Player Details");
            toolbarTitle.setStyle("-fx-text-fill: black; -fx-font-size: 14px;");
            editButton.setVisible(true);

            viewModeContent = createViewModeContent();
            mainContent.getChildren().clear();
            mainContent.getChildren().add(viewModeContent);
        }
    }

    /**
     * Deletes the current player from the database and closes the dialog.
     */
    private void deleteCurrentPlayer() {
        // Check if the player exists in the database
        if (player != null && player.getId() != -1) {
            // Delete the player using the data service
            playerDataService.removePlayer(player.getId());
            // Close the dialog after deletion
            close();
        } else {
            // Handle the case where the player is invalid or not found
            System.out.println("Player not found or already deleted.");
        }
    }
}
