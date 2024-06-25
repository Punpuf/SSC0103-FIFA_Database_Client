package com.punpuf.ssc0103fifa_database_client.repo;

import com.punpuf.ssc0103fifa_database_client.vo.Player;

import java.util.List;

public interface PlayerDataService {
    List<Player> getPlayers();

    List<Player> getPlayers(String name, int age, String club, String nationality);

    void setPlayers(String csvData);

    Player getPlayerById(int id);

    void updatePlayer(Player player);

    void removePlayer(int id);
}
