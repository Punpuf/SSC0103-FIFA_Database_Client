package com.punpuf.ssc0103fifa_database_client.repo;

import com.punpuf.ssc0103fifa_database_client.vo.Player;

import java.util.ArrayList;
import java.util.List;
import java.util.Optional;
import java.util.stream.Collectors;

public class PlayerDataServiceImpl implements PlayerDataService {
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
    public void setPlayers(List<Player> players) {
        this.players = new ArrayList<>(players);
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
