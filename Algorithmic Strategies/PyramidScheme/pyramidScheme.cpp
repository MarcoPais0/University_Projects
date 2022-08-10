#include <iostream>
#include <string>
#include <vector>

#define MAX_IDS 100001

class Pyramid
{
public:
    int size;
    std::vector<int> *members;
    std::vector<int> *paid;

    // Dummy constructor
    Pyramid()
    {
        members = new std::vector<int>[MAX_IDS];
        paid = new std::vector<int>[MAX_IDS];
    }

    // Add Pyramid Size
    void addSize(int s)
    {
        size = s;
    }

    // Add Pyramid Size
    void addPaid(int index, int value)
    {
        paid[index].push_back(value);
    }

    // Add edge to adjacency matrix
    void addEdge(int x, int y)
    {
        members[x].push_back(y);
        members[y].push_back(x);
    }

    // Function to find minimum size of vertex cover and its maximum value paid
    void minSizeVertexCover()
    {
        std::vector<int> *dp = new std::vector<int>[size];
        std::vector<int> *value = new std::vector<int>[size];

        for (int i = 0; i < size; i++)
        {
            // 0 denotes not included in vertex cover
            dp[i].push_back(0);
            value[i].push_back(0);

            // 1 denotes included in vertex cover and paid[i][0] denotes i's value paid
            dp[i].push_back(1);
            if (paid[i].size() != 0)
                value[i].push_back(paid[i][0]);
        }

        dfs(dp, value, 0, -1);

        // Printing minimum size vertex cover and its maximum value paid
        if (dp[0][0] == dp[0][1])
            std::cout << dp[0][0] << " " << std::max(value[0][0], value[0][1]) << std::endl;
        else if (dp[0][1] < dp[0][0])
            std::cout << dp[0][1] << " " << value[0][1] << std::endl;
        else
            std::cout << dp[0][0] << " " << value[0][0] << std::endl;

        delete[] dp;
        delete[] value;
        delete[] paid;
        delete[] members;
    }

    void dfs(std::vector<int> *dp, std::vector<int> *value, int src, int par)
    {
        for (int child : members[src])
            if (child != par)
                dfs(dp, value, child, src);

        for (int child : members[src])
            if (child != par)
            {
                // Not including source in the vertex cover
                // Chooses child smallest path and adds its path size because its father cannot be part of the solution
                dp[src][0] += dp[child][1];
                value[src][0] += value[child][1];

                // Including source in the vertex cover
                // Chooses child smallest path, belonging or not to the solution, since its father is already included in the solution
                if (dp[child][1] < dp[child][0])
                {
                    dp[src][1] += dp[child][1];
                    value[src][1] += value[child][1];
                }
                else if (dp[child][1] > dp[child][0])
                {
                    dp[src][1] += dp[child][0];
                    value[src][1] += value[child][0];
                }

                // Choose max value path in case of equal path sizes
                else
                {
                    if (value[child][1] > value[child][0])
                    {
                        dp[src][1] += dp[child][1];
                        value[src][1] += value[child][1];
                    }
                    else
                    {
                        dp[src][1] += dp[child][0];
                        value[src][1] += value[child][0];
                    }
                }
            }
    }
};

int main()
{
    // Faster I/O
    std::ios_base::sync_with_stdio(0);
    std::cin.tie(0);

    // Reads cin line by line
    std::string line;
    std::vector<Pyramid *> cases;
    Pyramid *newScheme = new Pyramid();
    int nMember = 0;
    while (std::getline(std::cin, line))
    {
        // End of input
        if (line.empty())
            break;

        // Creates member and adds to vector of cases
        if (line.compare("-1") == 0)
        {
            // Add case
            newScheme->addSize(nMember + 1);
            cases.push_back(newScheme);

            // New pyramid scheme
            newScheme = new Pyramid();

            line.clear();
            nMember = 0;
            continue;
        }

        // Id
        size_t pos = 0;
        pos = line.find(" ");
        int id = stoi(line.substr(0, pos));
        if (id > nMember)
            nMember = id;
        line.erase(0, pos + 1);

        // Recruited Id's
        while ((pos = line.find(" ")) != std::string::npos)
        {
            int newId = stoi(line.substr(0, pos));
            newScheme->addEdge(id, newId);
            line.erase(0, pos + 1);
        }

        // Initial value paid by Id
        newScheme->addPaid(id, stoi(line));
        line.clear();
    }
    delete[] newScheme->members;
    delete[] newScheme->paid;
    delete newScheme;

    // Print results
    for (Pyramid *c : cases)
    {
        c->minSizeVertexCover();
        delete c;
    }
    cases.clear();

    return 0;
}
