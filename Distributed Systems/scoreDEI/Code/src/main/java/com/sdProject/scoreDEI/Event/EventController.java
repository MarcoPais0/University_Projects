package com.sdProject.scoreDEI.Event;

import org.springframework.stereotype.Controller;

import java.util.Optional;

import com.sdProject.scoreDEI.Game.Game;
import com.sdProject.scoreDEI.Game.GameService;
import com.sdProject.scoreDEI.Team.Team;
import com.sdProject.scoreDEI.Team.TeamService;
import com.sdProject.scoreDEI.Player.PlayerService;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.ModelAttribute;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.ui.Model;

@Controller
public class EventController {

    @Autowired
    EventService eventService;

    @Autowired
    GameService gameService;

    @Autowired
    PlayerService playerService;

    @Autowired
    TeamService teamService;

    @GetMapping("/registerEvent")
    public String registerEvent(@RequestParam(name = "id", required = true) int id,
            @RequestParam(name = "eventType", required = true) String eventType, Model model) {
        Optional<Game> op = this.gameService.getGame(id);
        if (op.isPresent()) {
            Game game = op.get();
            model.addAttribute("event", new Event());
            model.addAttribute("game", game);
            model.addAttribute("eventType", eventType);

            if (eventType.equals("Game Started")) {
                if (op.get().getStarted() == false) {
                    game.setStarted(true);
                    try {
                        this.gameService.addGame(game);
                        return "registerOtherEvent";    
                    } catch (Exception e) {
                        return "redirect:/gameStats?id=" + id;
                    }
                }
                return "redirect:/gameStats?id=" + id;

            } else if (eventType.equals("Game Ended")) {
                if (op.get().getEnded() == false) {
                    game.setEnded(true);
                    try {
                        this.gameService.addGame(game);
                        return "registerOtherEvent";    
                    } catch (Exception e) {
                        return "redirect:/gameStats?id=" + id;
                    }
                }
                return "redirect:/gameStats?id=" + id;

            } else if (eventType.equals("Game Interrupted")) {
                if (op.get().getInterrupted() == false) {
                    game.setInterrupted(true);
                    try {
                        this.gameService.addGame(game);
                        return "registerOtherEvent";    
                    } catch (Exception e) {
                        return "redirect:/gameStats?id=" + id;
                    }
                }
                return "redirect:/gameStats?id=" + id;

            } else if (eventType.equals("Game Resumed")) {
                if (op.get().getInterrupted() == true) {
                    game.setInterrupted(false);
                    try {
                        this.gameService.addGame(game);
                        return "registerOtherEvent";    
                    } catch (Exception e) {
                        return "redirect:/gameStats?id=" + id;
                    }
                }
                return "redirect:/gameStats?id=" + id;

            } else
                return "registerEvent";

        } else
            return "redirect:/gameStats?id=" + id;
    }

    @GetMapping("/registerEventUser")
    public String registerEventUser(@RequestParam(name = "id", required = true) int id,
            @RequestParam(name = "eventType", required = true) String eventType, Model model) {
        Optional<Game> op = this.gameService.getGame(id);
        if (op.isPresent()) {
            Game game = op.get();
            model.addAttribute("event", new Event());
            model.addAttribute("game", game);
            model.addAttribute("eventType", eventType);

            if (eventType.equals("Game Started")) {
                if (op.get().getStarted() == false) {
                    game.setStarted(true);
                    try {
                        this.gameService.addGame(game);
                        return "registerOtherEventUser";    
                    } catch (Exception e) {
                        return "redirect:/gameStatsUser?id=" + id;
                    }
                }
                return "redirect:/gameStatsUser?id=" + id;

            } else if (eventType.equals("Game Ended")) {
                if (op.get().getEnded() == false) {
                    game.setEnded(true);
                    try {
                        this.gameService.addGame(game);
                        return "registerOtherEventUser";    
                    } catch (Exception e) {
                        return "redirect:/gameStatsUser?id=" + id;
                    }
                }
                return "redirect:/gameStatsUser?id=" + id;

            } else if (eventType.equals("Game Interrupted")) {
                if (op.get().getInterrupted() == false) {
                    game.setInterrupted(true);
                    try {
                        this.gameService.addGame(game);
                        return "registerOtherEventUser";    
                    } catch (Exception e) {
                        return "redirect:/gameStatsUser?id=" + id;
                    }
                }
                return "redirect:/gameStatsUser?id=" + id;

            } else if (eventType.equals("Game Resumed")) {
                if (op.get().getInterrupted() == true) {
                    game.setInterrupted(false);
                    try {
                        this.gameService.addGame(game);
                        return "registerOtherEventUser";    
                    } catch (Exception e) {
                        return "redirect:/gameStatsUser?id=" + id;
                    }
                }
                return "redirect:/gameStatsUser?id=" + id;

            } else
                return "registerEventUser";

        } else
            return "redirect:/gameStatsUser?id=" + id;
    }

    @PostMapping("/saveEvent")
    public String saveEvent(@ModelAttribute Event event) {
        if (event.getEventType().equals("Goal")) {
            event.getPlayer().setGoals(event.getPlayer().getGoals() + 1);
            this.playerService.addPlayer(event.getPlayer());
        } else if (event.getEventType().equals("Game Ended")) {
            Team homeTeam = event.getGame().getHomeTeam();
            Team awayTeam = event.getGame().getAwayTeam();
            homeTeam.setGames(homeTeam.getGames() + 1);
            awayTeam.setGames(awayTeam.getGames() + 1);
            int homeGoals = this.gameService.getTeamGoals(homeTeam);
            int awayGoals = this.gameService.getTeamGoals(awayTeam);
            if (homeGoals > awayGoals) {
                homeTeam.setWins(homeTeam.getWins() + 1);
                awayTeam.setLosses(awayTeam.getLosses() + 1);
            } else if (homeGoals < awayGoals) {
                homeTeam.setLosses(homeTeam.getLosses() + 1);
                awayTeam.setWins(awayTeam.getWins() + 1);
            } else {
                homeTeam.setDefeats(homeTeam.getDefeats() + 1);
                awayTeam.setDefeats(awayTeam.getDefeats() + 1);
            }

            this.teamService.addTeam(homeTeam);
            this.teamService.addTeam(awayTeam);
        }

        this.eventService.addEvent(event);

        return "redirect:/gameStats?id=" + event.getGame().getId();
    }

    @PostMapping("/saveEventUser")
    public String saveEventUser(@ModelAttribute Event event) {
        if (event.getEventType().equals("Goal")) {
            event.getPlayer().setGoals(event.getPlayer().getGoals() + 1);
            this.playerService.addPlayer(event.getPlayer());
        } else if (event.getEventType().equals("Game Ended")) {
            Team homeTeam = event.getGame().getHomeTeam();
            Team awayTeam = event.getGame().getAwayTeam();
            homeTeam.setGames(homeTeam.getGames() + 1);
            awayTeam.setGames(awayTeam.getGames() + 1);
            int homeGoals = this.gameService.getTeamGoals(homeTeam);
            int awayGoals = this.gameService.getTeamGoals(awayTeam);
            if (homeGoals > awayGoals) {
                homeTeam.setWins(homeTeam.getWins() + 1);
                awayTeam.setLosses(awayTeam.getLosses() + 1);
            } else if (homeGoals < awayGoals) {
                homeTeam.setLosses(homeTeam.getLosses() + 1);
                awayTeam.setWins(awayTeam.getWins() + 1);
            } else {
                homeTeam.setDefeats(homeTeam.getDefeats() + 1);
                awayTeam.setDefeats(awayTeam.getDefeats() + 1);
            }

            this.teamService.addTeam(homeTeam);
            this.teamService.addTeam(awayTeam);
        }

        this.eventService.addEvent(event);

        return "redirect:/gameStatsUser?id=" + event.getGame().getId();
    }
}
