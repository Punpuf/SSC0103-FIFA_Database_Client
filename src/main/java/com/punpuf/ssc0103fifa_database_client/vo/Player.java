package com.punpuf.ssc0103fifa_database_client.vo;

import org.json.JSONObject;

/**
 * Class that represents the characteristics of a FIFA player
 * @author Jean Patrick Ngandu Mamani
 * @author Marcus da Silva Carr
 */
@SuppressWarnings("unused")
public class Player {
    int id;
    int age;
    String name;
    String country;
    String club;

    public Player(int id, int age, String name, String country, String club) {
        this.id = id;
        this.age = age;
        this.name = name;
        this.country = country;
        this.club = club;
    }

    public static Player fromJSON(JSONObject object) {
        int id = object.getInt("id");
        int age = object.optInt("idade", -1);
        String name = object.optString("nomeJogador");
        String country = object.optString("nacionalidade");
        String club = object.optString("nomeClube");
        return new Player(id, age, name, country, club);
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public int getAge() {
        return age;
    }

    public void setAge(int age) {
        this.age = age;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getCountry() {
        return country;
    }

    public void setCountry(String country) {
        this.country = country;
    }

    public String getClub() {
        return club;
    }

    public void setClub(String club) {
        this.club = club;
    }


    public JSONObject toJSON() {
        JSONObject object = new JSONObject()
                .put("id", id)
                .putOpt("nomeJogador", name)
                .putOpt("nacionalidade", country)
                .putOpt("nomeClube", club);

        if(age != -1) {
            object.put("age", age);
        }

        return object;
    }

    @Override
    public String toString() {
        return "Player{" +
                "ID=" + id +
                ", Age=" + age +
                ", Name='" + name + '\'' +
                ", Nationality='" + country + '\'' +
                ", Club='" + club + '\'' +
                '}';
    }
}

