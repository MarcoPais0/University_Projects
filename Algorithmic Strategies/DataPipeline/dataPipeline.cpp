#include <bits/stdc++.h>
using namespace std;

// Prototype
vector<string> split(const string &str);

// Pipeline data to analyze
class Pipeline
{
public:
    int statistic, size;
    int initialNode, terminalNode;
    int initialNodesCount, terminalNodesCount = 0;
    int timeSum = 0;
    vector<int> *operations;
    vector<int> *reverseOperations;
    int *time;

    // Pipeline constructor
    Pipeline(int s)
    {
        size = s;
        operations = new vector<int>[s];
        reverseOperations = new vector<int>[s];
        time = new int[s];
    }

    // Delete pipeline class instance
    ~Pipeline()
    {
        delete[] operations;
        delete[] reverseOperations;
        delete[] time;
    }

    // Add connection (X, Y) and (Y, X) to adjancency list and reverse ajacency list, respectively
    void addConnection(int x, int y)
    {
        operations[x].push_back(y);
        reverseOperations[y].push_back(x);
    }

    // Add process time of operation
    void addTime(int index, int t)
    {
        time[index] = t;
    }

    // Add statistic to be measured
    void addInfo(int stat, int in, int inc, int sum)
    {
        statistic = stat;
        initialNode = in;
        initialNodesCount = inc;
        timeSum = sum;
    }

    // Checks if graph is valid and stores transversal order
    bool isValid(vector<bool> &path, vector<bool> &visited, vector<int> &order, int v, int &countNodes)
    {
        // Check if graph has only one terminal node
        if (terminalNodesCount > 1)
            return false;

        visited[v] = true;
        path[v] = true;
        for (int u : operations[v])
        {
            // Check if graph has cycles
            if (path[u])
                return false;

            // Check if node was visited
            if (!visited[u])
            {
                // Count terminal nodes and find terminal node
                if (operations[u].empty())
                {
                    terminalNode = u;
                    terminalNodesCount++;
                }

                // Count nodes
                countNodes++;

                isValid(path, visited, order, u, countNodes);
            }
        }
        path[v] = false;

        // Add node to computation order
        order.push_back(v);

        // Check if graph is connected and has only one terminal node
        if (countNodes != size - 1 || terminalNodesCount > 1)
            return false;

        return true;
    }

    // Finds feasible order of computation by ascending nodes
    bool bfs(vector<bool> &visited, vector<int> &order, int &countNodes)
    {
        // In-Degree vector to check if nodes can be processed
        vector<int> indegree(size, 0);

        // Reverse priority queue
        priority_queue<int, vector<int>, greater<int>> q;

        for (int i = 1; i < size; i++)
            for (int node : operations[i])
                // Iterate over all edges
                indegree[node]++;

        int cycle = 0;
        visited[initialNode] = true;
        q.push(initialNode);

        // Remove nodes until queue is empty
        while (q.size() > 0)
        {
            if (terminalNodesCount > 1)
                return false;

            int currentNode = q.top();
            order.push_back(currentNode);
            q.pop();

            for (int u : operations[currentNode])
            {
                // Remove edges
                indegree[u]--;
                if (indegree[u] == 0)
                    // Node has no more incoming edges
                    q.push(u);

                // Count terminal nodes and find terminal node
                if (!visited[u])
                {
                    if (operations[u].empty())
                    {
                        terminalNode = u;
                        terminalNodesCount++;
                    }
                    visited[u] = true;

                    // Count nodes
                    countNodes++;
                }
            }
            cycle++;
        }

        // Check if graph is connected and has only one terminal node
        if (countNodes != size - 1 || terminalNodesCount > 1 || cycle != size - 1)
            return false;

        return true;
    }

    // Tranverses graph until terminal node with depth first search
    void dfs(vector<bool> &visited, int v, int &countNodes, vector<int> *operations)
    {
        // Check v node as visited
        visited[v] = true;
        for (int u : operations[v])
        {
            // Check if node was visited
            if (!visited[u])
            {
                // Count nodes
                countNodes++;
                dfs(visited, u, countNodes, operations);
            }
        }
    }

    // Get statistical output
    void getStatistic()
    {
        // Visited nodes vector
        vector<bool> visited(size, false);

        // Vector to use for cycle checking
        vector<bool> path(size, false);

        // Vector with order of computation nodes
        vector<int> order;

        // Counter for nodes in graph tranversal
        int countNodes = 1;

        // Check if graph is valid and calculate statistics
        if (initialNodesCount == 1)
        {
            switch (statistic)
            {
            case 0:
                if (isValid(path, visited, order, initialNode, countNodes))
                    cout << "VALID" << endl;
                else
                    cout << "INVALID" << endl;
                break;

            case 1:
            {
                if (bfs(visited, order, countNodes))
                {
                    cout << timeSum << endl;
                    for (int node : order)
                        cout << node << endl;
                }
                else
                    cout << "INVALID" << endl;
                break;
            }

            case 2:
            {
                // Vector to calculate max computation cost to each node
                vector<int> distance(size, 1);

                // Initalize distance of initial node
                distance[initialNode] = time[initialNode];
                if (isValid(path, visited, order, initialNode, countNodes))
                {
                    // Goes through graph computation order
                    for (int i = size - 2; i >= 0; i--)
                    {
                        int v = order[i];
                        // Calculates computation cost between edges with algorithm similarr to bellman-ford
                        for (int u : operations[v])
                            if (distance[u] < distance[v] + time[u])
                                distance[u] = distance[v] + time[u];
                    }
                    cout << distance[terminalNode] << endl;
                }
                else
                    cout << "INVALID" << endl;
                break;
            }

            case 3:
            {
                // Vector with botleneck nodes
                vector<int> solution;

                // Check if its valid and get order
                if (isValid(path, visited, order, initialNode, countNodes))
                {
                    // Add initial node to botleneck
                    solution.push_back(initialNode);

                    // Loops through order vector from second processed node to penultimate node
                    for (int i = size - 3; i >= 1; i--)
                    {
                        int v = order[i];
                        int countNodes = 1;
                        fill(visited.begin(), visited.end(), false);

                        // Check graph transversal in order with dfs
                        dfs(visited, v, countNodes, operations);

                        // Check graph transversal in reverse with dfs
                        dfs(visited, v, countNodes, reverseOperations);

                        // Add node v to botleneck if it transverses all graph
                        if (countNodes == size - 1)
                            solution.push_back(v);
                    }

                    // Add terminal node to botleneck vector
                    solution.push_back(terminalNode);
                    for (int node : solution)
                        cout << node << endl;
                }
                else
                    cout << "INVALID" << endl;
                break;
            }
            }
        }
        else
            cout << "INVALID" << endl;
    }
};

int main()
{
    // Faster I/O
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    // Get input
    string operations;
    int nOperations, initialNode, initialNodesCount, timeSum = 0;
    vector<Pipeline *> cases;
    Pipeline *newPipeline;
    while (getline(cin, operations))
    {
        // Check if exists another case
        if (operations.empty())
            break;
        else
        {
            // Number of operations (added 1 because operations start at 1)
            nOperations = stoi(operations) + 1;

            // New pipeline to analyze
            newPipeline = new Pipeline(nOperations);
            initialNodesCount = 0;
            initialNode = 0;
            timeSum = 0;
        }

        // Get operations description
        for (int i = 1; i < nOperations; i++)
        {
            // New operation
            string operation;
            getline(cin, operation);
            vector<string> newOperation = split(operation);

            // Operation process time
            int processTime = stoi(newOperation[0]);

            // Add to total process time
            timeSum += processTime;

            newPipeline->addTime(i, processTime);

            // Number of dependencies
            int nDependencies = stoi(newOperation[1]);

            // Save initial node
            if (nDependencies == 0)
            {
                initialNode = i;
                initialNodesCount++;
            }

            // Get dependencies
            for (int j = 2; j < nDependencies + 2; j++)
                newPipeline->addConnection(stoi(newOperation[j]), i);
        }

        // Get statistic
        string stat;
        getline(cin, stat);
        int statistic = stoi(stat);

        // Add final info of the pipeline
        newPipeline->addInfo(statistic, initialNode, initialNodesCount, timeSum);

        // Add new pipeline data to cases
        cases.push_back(newPipeline);
    }

    // Run cases
    for (Pipeline *c : cases)
    {
        c->getStatistic();
        delete c;
    }

    return 0;
}

// Split string by spaces
vector<string> split(const string &str)
{
    vector<string> tokens;
    string::size_type start = 0;
    string::size_type end = 0;

    while ((end = str.find(" ", start)) != string::npos)
    {
        tokens.push_back(str.substr(start, end - start));
        start = end + 1;
    }
    tokens.push_back(str.substr(start));

    return tokens;
}
