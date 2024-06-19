package com.punpuf.ssc0103fifa_database_client.ui;

import javafx.scene.control.TextField;
import javafx.scene.layout.HBox;
import javafx.scene.layout.Priority;

public class FilledTextField extends HBox {

    public FilledTextField(String promptText, String initialValue) {
        TextField textField = new TextField(initialValue);
        textField.setPromptText(promptText);
        textField.setPrefWidth(200);
        getChildren().add(textField);
        HBox.setHgrow(textField, Priority.ALWAYS);
    }
}