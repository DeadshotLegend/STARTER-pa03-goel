#include "Graph.hpp"
using namespace std;

// NodeInfo -----------------------------------------------------------------------------------------------------------------------------------

NodeInfo::NodeInfo() {
    preActivationValue = 0;
    bias = 0;
    activationFunction = identity;
    activationDerivative = identity;
    activate();
    delta = 0;
}

NodeInfo::NodeInfo(string activationFunction, double value, double bias) {
    this->activationFunction = getActivationFunction(activationFunction);
    this->activationDerivative = getActivationDerivative(activationFunction);
    this->preActivationValue = value;
    this->activate();
    this->bias = bias;
    this->delta = 0;
}

double NodeInfo::activate() {
    postActivationValue = activationFunction(preActivationValue);
    return postActivationValue;
}

double NodeInfo::derive() {
    return activationDerivative(preActivationValue);
}

bool NodeInfo::operator==(const NodeInfo& other) {
    return (this->preActivationValue == other.preActivationValue) &&
           (this->postActivationValue == other.postActivationValue) &&
           (this->activationFunction == other.activationFunction) &&
           (this->bias == other.bias) &&
           (this->activationDerivative == other.activationDerivative);
}

std::ostream& operator<<(std::ostream& out, const NodeInfo& n) {
    out << "bias: " << n.bias
        << " preActivationValue: " << n.preActivationValue
        << " postActivationValue: " << n.postActivationValue
        << " activationFunction: " << getActivationIdentifier(n.activationFunction)
        << " activationDerivative: " << getActivationIdentifier(n.activationDerivative) << endl;
    return out;
}

// Connection -----------------------------------------------------------------------------------------------------------------------------------

Connection::Connection() {
    this->source = -1;
    this->dest = -1;
    this->weight = 0;
    this->delta = 0;
}

Connection::Connection(int source, int dest, double weight) {
    this->source = source;
    this->dest = dest;
    this->weight = weight;
    this->delta = 0;
}

bool Connection::operator<(const Connection& other) {
    return this->dest < other.dest;
}

bool Connection::operator==(const Connection& other) {
    return (this->dest == other.dest) &&
           (this->source == other.source) &&
           (this->weight == other.weight);
}

std::ostream& operator<<(std::ostream& out, const Connection& c) {
    out << "source: " << c.source
        << " dest: " << c.dest
        << " weight: " << c.weight << endl;
    return out;
}



// ------------------------ YOU DO NOT NEED TO MODIFY THE CODE ABOVE (but feel free to explore it) --------------------------------------




// Graph -----------------------------------------------------------------------------------------------------------------------------------

// STUDENT TODO: IMPLEMENT
void Graph::updateNode(int id, NodeInfo n) {
    if (id < 0 || id >= static_cast<int>(nodes.size())) {
        cout << "Attempting to update node with id: " << id << " but node does not exist" << endl;
        return;
    }

    if (nodes.at(id) != nullptr) {
        delete nodes.at(id);
    }

    nodes.at(id) = new NodeInfo(n);
}

// STUDENT TODO: IMPLEMENT
NodeInfo* Graph::getNode(int id) const {
    if (id < 0 || id >= static_cast<int>(nodes.size())) {
        return nullptr;
    }
    return nodes.at(id);
}

// STUDENT TODO: IMPLEMENT
void Graph::updateConnection(int v, int u, double w) {
    if (v < 0 || v >= static_cast<int>(nodes.size())) {
        cerr << "Attempting to update connection between " << v << " and " << u
             << " with weight " << w << " but " << v << " does not exist" << endl;
        exit(1);
    }
    if (u < 0 || u >= static_cast<int>(nodes.size())) {
        cerr << "Attempting to update connection between " << v << " and " << u
             << " with weight " << w << " but " << u << " does not exist" << endl;
        exit(1);
    }

    adjacencyList.at(v)[u] = Connection(v, u, w);
}

// STUDENT TODO: IMPLEMENT
void Graph::clear() {
    for (int i = 0; i < static_cast<int>(nodes.size()); i++) {
        if (nodes.at(i) != nullptr) {
            delete nodes.at(i);
            nodes.at(i) = nullptr;
        }
    }

    nodes.clear();
    adjacencyList.clear();
    size = 0;
}




// ------------------------ YOU DO NOT NEED TO MODIFY THE REMAINING CODE (but please feel free to explore it!) --------------------------------------





Graph::Graph() {
    this->size = 0;
}

Graph::Graph(int size) {
    resize(size);
}

Graph::Graph(const Graph& other) {
    this->size = other.size;
    this->adjacencyList = other.adjacencyList;
    for (int i = 0; i < static_cast<int>(other.nodes.size()); i++) {
        nodes.push_back(new NodeInfo(*other.nodes.at(i)));
    }
}

Graph& Graph::operator=(const Graph& other) {
    if (this == &other) {
        return *this;
    }
    clear();
    this->size = other.size;
    this->adjacencyList = other.adjacencyList;
    for (int i = 0; i < static_cast<int>(other.nodes.size()); i++) {
        nodes.push_back(new NodeInfo(*other.nodes.at(i)));
    }
    return *this;
}

Graph::~Graph() {
    clear();
}

AdjList& Graph::getAdjacencyList() {
    return adjacencyList;
}

ostream& operator<<(ostream& out, const Graph& g) {
    // output as dot format for graph visualization
    out << "digraph G {" << endl;
    for (int i = 0; i < static_cast<int>(g.adjacencyList.size()); i++) {
        for (auto j = g.adjacencyList.at(i).begin(); j != g.adjacencyList.at(i).end(); j++) {
            out << "\t" << i << " -> " << j->second.dest << "[label=\"" << j->second.weight << "\"]" << endl;
        }
    }
    out << "}" << endl;
    for (int i = 0; i < static_cast<int>(g.nodes.size()); i++) {
        string end = "\n";
        if (i == static_cast<int>(g.nodes.size()) - 1) {
            end = "";
        }
        out << "node " << i
            << ": (z=" << g.nodes.at(i)->preActivationValue << "\t"
            << ", a=" << g.nodes.at(i)->postActivationValue << "\t"
            << ", bias=" << g.nodes.at(i)->bias << "\t"
            << ", activation=" << getActivationIdentifier(g.nodes.at(i)->activationFunction) << ")" << end;
    }

    return out;
}

void Graph::resize(int size) {
    this->size = size;
    adjacencyList.clear();
    adjacencyList.resize(size);
    nodes.clear();
    for (int i = 0; i < size; i++) {
        nodes.push_back(nullptr);
    }
}

vector<NodeInfo*> Graph::getNodes() const {
    return nodes;
}