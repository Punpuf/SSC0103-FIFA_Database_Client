package com.punpuf.ssc0103fifa_database_client.repo;

import com.punpuf.ssc0103fifa_database_client.vo.Player;

import java.util.ArrayList;
import java.util.List;
import java.util.Optional;

public class PlayerDataServiceImpl implements PlayerDataService {
    private List<Player> players = new ArrayList<>();

    @Override
    public List<Player> getPlayers() {
        return new ArrayList<>(players);
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
    public void updatePlayer(int id, Player player) {
        for (int i = 0; i < players.size(); i++) {
            if (players.get(i).getId() == id) {
                players.set(i, player);
                break;
            }
        }
    }
}
