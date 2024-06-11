package com.punpuf.ssc0103fifa_database_client;

import javafx.scene.image.Image;

import java.util.Objects;

public class Paths {
    public static final String projectRoot = System.getProperty("user.dir");
    public static final String resourcePathBase = "/com/punpuf/ssc0103fifa_database_client";

    public static Image getIconImage(String pngIconName) {
        System.out.println(resourcePathBase + "\\icons\\" + pngIconName + ".png");
        return new Image(Objects.requireNonNull(Paths.class.getResourceAsStream(resourcePathBase + "/icons/" + pngIconName + ".png")));
    }

    public static Image getFlagImage(String pngIconName) {
        return new Image(Objects.requireNonNull(
                Paths.class.getResourceAsStream(resourcePathBase + "/country_flags/" + pngIconName.toLowerCase() + ".png")));
    }
}
