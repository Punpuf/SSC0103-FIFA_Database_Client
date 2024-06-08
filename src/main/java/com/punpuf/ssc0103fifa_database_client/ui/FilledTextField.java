package com.punpuf.ssc0103fifa_database_client.ui;

import javafx.scene.control.TextField;
import javafx.scene.layout.HBox;
import javafx.scene.layout.Priority;

public class FilledTextField extends HBox {
    private final TextField textField;

    public FilledTextField(String promptText, String initialValue) {
        textField = new TextField(initialValue);
        textField.setPromptText(promptText);
        textField.setPrefWidth(200);
        getChildren().add(textField);
        HBox.setHgrow(textField, Priority.ALWAYS);
    }

    public String getText() {
        return textField.getText();
    }
}