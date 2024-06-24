package com.punpuf.ssc0103fifa_database_client.repo;

import com.punpuf.ssc0103fifa_database_client.vo.Player;

import java.io.IOException;
import java.io.PrintStream;
import java.net.Socket;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

import netscape.javascript.JSObject;
import org.json.JSONArray;
import org.json.JSONObject;

public class RemotePlayerService implements PlayerDataService{
    Socket client;
    Scanner scanner;
    PrintStream printStream;

    public RemotePlayerService() throws IOException {
        client = new Socket("127.0.0.1", 5000);
        scanner = new Scanner(client.getInputStream());
        printStream = new PrintStream(client.getOutputStream());
    }

    @Override
    public List<Player> getPlayers() {
        return getPlayers(new Selector());
    }

    @Override
    public List<Player> getPlayers(String name, int age, String club, String nationality) {
        Selector selector = new Selector();
        selector.setAge(age);
        selector.setName(name);
        selector.setClub(club);
        selector.setNationality(nationality);
        return getPlayers(selector);
    }


    private List<Player> getPlayers(Selector selector) {
        JSONObject object = new JSONObject()
                .put("tipo", "buscar")
                .put("seletor", selector.toJSON());

        printStream.println(object);

        while(!scanner.hasNextLine());

        String linha = scanner.nextLine();
        JSONObject resposta = new JSONObject(linha);

        System.out.println(resposta);

        if(!resposta.get("status").equals("ok")) {
            return List.of();
        }

        JSONArray dadosRegistros = resposta.getJSONArray("registros");
        ArrayList<Player> players = new ArrayList<>();

        for(int i = 0; i < dadosRegistros.length(); i++) {
            players.add(Player.fromJSON(dadosRegistros.getJSONObject(i)));
        }

        return players;
    }

    @Override
    public void setPlayers(List<Player> players) {

    }

    @Override
    public Player getPlayerById(int id) {
        Selector selector = new Selector();
        selector.setId(id);
        List<Player> players = getPlayers(selector);
        if(!players.isEmpty()) return players.get(0);

        return null;
    }

    @Override
    public void updatePlayer(Player player) {
        JSONObject object = new JSONObject()
                .put("tipo", "atualizar")
                .put("registro", player.toJSON());
    }

    @Override
    public void removePlayer(int id) {

    }


    private static class Selector {
        int id = -1;
        int age = -1;
        String name = null;
        String nationality = null;
        String club = null;

        public int getAge() {
            return age;
        }

        public void setAge(int age) {
            this.age = age;
        }

        public int getId() {
            return id;
        }

        public void setId(int id) {
            this.id = id;
        }

        public String getName() {
            return name;
        }

        public void setName(String name) {
            this.name = name;
        }

        public String getNationality() {
            return nationality;
        }

        public void setNationality(String nationality) {
            this.nationality = nationality;
        }

        public String getClub() {
            return club;
        }

        public void setClub(String club) {
            this.club = club;
        }

        public JSONObject toJSON() {
            JSONObject object = new JSONObject();

            if(id != -1) {
                object.put("id", id);
            }
            if(age != -1) {
                object.put("idade", age);
            }
            if(name != null && !name.isEmpty()) {
                object.put("nomeJogador", name);
            }
            if(club != null && !club.isEmpty()) {
                object.put("nomeClube", club);
            }
            if(nationality != null && !nationality.isEmpty()) {
                object.put("nacionalidade", nationality);
            }

            return object;
        }

    }
}
