package com.punpuf.ssc0103fifa_database_client.ui;

import com.jfoenix.controls.JFXButton;
import com.punpuf.ssc0103fifa_database_client.Paths;
import com.punpuf.ssc0103fifa_database_client.repo.PlayerDataService;
import com.punpuf.ssc0103fifa_database_client.utils.CountryCodeFinder;
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

public class PlayerDetailDialog extends Stage {

    private final static int ICON_WIDTH_PX = 16;
    private final PlayerDataService playerDataService; // Data service for managing player data
    private final VBox editModeContent;
    private final VBox mainContent;
    private boolean isEditMode = false;
    private Player player; // The player data to display
    // Toolbar components
    private Label toolbarTitle;
    private JFXButton editButton;
    // Main content components
    private VBox viewModeContent;

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

        // Create content area
        mainContent = new VBox(10);
        mainContent.setPadding(new Insets(10));
        mainContent.setAlignment(Pos.TOP_LEFT);

        // Create view mode content
        viewModeContent = createViewModeContent();
        mainContent.getChildren().add(viewModeContent);

        // Create edit mode content
        editModeContent = createEditModeContent();

        // Set up the main layout
        VBox layout = new VBox(toolbar, mainContent);
        layout.setAlignment(Pos.TOP_LEFT);

        // Set up scene
        Scene scene = new Scene(layout);
        setScene(scene);
    }

    private HBox createToolbar() {
        // Close button with icon
        /*Image closeIcon = new Image(Objects.requireNonNull(getClass().getResourceAsStream(resourcePathBase + "/icons/ic_close.png")));
        ImageView closeIconView = new ImageView(closeIcon);
        closeIconView.setFitHeight(ICON_WIDTH_PX);
        closeIconView.setPreserveRatio(true);
        JFXButton closeButton = new JFXButton("", closeIconView);
        closeButton.setStyle("-fx-background-color: transparent;");
        closeButton.setOnAction(event -> close());*/

        // Toolbar title
        toolbarTitle = new Label("Detalhes do jogador");
        toolbarTitle.setStyle("-fx-text-fill: black; -fx-font-size: 14px;");
        toolbarTitle.setMaxWidth(Integer.MAX_VALUE);
        HBox.setHgrow(toolbarTitle, Priority.ALWAYS); // Make title expand its size, leaving edit button to the right


        // Edit button with icon
        Image editIcon = Paths.getIconImage("ic_edit");
        ImageView editIconView = new ImageView(editIcon);
        editIconView.setFitWidth(ICON_WIDTH_PX);
        editIconView.setPreserveRatio(true);
        editButton = new JFXButton("", editIconView);
        editButton.setStyle("-fx-background-color: transparent; -fx-text-fill: white;");
        editButton.setOnAction(event -> toggleEditMode());
        //HBox.setMargin(editButton, new Insets(0, 16, 0, 0));

        // Toolbar layout
        HBox toolbar = new HBox(10.0, toolbarTitle, editButton);
        toolbar.setStyle("-fx-background-color: #009688; -fx-padding: 14px;");
        toolbar.setAlignment(Pos.CENTER_LEFT);


        return toolbar;
    }

    private VBox createViewModeContent() {
        VBox content = new VBox(10);
        content.setAlignment(Pos.TOP_LEFT);

        // Country flag
        ImageView flagView = new ImageView(Paths.getFlagImage(CountryCodeFinder.getCountryCode(player.getCountry())));
        flagView.setFitHeight(50);
        flagView.setPreserveRatio(true);

        // Player name
        Label playerName = new Label(player.getName());
        playerName.setStyle("-fx-font-size: 24px; -fx-font-weight: bold;");

        // Other player information with icons (placeholders)
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

    private VBox createEditModeContent() {
        VBox content = new VBox(10);
        content.setAlignment(Pos.TOP_LEFT);
        content.setPadding(new Insets(10));

        // Editable fields for player details
        TextField playerNameField = new TextField(player.getName());
        playerNameField.setPromptText("Nome do/a jogador/a");
        TextField ageField = new TextField(String.valueOf(player.getAge()));
        ageField.setPromptText("Idade");
        TextField clubField = new TextField(player.getClub());
        clubField.setPromptText("Futebol Clube");

        // Save button
        JFXButton saveButton = new JFXButton("Salvar");
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

    private void toggleEditMode() {
        isEditMode = !isEditMode;
        if (isEditMode) {
            toolbarTitle.setText("Editando infos");
            toolbarTitle.setStyle("-fx-text-fill: black; -fx-font-size: 14px;");
            editButton.setVisible(false);

            mainContent.getChildren().clear();
            mainContent.getChildren().add(editModeContent);
        } else {
            toolbarTitle.setText("Detalhes do jogador");
            toolbarTitle.setStyle("-fx-text-fill: black; -fx-font-size: 14px;");
            editButton.setVisible(true);

            viewModeContent = createViewModeContent();
            mainContent.getChildren().clear();
            mainContent.getChildren().add(viewModeContent);
        }
    }
}
