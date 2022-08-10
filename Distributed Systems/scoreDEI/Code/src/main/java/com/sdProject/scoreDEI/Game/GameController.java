package com.sdProject.scoreDEI.Game;

import org.springframework.stereotype.Controller;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.ModelAttribute;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.ui.Model;

import java.util.Optional;

import com.sdProject.scoreDEI.Team.TeamService;

@Controller
public class GameController {
    @Autowired
    GameService gameService;

    @Autowired
    TeamService teamService;

    @GetMapping("/createGame")
    public String createTeam(Model model) {
        model.addAttribute("game", new Game());
        model.addAttribute("allTeams", this.teamService.getAllTeams());
        return "createGame";
    }

    @GetMapping("/manageGame")
    public String manageGame(@RequestParam(name = "id", required = true) int id, Model model) {
        Optional<Game> op = this.gameService.getGame(id);
        if (op.isPresent()) {
            model.addAttribute("game", op.get());
            model.addAttribute("allTeams", this.teamService.getAllTeams());
            return "manageGame";
        } else {
            return "redirect:/listGames";
        }
    }

    @PostMapping("/saveGame")
    public String saveTeam(@ModelAttribute Game game, Model model) {
        model.addAttribute("game", game);
        try {
            this.gameService.addGame(game);
            return "redirect:/listGames";    
        } catch (Exception e) {
            return "redirect:/createGame";
        }
        
    }

    @PostMapping("/deleteGame")
    public String deleteGame(@ModelAttribute Game game) {
        try {
            this.gameService.deleteGame(game);
            return "redirect:/listGames";    
        } catch (Exception e) {
            return "redirect:/listGames";
        }
    }

    @GetMapping("/listGames")
    public String listPlayer(Model model) {
        model.addAttribute("games", this.gameService.getAllGames());
        return "listGames";
    }
}
