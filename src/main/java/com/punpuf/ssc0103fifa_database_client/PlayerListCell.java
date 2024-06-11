package com.punpuf.ssc0103fifa_database_client;

import com.punpuf.ssc0103fifa_database_client.utils.CountryCodeFinder;
import com.punpuf.ssc0103fifa_database_client.vo.Player;
import javafx.geometry.Insets;
import javafx.scene.control.Label;
import javafx.scene.control.ListCell;
import javafx.scene.image.ImageView;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;

public class PlayerListCell extends ListCell<Player> {
    @Override
    protected void updateItem(Player player, boolean empty) {
        super.updateItem(player, empty);

        if (empty || player == null) {
            setText(null);
            setGraphic(null);
        } else {
            // Player info layout
            HBox cellLayout = new HBox(10);
            cellLayout.setPadding(new Insets(10));

            // Country flag
            ImageView flagView = new ImageView(Paths.getFlagImage(CountryCodeFinder.getCountryCode(player.getCountry())));
            flagView.setFitWidth(64);
            flagView.setPreserveRatio(true);
            //flagView.setFitHeight(32);

            // Player name and info
            VBox playerInfo = new VBox();
            Label playerName = new Label(player.getName());
            playerName.setStyle("-fx-font-size: 18px;");
            Label playerDetails = new Label(player.getAge() == -1 ? "" : "Age: " + player.getAge());

            playerInfo.getChildren().addAll(playerName, playerDetails);
            cellLayout.getChildren().addAll(flagView, playerInfo);

            setGraphic(cellLayout);
        }
    }
}
