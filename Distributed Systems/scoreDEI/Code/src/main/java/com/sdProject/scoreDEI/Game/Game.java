package com.sdProject.scoreDEI.Game;

import java.sql.Date;
import java.sql.Time;
import java.util.List;
import java.util.ArrayList;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.ManyToOne;
import javax.persistence.OneToMany;

import org.hibernate.annotations.OnDelete;
import org.hibernate.annotations.OnDeleteAction;

import com.sdProject.scoreDEI.Event.Event;
import com.sdProject.scoreDEI.Team.Team;

@Entity
public class Game {

    @Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    private int id;
    @ManyToOne
    @OnDelete(action = OnDeleteAction.CASCADE)
    private Team homeTeam;
    @ManyToOne
    @OnDelete(action = OnDeleteAction.CASCADE)
    private Team awayTeam;
    private String location;
    private Date date;
    private Time time;
    @OneToMany(mappedBy = "game")
    private List<Event> events;
    private boolean ended;
    private boolean interrupted;
    private boolean started;

    public Game() {
    }

    public Game(Team homeTeam, Team awayTeam, String location, Date date, Time time) {
        this.homeTeam = homeTeam;
        this.awayTeam = awayTeam;
        this.location = location;
        this.date = date;
        this.time = time;
        this.events = new ArrayList<>();
        ended = false;
        interrupted = false;
        started = false;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public Team getHomeTeam() {
        return homeTeam;
    }

    public void setHomeTeam(Team homeTeam) {
        this.homeTeam = homeTeam;
    }

    public Team getAwayTeam() {
        return awayTeam;
    }

    public void setAwayTeam(Team awayTeam) {
        this.awayTeam = awayTeam;
    }

    public String getLocation() {
        return location;
    }

    public void setLocation(String location) {
        this.location = location;
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

    public List<Event> getEvents() {
        return events;
    }

    public void setSquad(List<Event> events) {
        this.events = events;
    }

    public boolean getEnded() {
        return ended;
    }

    public void setEnded(boolean ended) {
        this.ended = ended;
    }

    public boolean getStarted() {
        return started;
    }

    public void setStarted(boolean started) {
        this.started = started;
    }

    public boolean getInterrupted() {
        return interrupted;
    }

    public void setInterrupted(boolean interrupted) {
        this.interrupted = interrupted;
    }
}
