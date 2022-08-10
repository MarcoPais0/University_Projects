package com.sdProject.scoreDEI.Team;

import java.text.SimpleDateFormat;
import java.util.Optional;

import org.json.JSONArray;
import org.json.JSONObject;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.ModelAttribute;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.ui.Model;

import com.mashape.unirest.http.HttpResponse;
import com.mashape.unirest.http.JsonNode;
import com.mashape.unirest.http.Unirest;
import com.sdProject.scoreDEI.Player.Player;
import com.sdProject.scoreDEI.Player.PlayerService;

@Controller
public class TeamController {
    @Autowired
    TeamService teamService;

    @Autowired
    PlayerService playerService;

    @GetMapping("/load")
    public String loadTeams(Model model) throws Exception {

        // Adiciona as teams
        Unirest.setTimeouts(0, 0);
        String host = "https://v3.football.api-sports.io/";
        String query = "/teams?league=2&season=2021";
        String APIKey = "b43ff12ff9935aac041a44e7554f52be";
        HttpResponse<JsonNode> response = Unirest.get(host + query)
                .header("x-rapidapi-key", APIKey)
                .header("x-rapidapi-host", "v3.football.api-sports.io")
                .asJson();
        JSONArray jo = (JSONArray) response.getBody().getObject().get("response");

        // Número de equipas adicionadas com os respetivos jogadores
        for (int i = 0; i < 4; i++) {
            // System.out.println(jo.get(i));

            JSONObject object = (JSONObject) jo.get(i);
            JSONObject team = (JSONObject) object.get("team");
            Team t = new Team(team.getString("name"), team.getString("logo"));
            this.teamService.addTeam(t);

            // Adiciona os players
            Unirest.setTimeouts(0, 0);
            query = "/players?season=2021&team=" + team.get("id");
            response = Unirest.get(host + query)
                    .header("x-rapidapi-key", APIKey)
                    .header("x-rapidapi-host", "v3.football.api-sports.io")
                    .asJson();
            JSONArray jj = (JSONArray) response.getBody().getObject().get("response");

            for (int j = 0; j < jj.length(); j++) {

                JSONObject obj = (JSONObject) jj.get(j);
                JSONObject player = (JSONObject) obj.get("player");
                JSONObject birth = (JSONObject) player.get("birth");
                String d;
                try {
                    d = birth.getString("date");
                } catch (Exception e) {
                    d = "0000-00-00";
                }
                java.util.Date utilDate = new SimpleDateFormat("yyyy-MM-dd").parse(d);
                java.sql.Date date = new java.sql.Date(utilDate.getTime());
                JSONArray statistics = (JSONArray) obj.get("statistics");
                JSONObject steam0 = (JSONObject) statistics.get(0);
                JSONObject games = (JSONObject) steam0.get("games");
                JSONObject goals = (JSONObject) steam0.get("goals");
                int g;
                try {
                    g = goals.getInt("total");
                } catch (Exception e) {
                    g = 0;
                }
                Player p = new Player(player.getString("name"), games.getString("position"), date, t, g, player.getString("photo"));
                this.playerService.addPlayer(p);
            }
        }

        return "redirect:/listTeams";
    }

    @GetMapping("/createTeam")
    public String createTeam(Model model) {
        model.addAttribute("team", new Team());
        return "createTeam";
    }

    @GetMapping("/manageTeam")
    public String managePlayer(@RequestParam(name = "id", required = true) int id, Model model) {
        Optional<Team> op = this.teamService.getTeam(id);
        if (op.isPresent()) {
            model.addAttribute("team", op.get());
            return "manageTeam";
        } else {
            return "redirect:/listTeams";
        }
    }

    @PostMapping("/saveTeam")
    public String saveTeam(@ModelAttribute Team team, Model model) {
        model.addAttribute("team", team);
        try {
            this.teamService.addTeam(team);
            return "redirect:/listTeams";
        }
        catch (Exception e) {
            return "redirect:/createTeam";
        }
    }

    @PostMapping("/deleteTeam")
    public String deleteTeam(@ModelAttribute Team team) {
        try {
            this.teamService.deleteTeam(team);
            return "redirect:/listTeams";            
        } catch (Exception e) {
            return "redirect:/listTeams";
        }
    }

    @GetMapping("/listTeams")
    public String listTeams(@RequestParam(name = "order", required = false) String order, Model model) {
        if (order == null)
            order = "DescendingTeams";

        if (order.equals("DescendingTeams")) {
            model.addAttribute("order", "Descending");
            model.addAttribute("teams", this.teamService.getTeamsDescending());
        } else if (order.equals("AscendingTeams")) {
            model.addAttribute("order", "Ascending");
            model.addAttribute("teams", this.teamService.getTeamsAscending());
        }

        else if (order.equals("DescendingGames")) {
            model.addAttribute("order", "Descending");
            model.addAttribute("teams", this.teamService.getGamesDescending());
        } else if (order.equals("AscendingGames")) {
            model.addAttribute("order", "Ascending");
            model.addAttribute("teams", this.teamService.getGamesAscending());
        }

        else if (order.equals("DescendingWins")) {
            model.addAttribute("order", "Descending");
            model.addAttribute("teams", this.teamService.getWinsDescending());
        } else if (order.equals("AscendingWins")) {
            model.addAttribute("order", "Ascending");
            model.addAttribute("teams", this.teamService.getWinsAscending());
        }

        else if (order.equals("DescendingLosses")) {
            model.addAttribute("order", "Descending");
            model.addAttribute("teams", this.teamService.getLossesDescending());
        } else if (order.equals("AscendingLosses")) {
            model.addAttribute("order", "Ascending");
            model.addAttribute("teams", this.teamService.getLossesAscending());
        }

        else if (order.equals("DescendingDefeats")) {
            model.addAttribute("order", "Descending");
            model.addAttribute("teams", this.teamService.getDefeatsDescending());
        } else if (order.equals("AscendingDefeats")) {
            model.addAttribute("order", "Ascending");
            model.addAttribute("teams", this.teamService.getDefeatsAscending());
        }

        return "listTeams";
    }

    @GetMapping("/statsTeams")
    public String statsTeams(@RequestParam(name = "order", required = false) String order, Model model) {
        if (order == null)
            order = "DescendingTeams";

        if (order.equals("DescendingTeams")) {
            model.addAttribute("order", "Descending");
            model.addAttribute("teams", this.teamService.getTeamsDescending());
        } else if (order.equals("AscendingTeams")) {
            model.addAttribute("order", "Ascending");
            model.addAttribute("teams", this.teamService.getTeamsAscending());
        }

        else if (order.equals("DescendingGames")) {
            model.addAttribute("order", "Descending");
            model.addAttribute("teams", this.teamService.getGamesDescending());
        } else if (order.equals("AscendingGames")) {
            model.addAttribute("order", "Ascending");
            model.addAttribute("teams", this.teamService.getGamesAscending());
        }

        else if (order.equals("DescendingWins")) {
            model.addAttribute("order", "Descending");
            model.addAttribute("teams", this.teamService.getWinsDescending());
        } else if (order.equals("AscendingWins")) {
            model.addAttribute("order", "Ascending");
            model.addAttribute("teams", this.teamService.getWinsAscending());
        }

        else if (order.equals("DescendingLosses")) {
            model.addAttribute("order", "Descending");
            model.addAttribute("teams", this.teamService.getLossesDescending());
        } else if (order.equals("AscendingLosses")) {
            model.addAttribute("order", "Ascending");
            model.addAttribute("teams", this.teamService.getLossesAscending());
        }

        else if (order.equals("DescendingDefeats")) {
            model.addAttribute("order", "Descending");
            model.addAttribute("teams", this.teamService.getDefeatsDescending());
        } else if (order.equals("AscendingDefeats")) {
            model.addAttribute("order", "Ascending");
            model.addAttribute("teams", this.teamService.getDefeatsAscending());
        }

        return "statsTeams";
    }

    @GetMapping("/statsTeamsUnsigned")
    public String statsTeamsUnsigned(@RequestParam(name = "order", required = false) String order, Model model) {
        if (order == null)
            order = "DescendingTeams";

        if (order.equals("DescendingTeams")) {
            model.addAttribute("order", "Descending");
            model.addAttribute("teams", this.teamService.getTeamsDescending());
        } else if (order.equals("AscendingTeams")) {
            model.addAttribute("order", "Ascending");
            model.addAttribute("teams", this.teamService.getTeamsAscending());
        }

        else if (order.equals("DescendingGames")) {
            model.addAttribute("order", "Descending");
            model.addAttribute("teams", this.teamService.getGamesDescending());
        } else if (order.equals("AscendingGames")) {
            model.addAttribute("order", "Ascending");
            model.addAttribute("teams", this.teamService.getGamesAscending());
        }

        else if (order.equals("DescendingWins")) {
            model.addAttribute("order", "Descending");
            model.addAttribute("teams", this.teamService.getWinsDescending());
        } else if (order.equals("AscendingWins")) {
            model.addAttribute("order", "Ascending");
            model.addAttribute("teams", this.teamService.getWinsAscending());
        }

        else if (order.equals("DescendingLosses")) {
            model.addAttribute("order", "Descending");
            model.addAttribute("teams", this.teamService.getLossesDescending());
        } else if (order.equals("AscendingLosses")) {
            model.addAttribute("order", "Ascending");
            model.addAttribute("teams", this.teamService.getLossesAscending());
        }

        else if (order.equals("DescendingDefeats")) {
            model.addAttribute("order", "Descending");
            model.addAttribute("teams", this.teamService.getDefeatsDescending());
        } else if (order.equals("AscendingDefeats")) {
            model.addAttribute("order", "Ascending");
            model.addAttribute("teams", this.teamService.getDefeatsAscending());
        }

        return "statsTeamsUnsigned";
    }
}
