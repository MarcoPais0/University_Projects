package com.sdProject.scoreDEI.Views;

import java.text.SimpleDateFormat;
import java.util.Collections;
import java.util.Comparator;
import java.util.Date;
import java.util.List;
import java.util.Optional;

import com.sdProject.scoreDEI.Event.Event;
import com.sdProject.scoreDEI.Game.Game;
import com.sdProject.scoreDEI.Game.GameService;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.ui.Model;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestParam;

@Controller
public class ViewsController {
    @Autowired
    GameService gameService;

    @GetMapping("/homepage")
    public String homepageAdmin(Model model) {
        model.addAttribute("games", this.gameService.getPresentGames());
        return "homepage";
    }

    @GetMapping("/homepageUser")
    public String homepageUser(Model model) {
        model.addAttribute("games", this.gameService.getPresentGames());
        return "homepageUser";
    }

    @GetMapping("/homepageUnsigned")
    public String homepageUnsigned(Model model) {
        model.addAttribute("games", this.gameService.getPresentGames());
        return "homepageUnsigned";
    }

    @GetMapping("/gameStats")
    public String gameStats(@RequestParam(name = "id", required = true) int id, Model model) {
        Optional<Game> op = this.gameService.getGameById(id);
        long time = -1;
        if (op.isPresent()) {
            Game game = op.get();
            List<Event> events = game.getEvents();
            Collections.sort(events, new Comparator<Event>() {
                @Override
                public int compare(Event e1, Event e2) {
                    return e1.getTime().compareTo(e2.getTime());
                }
            });
            if (game.getStarted()) {
                SimpleDateFormat format = new SimpleDateFormat("HH:mm:ss");
                try {
                    Date time1 = format.parse(java.time.LocalTime.now().toString());
                    Date time2 = format.parse(game.getTime().toString());
                    time = time1.getTime() - time2.getTime();
                    time = (time / (60 * 1000)) + 60;
                } catch (Exception e) {
                    time = -1;
                }
            }
            model.addAttribute("game", game);
            model.addAttribute("homeGoals", this.gameService.getTeamGoals(game.getHomeTeam()));
            model.addAttribute("awayGoals", this.gameService.getTeamGoals(game.getAwayTeam()));
            model.addAttribute("events", events);
            model.addAttribute("gameTime", time);
            return "gameStats";
        } else {
            return "redirect:/homepage";
        }
    }

    @GetMapping("/gameStatsUser")
    public String gameStatsUser(@RequestParam(name = "id", required = true) int id, Model model) {
        Optional<Game> op = this.gameService.getGameById(id);
        long time = -1;
        if (op.isPresent()) {
            Game game = op.get();
            List<Event> events = game.getEvents();
            Collections.sort(events, new Comparator<Event>() {
                @Override
                public int compare(Event e1, Event e2) {
                    return e1.getTime().compareTo(e2.getTime());
                }
            });
            if (game.getStarted()) {
                SimpleDateFormat format = new SimpleDateFormat("HH:mm:ss");
                try {
                    Date time1 = format.parse(java.time.LocalTime.now().toString());
                    Date time2 = format.parse(game.getTime().toString());
                    time = time1.getTime() - time2.getTime();
                    time = (time / (60 * 1000)) + 60;
                } catch (Exception e) {
                    time = -1;
                }
            }
            model.addAttribute("game", game);
            model.addAttribute("homeGoals", this.gameService.getTeamGoals(game.getHomeTeam()));
            model.addAttribute("awayGoals", this.gameService.getTeamGoals(game.getAwayTeam()));
            model.addAttribute("events", events);
            model.addAttribute("gameTime", time);
            return "gameStatsUser";
        } else {
            return "redirect:/homepageUser";
        }
    }

    @GetMapping("/gameStatsUnsigned")
    public String gameStatsUnsigned(@RequestParam(name = "id", required = true) int id, Model model) {
        Optional<Game> op = this.gameService.getGameById(id);
        long time = -1;
        if (op.isPresent()) {
            Game game = op.get();
            List<Event> events = game.getEvents();
            Collections.sort(events, new Comparator<Event>() {
                @Override
                public int compare(Event e1, Event e2) {
                    return e1.getTime().compareTo(e2.getTime());
                }
            });
            if (game.getStarted()) {
                SimpleDateFormat format = new SimpleDateFormat("HH:mm:ss");
                try {
                    Date time1 = format.parse(java.time.LocalTime.now().toString());
                    Date time2 = format.parse(game.getTime().toString());
                    time = time1.getTime() - time2.getTime();
                    time = (time / (60 * 1000)) + 60;
                } catch (Exception e) {
                    time = -1;
                }
            }
            model.addAttribute("game", game);
            model.addAttribute("homeGoals", this.gameService.getTeamGoals(game.getHomeTeam()));
            model.addAttribute("awayGoals", this.gameService.getTeamGoals(game.getAwayTeam()));
            model.addAttribute("events", events);
            model.addAttribute("gameTime", time);
            return "gameStatsUnsigned";
        } else {
            return "redirect:/homepageUnsigned";
        }
    }

    @GetMapping("/error")
    public String error(Model model) {
        return "error";
    }

}
