package com.sdProject.scoreDEI.Person;

import java.util.Optional;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.ModelAttribute;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.ui.Model;

@Controller
public class PersonController {
    @Autowired
    PersonService personService;

    @RequestMapping(value = { "/", "", "/index" }, method = RequestMethod.GET)
    public String redirect() {
        return "redirect:/login";
    }

    @GetMapping("/login")
    public String login() {
        return "login";
    }

    @PostMapping("/login")
    public String login(@RequestParam String email, @RequestParam String password, Model model) {
        Optional<Person> op = this.personService.authenticatePerson(email, password);

        if (op.isPresent()) {
            if (op.get().getPassword().equals(password))
                if (op.get().getRole() == true)
                    return "redirect:/homepage";
                else
                    return "redirect:/homepageUser";
        }

        return "login";
    }

    @GetMapping("/createUser")
    public String createUser(Model model) {
        model.addAttribute("user", new Person());
        return "createUser";
    }

    @GetMapping("/manageUser")
    public String manageUser(@RequestParam(name = "id", required = true) int id, Model model) {
        Optional<Person> op = this.personService.getPersonById(id);
        if (op.isPresent()) {
            model.addAttribute("user", op.get());
            return "manageUser";
        } else {
            return "redirect:/listUsers";
        }
    }

    @PostMapping("/saveUser")
    public String saveUser(@ModelAttribute Person person) {
        try {
            this.personService.addPerson(person);
            return "redirect:/listUsers";
        }
        catch (Exception e) {
            return "redirect:/createUser";
        }
    }

    @PostMapping("/deleteUser")
    public String deleteUser(@ModelAttribute Person person) {
        try {
            this.personService.deletePerson(person);
            return "redirect:/listUsers";    
        } catch (Exception e) {
            return "redirect:/listUsers";
        }        
    }

    @GetMapping("/listUsers")
    public String listUser(Model model) {
        model.addAttribute("users", this.personService.getAllUsers());
        return "listUsers";
    }
}
