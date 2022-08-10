package com.sdProject.scoreDEI.Team;

import java.util.List;
import java.util.Optional;
import java.util.ArrayList;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

@Service
public class TeamService {

    @Autowired
    private TeamRepository teamRepository;

    public void addTeam(Team team) {
        teamRepository.save(team);
    }

    public void deleteTeam(Team team) {
        teamRepository.delete(team);
    }

    public Optional<Team> getTeam(int id) {
        return teamRepository.findById(id);
    }

    public Team getTeamByName(String name) {
        return teamRepository.findByName(name);
    }

    public List<Team> getAllTeams() {
        List<Team> teams = new ArrayList<>();
        teamRepository.findAll().forEach(teams::add);
        return teams;
    }

    public List<Team> getTeamsDescending() {
        List<Team> teams = teamRepository.getTeamsDescending();
        return teams;
    }

    public List<Team> getTeamsAscending() {
        List<Team> teams = teamRepository.getTeamsAscending();
        return teams;
    }

    public List<Team> getGamesDescending() {
        List<Team> teams = teamRepository.getGamesDescending();
        return teams;
    }

    public List<Team> getGamesAscending() {
        List<Team> teams = teamRepository.getGamesAscending();
        return teams;
    }

    public List<Team> getWinsDescending() {
        List<Team> teams = teamRepository.getWinsDescending();
        return teams;
    }

    public List<Team> getWinsAscending() {
        List<Team> teams = teamRepository.getWinsAscending();
        return teams;
    }

    public List<Team> getLossesDescending() {
        List<Team> teams = teamRepository.getLossesDescending();
        return teams;
    }

    public List<Team> getLossesAscending() {
        List<Team> teams = teamRepository.getLossesAscending();
        return teams;
    }

    public List<Team> getDefeatsDescending() {
        List<Team> teams = teamRepository.getDefeatsDescending();
        return teams;
    }

    public List<Team> getDefeatsAscending() {
        List<Team> teams = teamRepository.getDefeatsAscending();
        return teams;
    }
}
