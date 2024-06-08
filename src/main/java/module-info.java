module com.punpuf.ssc0103fifa_database_client {
    requires javafx.controls;
    requires javafx.fxml;

    requires org.controlsfx.controls;
    requires com.dlsc.formsfx;
    requires org.kordamp.ikonli.javafx;
    requires org.kordamp.bootstrapfx.core;
    requires com.jfoenix;

    opens com.punpuf.ssc0103fifa_database_client to javafx.fxml;
    exports com.punpuf.ssc0103fifa_database_client;
}