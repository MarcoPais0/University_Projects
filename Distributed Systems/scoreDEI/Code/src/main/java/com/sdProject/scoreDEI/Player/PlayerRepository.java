package com.sdProject.scoreDEI.Player;

import java.util.List;

import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.CrudRepository;

public interface PlayerRepository extends CrudRepository<Player, Integer> {
    @Query("SELECT p FROM Player p ORDER BY name DESC")
    List<Player> getPlayersDescending();

    @Query("SELECT p FROM Player p ORDER BY name ASC")
    List<Player> getPlayersAscending();

    @Query("SELECT p FROM Player p ORDER BY goals DESC")
    List<Player> getGoalsDescending();

    @Query("SELECT p FROM Player p ORDER BY goals ASC")
    List<Player> getGoalsAscending();
}