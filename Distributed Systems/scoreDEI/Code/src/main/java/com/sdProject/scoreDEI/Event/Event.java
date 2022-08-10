package com.sdProject.scoreDEI.Event;

import java.sql.Date;
import java.sql.Time;

import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.ManyToOne;

import org.hibernate.annotations.OnDelete;
import org.hibernate.annotations.OnDeleteAction;

import com.sdProject.scoreDEI.Game.Game;
import com.sdProject.scoreDEI.Player.Player;
import com.sdProject.scoreDEI.Team.Team;

@Entity
public class Event {

    @Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    private int id;
    @ManyToOne
    @OnDelete(action = OnDeleteAction.CASCADE)
    private Game game;
    @ManyToOne
    @OnDelete(action = OnDeleteAction.CASCADE)
    private Team team;
    @ManyToOne
    @OnDelete(action = OnDeleteAction.CASCADE)
    private Player player;
    private String eventType;
    private Date date;
    private Time time;

    public Event() {
    }

    public Event(Game game, Team team, Player player, String eventType, Date date, Time time) {
        this.game = game;
        this.team = team;
        this.player = player;
        this.eventType = eventType;
        this.date = date;
        this.time = time;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public Game getGame() {
        return game;
    }

    public void setGame(Game game) {
        this.game = game;
    }

    public Team getTeam() {
        return team;
    }

    public void setTeam(Team team) {
        this.team = team;
    }

    public Player getPlayer() {
        return player;
    }

    public void setPlayer(Player player) {
        this.player = player;
    }

    public String getEventType() {
        return eventType;
    }

    public void setEventType(String eventType) {
        this.eventType = eventType;
    }

    public Date getDate() {
        return date;
    }

    public void setDate(Date date) {
        this.date = date;
    }

    public Time getTime() {
        return time;
    }

    public void setTime(Time time) {
        this.time = time;
    }

}
