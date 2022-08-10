#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <map>

#define MAX_RECRUITS 10

class Member
{
public:
    int id, value, recruitsCount;
    int recruits[MAX_RECRUITS];

    // Dummy constructor
    Member() {}

    // Piece constructor
    Member(int id, int r[MAX_RECRUITS], int value)
    {
        this->id = id;
        std::memcpy(recruits, r, sizeof(recruits));
        this->value = value;
        recruitsCount = CountRecruits();
    }

private:
    int CountRecruits()
    {
        int counter = 0;
        for (int i : recruits)
        {
            if (i != -1)
                counter++;
        }
        return counter;
    }
};

bool orderMembers(Member m1, Member m2)
{
    if (m1.recruitsCount != m2.recruitsCount)
        return (m1.recruitsCount > m2.recruitsCount);
    return (m1.value > m2.value);
}

void preProcessing(std::map<int, std::vector<int>> *links, int member_counter, std::vector<Member> *members)
{
    // Adds all the members (values) linked to another member (key) (3: [3, 4, 6, 7])
    for (int i = 0; i < member_counter; i++)
    {
        // i is connected to itself
        if (links->find(i) == links->end())
            links->insert(std::pair<int, std::vector<int>>({i}, {i}));
        else
            links->at(i).push_back(i);
        // adds connections from i to all its recruits
        for (int j = 0; j < members->at(i).recruitsCount; j++)
        {
            if (links->find(members->at(i).recruits[j]) == links->end())
                links->insert(std::pair<int, std::vector<int>>({members->at(i).recruits[j]}, {i}));
            else
                links->at(members->at(i).recruits[j]).push_back(i);
        }
    }
}

int main()
{
    // Faster I/O
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);

    // Reads cin line by line
    std::string line;
    std::vector<Member> members;
    int member_counter = 0;
    while (std::getline(std::cin, line))
    {
        if (line.compare("-1") == 0)
        {
            // Solve pyramid and reset structures for next pyramid

            std::map<int, std::vector<int>> links;
            preProcessing(&links, member_counter, &members);

            // print links
            /*for (int i = 0; i < member_counter; i++){
                std::cout << std::endl << i << " - ";
                if (links.find(i) == links.end())
                    continue;
                std::vector<int> values = links.find(i)->second;
                for (int j: values){
                    std::cout << j << " ";
                }
                std::cout << std::endl;
            }*/

            // print members
            /*for(int i = 0; i< member_counter; i++){
                 std::cout << std::endl << "end id: " << members.at(i).id << std::endl;
                for (int j = 0; j < MAX_RECRUITS; j++)
                    std::cout << members.at(i).recruits[j] << ", ";
                std::cout << std::endl << "end value: " << members.at(i).value << std::endl;
            }*/

            line.clear();
            members.clear();
            member_counter = 0;
            continue;
        }

        // End of input
        if (line.empty())
            break;

        int recruits[MAX_RECRUITS];
        for (int i = 0; i < MAX_RECRUITS; i++)
            recruits[i] = -1;
        size_t pos = 0;
        int aux;
        // Id
        pos = line.find(" ");
        int id = std::stoi(line.substr(0, pos));
        line.erase(0, pos + 1);
        // Recruited Id's
        while ((pos = line.find(" ")) != std::string::npos)
        {
            for (int i = 0; i < 10; i++)
            {
                if (recruits[i] == -1)
                {
                    recruits[i] = std::stoi(line.substr(0, pos));
                    break;
                }
            }
            line.erase(0, pos + 1);
        }
        // Initial value paid
        int value = std::stoi(line);

        // Creates Member and adds to vector of members
        Member *m = new Member(id, recruits, value);
        members.push_back(*m);
        member_counter++;
        line.clear();
    }
    return 0;
}