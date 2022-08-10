package com.sdProject.scoreDEI.Game;

import java.util.List;
import java.util.Optional;

import com.sdProject.scoreDEI.Team.Team;

import java.util.ArrayList;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

@Service
public class GameService {

    @Autowired
    private GameRepository gameRepository;

    public void addGame(Game game) {
        gameRepository.save(game);
    }

    public Optional<Game> getGame(int id) {
        return gameRepository.findById(id);
    }

    public void deleteGame(Game game) {
        gameRepository.delete(game);
    }

    public Optional<Game> getGameById(int id) {
        return gameRepository.findById(id);
    }

    public List<Game> getAllGames() {
        List<Game> games = new ArrayList<>();
        gameRepository.findAll().forEach(games::add);
        return games;
    }

    public List<Game> getPresentGames() {
        List<Game> games = gameRepository.getPresentGames();
        return games;
    }

    public int getTeamGoals(Team team) {
        int goals = gameRepository.getGameGoals(team);
        return goals;
    }
}
