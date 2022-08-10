package com.sdProject.scoreDEI.Player;

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
public class PlayerController {
    @Autowired
    PlayerService playerService;

    @Autowired
    TeamService teamService;

    @GetMapping("/createPlayer")
    public String createPlayer(Model model) {
        model.addAttribute("player", new Player());
        model.addAttribute("allTeams", this.teamService.getAllTeams());
        return "createPlayer";
    }

    @GetMapping("/managePlayer")
    public String managePlayer(@RequestParam(name = "id", required = true) int id, Model model) {
        Optional<Player> op = this.playerService.getPlayer(id);
        if (op.isPresent()) {
            model.addAttribute("player", op.get());
            model.addAttribute("allTeams", this.teamService.getAllTeams());
            return "managePlayer";
        } else {
            return "redirect:/listPlayers";
        }
    }

    @PostMapping("/savePlayer")
    public String savePlayer(@ModelAttribute Player player) {
        try {
            this.playerService.addPlayer(player);
            return "redirect:/listPlayers";   
        } catch (Exception e) {
            return "redirect:/createPlayer";
        }
    }

    @PostMapping("/deletePlayer")
    public String deletePlayer(@ModelAttribute Player player) {
        try {
            this.playerService.deletePlayer(player);
            return "redirect:/listPlayers";            
        } catch (Exception e) {
            return "redirect:/listPlayers";
        }
    }

    @GetMapping("/listPlayers")
    public String listPlayer(@RequestParam(name = "order", required = false) String order, Model model) {
        if (order == null)
            order = "AscendingPlayers";

        if (order.equals("DescendingPlayers")) {
            model.addAttribute("order", "Descending");
            model.addAttribute("players", this.playerService.getPlayersDescending());
        } else if (order.equals("AscendingPlayers")) {
            model.addAttribute("order", "Ascending");
            model.addAttribute("players", this.playerService.getPlayersAscending());

        } else if (order.equals("DescendingGoals")) {
            model.addAttribute("order", "Descending");
            model.addAttribute("players", this.playerService.getGoalsDescending());
        } else if (order.equals("AscendingGoals")) {
            model.addAttribute("order", "Ascending");
            model.addAttribute("players", this.playerService.getGoalsAscending());
        }
        return "listPlayers";
    }

    @GetMapping("/statsPlayers")
    public String statsPlayers(@RequestParam(name = "order", required = false) String order, Model model) {
        if (order == null)
            order = "AscendingPlayers";

        if (order.equals("DescendingPlayers")) {
            model.addAttribute("order", "Descending");
            model.addAttribute("players", this.playerService.getPlayersDescending());
        } else if (order.equals("AscendingPlayers")) {
            model.addAttribute("order", "Ascending");
            model.addAttribute("players", this.playerService.getPlayersAscending());

        } else if (order.equals("DescendingGoals")) {
            model.addAttribute("order", "Descending");
            model.addAttribute("players", this.playerService.getGoalsDescending());
        } else if (order.equals("AscendingGoals")) {
            model.addAttribute("order", "Ascending");
            model.addAttribute("players", this.playerService.getGoalsAscending());
        }
        return "statsPlayers";
    }

    @GetMapping("/statsPlayersUnsigned")
    public String statsPlayersUnsigned(@RequestParam(name = "order", required = false) String order, Model model) {
        if (order == null)
            order = "AscendingPlayers";

        if (order.equals("DescendingPlayers")) {
            model.addAttribute("order", "Descending");
            model.addAttribute("players", this.playerService.getPlayersDescending());
        } else if (order.equals("AscendingPlayers")) {
            model.addAttribute("order", "Ascending");
            model.addAttribute("players", this.playerService.getPlayersAscending());

        } else if (order.equals("DescendingGoals")) {
            model.addAttribute("order", "Descending");
            model.addAttribute("players", this.playerService.getGoalsDescending());
        } else if (order.equals("AscendingGoals")) {
            model.addAttribute("order", "Ascending");
            model.addAttribute("players", this.playerService.getGoalsAscending());
        }
        return "statsPlayersUnsigned";
    }
}
