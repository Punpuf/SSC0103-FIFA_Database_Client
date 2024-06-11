module com.punpuf.ssc0103fifa_database_client {
    requires javafx.controls;
    requires javafx.fxml;

    requires org.controlsfx.controls;
    requires com.dlsc.formsfx;
    requires org.kordamp.ikonli.javafx;
    requires org.kordamp.bootstrapfx.core;
    requires com.jfoenix;
    requires java.desktop;

    opens com.punpuf.ssc0103fifa_database_client to javafx.fxml;
    exports com.punpuf.ssc0103fifa_database_client;
    exports com.punpuf.ssc0103fifa_database_client.ui;
    opens com.punpuf.ssc0103fifa_database_client.ui to javafx.fxml;
    exports com.punpuf.ssc0103fifa_database_client.utils;
    opens com.punpuf.ssc0103fifa_database_client.utils to javafx.fxml;
    exports com.punpuf.ssc0103fifa_database_client.vo;
    opens com.punpuf.ssc0103fifa_database_client.vo to javafx.fxml;
}