
#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <list>
#include <string.h>
#include <cmath>
using namespace std;
#define INFI 10000000

// Split string
void splitString(const string& str, char delimiter, vector<string>& tokens) {
    string token;
    size_t start = 0;
    size_t end = str.find(delimiter);

    while (end != string::npos) {
        token = str.substr(start, end - start);
        tokens.push_back(token);
        start = end + 1;

        // Find the next non-delimiter character
        while (start < str.length() && str[start] == delimiter) {
            start++;
        }

        end = str.find(delimiter, start);
    }

    // Handle the last token
    if (start < str.length()) {
        token = str.substr(start);
        tokens.push_back(token);
    }
}

float round_num(float var) {
    
    //1000/3 = 333.33
    // 37.66666 * 100 =3766.66
    // 3766.66 + .5 =3767.16    for rounding off value
    // then type cast to int so value is 3767
    // then divided by 100 so the value converted into 37.67
    float value = (int)(var * 100 + .5);
    return (float)value / 100;
}

class Graph {
private:
    struct Node {
        string name;
        string nodeType;
        int idx;
        //INSERT_EDGE <name> <type> <index>
        Node(string name, string nodeType, int idx) {
            this->name = name;
            this->nodeType = nodeType;
            this->idx = idx;
        }
    };
    //INSERT_EDGE <id_number1> <id_number2> <distance> <messageToken>
    struct Edge {
        int node1;
        int node2;
        double distance;
        int messageToken;

        Edge(int node1, int node2, double distance, int messageToken) {
            this->node1 = node1;
            this->node2 = node2;
            this->distance = distance;
            this->messageToken = messageToken;
        }
    };

    vector<Node> nodes; //store nodes
    vector<list<Edge>> adjacencyList; //vectore of vector lists,store edges
    vector<double> distance; //store shortest distance
    vector<int> previous; //reconstruct shortest path

    int sourceNode; //index of source node
    int destinationNode;// index of HQ


public:
    void insertNode(const string& name, const string& nodeType, int idx) {
        Node newNode(name, nodeType, idx);
        nodes.push_back(newNode);
        adjacencyList.resize(nodes.size());
        //Check if it SOURCE or HQ
        if (nodeType == "SOURCE") {// Clear the tokens vector for the next iteration
            sourceNode = idx - 1;
        } else if (nodeType == "HQ") {
            destinationNode = idx - 1;
        }
    }
    void insertEdge(int node1, int node2, double distance, int messageToken) {
        // Check if the connection is allowed
        bool isAllowed = true;
    
        // Check for unallowed connections
        if(nodes[node1 - 1].nodeType == "SOURCE" && nodes[node2 - 1].nodeType == "SPY"){
            isAllowed = false;
        }else if(nodes[node1 - 1].nodeType == "SPY" && nodes[node2 - 1].nodeType == "SPY"){
            isAllowed = false;
        }else if(nodes[node1 - 1].nodeType == "CIV" && nodes[node2 - 1].nodeType == "HQ"){
            isAllowed = false;
        }else if(nodes[node1 - 1].nodeType == "HQ" && nodes[node2 - 1].nodeType == "CIV"){
            isAllowed = false;
        }else if(nodes[node1 - 1].nodeType == "SPY" && nodes[node2 - 1].nodeType == "SOURCE"){
            isAllowed = false;
        }
        //cout<<nodes[node1 - 1].nodeType<< nodes[node1 - 2].nodeType <<isAllowed<<endl;
    
        // Insert the edge only if it is allowed
        if (isAllowed) {
            Edge edge(node1 - 1, node2 - 1, distance, messageToken);
            adjacencyList[node1 - 1].push_back(edge);
    
            // Since it's an undirected graph, also insert the reverse edge
            Edge reverseEdge(node2 - 1, node1 - 1, distance, messageToken);
            adjacencyList[node2 - 1].push_back(reverseEdge);
        }
    }



void analyzeGraph() {
    // Dijkstra's algorithm to find the shortest path
    // Mark all vertices as unvisited
    distance.resize(nodes.size(), INFI);
    previous.resize(nodes.size(), -1);

    // Initialize distance
    distance[sourceNode] = 0;

    // Calculate the shortest path from the source vertex to all other vertices
    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq;
    pq.push({0, sourceNode});

    while (!pq.empty()) {
        int currNode = pq.top().second;
        pq.pop();

        for (const Edge& edge : adjacencyList[currNode]) {
            int neighbor = edge.node2;
            double edgeWeight = edge.distance;

            if (distance[currNode] + edgeWeight < distance[neighbor]) {
                distance[neighbor] = distance[currNode] + edgeWeight;
                previous[neighbor] = currNode;
                pq.push({distance[neighbor], neighbor});
            }
        }
    }

    // Determine the shortest path by considering the lexicographically smallest path
    int totalMessages = 0;
    int currNode = destinationNode;
    vector<vector<int>> paths;
    vector<int> path;
    while (previous[currNode] != -1) {
        int prevNode = previous[currNode];
        for (const Edge& edge : adjacencyList[prevNode]) {
            if (edge.node2 == currNode) {
                totalMessages += edge.messageToken;
                break;
            }
        }
        path.push_back(currNode);
        currNode = prevNode;
    }
    path.push_back(sourceNode);
    paths.push_back(path);

    // Iterate over the paths and select the lexicographically smallest path based on node names
    vector<int> selectedPath = paths[0];
    for (int i = 1; i < paths.size(); ++i) {
        int j = 0;
        while (j < selectedPath.size() && j < paths[i].size()) {
            if (nodes[paths[i][j]].name < nodes[selectedPath[j]].name) {
                selectedPath = paths[i];
                break;
            } else if (nodes[paths[i][j]].name > nodes[selectedPath[j]].name) {
                break;
            }
            ++j;
        }
        if (j == paths[i].size() && selectedPath.size() > paths[i].size()) {
            selectedPath = paths[i];
        }
    }

    // Display the selected path
    for (int i = selectedPath.size() - 1; i >= 0; --i) {
        cout << nodes[selectedPath[i]].name;
        if (i != 0) {
            cout << " -> ";
        }
    }
    cout << endl;
    cout << totalMessages << endl;
    cout << distance[destinationNode] << endl;
}




};


int main() {
    string data;
    vector<string> tokens;
    int idx = 0; //start at index 1
    Graph graph; // Create an instance of the Graph class
    queue<vector<string>> insertNodeQueue;
    queue<vector<string>> insertEdgeQueue;

    while (getline(cin, data)) {
        splitString(data, ' ', tokens);
        if (tokens[0] == "INSERT") {
            if (tokens[1] == "SOURCE" || tokens[1] == "HQ") {
                insertNodeQueue.push(tokens);
            } else {
                insertNodeQueue.push(tokens);
            }
        } else if (tokens[0] == "INSERT_EDGE") {
            insertEdgeQueue.push(tokens);
        } else if (tokens[0] == "ANALYZE") {
            break;
        }

        tokens.clear(); // Clear the tokens vector for the next iteration
    }
    // Process insertNodeQueue
    while (!insertNodeQueue.empty()) {
        vector<string> nodeData = insertNodeQueue.front();
        insertNodeQueue.pop();
        /*
        for (const string& t : nodeData) {
            cout << t << " ";
        }
        cout << endl;
        */
        string name = nodeData[2];
        string nodeType = nodeData[1];
        idx++;
        //cout << "Inserting node: " << name << " " << nodeType << " " << idx << endl;
        graph.insertNode(name, nodeType, idx);
    }
    // Process insertEdgeQueue
    while (!insertEdgeQueue.empty()) {
        vector<string> edgeData = insertEdgeQueue.front();
        insertEdgeQueue.pop();
        /*
        for (const string& t : edgeData) {
            cout << t << " ";
        }
        cout << endl;
        */
        int node1 = stoi(edgeData[1]);
        int node2 = stoi(edgeData[2]);
        double distance;
        int messageToken = 0;

        if (edgeData.size() > 3) {
            messageToken = stoi(edgeData[3]);
            distance = round_num(1000.0 / messageToken);
        } else {
            messageToken = 0;
            distance = INFI;
        }

        //cout << "Inserting edge: " << node1 << " " << node2 << " " << distance << " " << messageToken << endl;
        graph.insertEdge(node1, node2, distance, messageToken);
    }



    graph.analyzeGraph();

    return 0;
}
