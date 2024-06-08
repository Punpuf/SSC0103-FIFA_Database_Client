package com.punpuf.ssc0103fifa_database_client.repo;

import com.punpuf.ssc0103fifa_database_client.vo.Player;

import java.util.List;

public interface PlayerDataService {
    List<Player> getPlayers();

    void setPlayers(List<Player> players);

    Player getPlayerById(int id);

    void updatePlayer(int id, Player player);
}
