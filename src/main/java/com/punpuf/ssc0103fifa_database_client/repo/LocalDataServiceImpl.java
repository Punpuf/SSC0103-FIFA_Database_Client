package com.punpuf.ssc0103fifa_database_client.repo;

import com.punpuf.ssc0103fifa_database_client.vo.Player;

import java.util.ArrayList;
import java.util.List;
import java.util.Optional;
import java.util.stream.Collectors;

public class LocalDataServiceImpl implements PlayerDataService {
    private List<Player> players = new ArrayList<>();

    @Override
    public List<Player> getPlayers() {
        return new ArrayList<>(players);
    }

    @Override
    public List<Player> getPlayers(String name, int age, String club, String nationality) {
        return players.stream()
                .filter(player -> name == null || name.isEmpty() || player.getName().toLowerCase().contains(name.toLowerCase()))
                .filter(player -> age == -1 || player.getAge() == age)
                .filter(player -> club == null || club.isEmpty() || player.getClub().toLowerCase().contains(club.toLowerCase()))
                .filter(player -> nationality == null || nationality.isEmpty() || player.getCountry().toLowerCase().contains(nationality.toLowerCase()))
                .collect(Collectors.toList());
    }


    @Override
    public void setPlayers(String csvData) {
        try {
            String[] lines = csvData.split("\n");
            ArrayList<Player> players = new ArrayList<>();

            // Skip the header line and process each line
            for (int i = 1; i < lines.length; i++) {
                String currentLine = lines[i];
                String[] lineProcessedContents = new String[5];
                for (int j = 0; j < 5; j++) {
                    lineProcessedContents[j] = "";
                }
                String[] dividedLine = currentLine.split(",");
                System.arraycopy(dividedLine, 0, lineProcessedContents, 0, dividedLine.length);

                int id = Integer.parseInt(lineProcessedContents[0]);
                int age = lineProcessedContents[1].isEmpty() ? -1 : Integer.parseInt(lineProcessedContents[1]);
                String nomeJogador = lineProcessedContents[2].isEmpty() ? "" : lineProcessedContents[2];
                String nacionalidade = lineProcessedContents[3].isEmpty() ? "" : lineProcessedContents[3];
                String nomeClube = lineProcessedContents[4].isEmpty() ? "" : lineProcessedContents[4];

                players.add(new Player(id, age, nomeJogador, nacionalidade, nomeClube));
            }

            // Update the instance players list
            this.players = players;
            System.out.println(players.size() + " players added.");

        } catch (Exception ex) {
            System.out.println("Exception occurred while processing CSV data: " + ex);
        }
    }


    @Override
    public Player getPlayerById(int id) {
        Optional<Player> playerOpt = players.stream().filter(p -> p.getId() == id).findFirst();
        return playerOpt.orElse(null);
    }

    @Override
    public void updatePlayer(Player player) {
        for (int i = 0; i < players.size(); i++) {
            if (players.get(i).getId() == player.getId()) {
                players.set(i, player);
                break;
            }
        }
    }

    @Override
    public void removePlayer(int id) {
        for (int i = 0; i < players.size(); i++) {
            if (players.get(i).getId() == id) {
                players.remove(i);
                break;
            }
        }
    }
}
