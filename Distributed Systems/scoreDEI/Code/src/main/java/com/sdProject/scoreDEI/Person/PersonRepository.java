package com.sdProject.scoreDEI.Person;

import java.util.Optional;

import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.CrudRepository;
import org.springframework.data.repository.query.Param;

public interface PersonRepository extends CrudRepository<Person, Integer> {

    @Query("SELECT p FROM Person p WHERE p.email=:email")
    Optional<Person> authenticate(@Param("email") String email);

}
