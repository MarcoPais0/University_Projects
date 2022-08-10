package com.sdProject.scoreDEI.Person;

import java.util.List;
import java.util.Optional;
import java.util.ArrayList;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

@Service
public class PersonService {

    @Autowired
    private PersonRepository personRepository;

    public void addPerson(Person person) {
        personRepository.save(person);
    }

    public void deletePerson(Person person) {
        personRepository.delete(person);
    }

    public Optional<Person> authenticatePerson(String email, String password) {
        Optional<Person> p = personRepository.authenticate(email);

        return p;
    }

    public Optional<Person> getPersonById(int id) {
        return personRepository.findById(id);
    }

    public List<Person> getAllUsers() {
        List<Person> users = new ArrayList<>();
        personRepository.findAll().forEach(users::add);
        return users;
    }
}
