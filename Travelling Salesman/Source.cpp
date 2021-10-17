#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <iostream>
#include "Node.h"
#include "NodeManager.h"

const int nodeCount = 30;
const int populationSize = 50;

const int windowWidth = 1200, windowHeight = 800;
const int displayWidth = 1200, displayHeight = 800;
const int padding = 50;

Route bestRoute;
std::vector<Node> nodes;
std::vector<Edge> edges;
std::vector<Route> routes;

int GetRandom(int min, int max) {
    //rng setup
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> gen(min, max);

    return gen(rng);
}

void Rank() {
    bool solved = false;

    while (!solved) {
        bool tempSolved = true;
        for (int i = 0; i < routes.size() - 1; ++i) {
            if (routes[i].GetDistance() > routes[i + 1].GetDistance()) {
                Route temp = routes[i];
                routes[i] = routes[i + 1];
                routes[i + 1] = temp;
                tempSolved = false;
            }
        }
        solved = tempSolved;
    }
}

void reset() {
    nodes.clear(); edges.clear(); routes.clear();
    
    //generate nodes
    for (int i = 0; i < nodeCount; ++i) {
        Node n = Node(sf::Vector2f(GetRandom(padding, displayWidth - padding), GetRandom(padding, displayHeight - padding)), i);
        nodes.push_back(n);
    }

    //generate edges
    for (int i = 0; i < nodes.size(); ++i) {
        for (int j = 0; j < nodes.size(); ++j) {
            bool exists = false;
            for (int e = 0; e < edges.size(); ++e) {

                if (edges[e].DoesConnect(nodes[i], nodes[j])) exists = true;
            }
            if (!exists) {
                Edge edge = Edge(nodes[i], nodes[j]);
                edges.push_back(edge);
            }
        }
    }

    //setup routes
    for (int i = 0; i < populationSize; ++i) {
        Route r = Route();
        std::vector<Node> nCopy = nodes;

        int idx = GetRandom(0, nCopy.size() - 1);
        r.AddNode(nCopy[idx]);
        Node first = nCopy[idx];
        nCopy.erase(nCopy.begin() + idx);

        while (nCopy.size() > 0) {
            idx = GetRandom(0, nCopy.size() - 1);
            r.AddNode(nCopy[idx]);
            nCopy.erase(nCopy.begin() + idx);
        }

        routes.push_back(r);
    }
    Rank();
    bestRoute = routes[0];
}

//Genetic Algorith solution
void SolveWithGA() {
    //get top 10%;
    std::vector<Route> topRoutes(routes.begin(), routes.begin() + (populationSize / 10));

    //create new generation using parents
    routes.clear();
    for (int i = 0; i < populationSize; ++i) {
        int id1 = GetRandom(0, topRoutes.size() - 1);
        int id2 = id1;
        while (id1 == id2)
            id2 = GetRandom(0, topRoutes.size() - 1);

        std::vector<Node> nodes1 = topRoutes[id1].GetNodes();
        std::vector<Node> nodes2 = topRoutes[id2].GetNodes();

        int idx1 = GetRandom(0, nodes1.size() - 1);
        int idx2 = idx1;
        while (idx2 == idx1)
            idx2 = GetRandom(0, nodes1.size() - 1);

        std::vector<Node> result(nodes1.size());
        if (idx1 > idx2) {
            int temp = idx1;
            idx1 = idx2;
            idx2 = temp;
        }

        //copy first section
        for (int i = idx1; i <= idx2; ++i) {
            result[i] = nodes1[i];
        }
        //fill with remaining
        for (int i = 0; i < result.size(); ++i) {
            if (i < idx1 || i > idx2) {
                bool found = false;
                int n = 0;
                while (!found && n < nodes2.size()) {
                    if (std::find(result.begin(), result.end(), nodes2[n]) == result.end()) {
                        result[i] = nodes2[n];
                        found = true;
                    }
                    n++;
                }
            }
        }

        routes.emplace_back(result);
    }

    //mutate
    for (int i = 0; i < routes.size(); ++i) {
        if (GetRandom(0, 1000) < 500) { //0.5%
            std::vector<Node> n = routes[i].GetNodes();
            int idx1 = GetRandom(0, n.size() - 1);
            int idx2 = idx1;
            while (idx1 == idx2)
                idx2 = GetRandom(0, n.size() - 1);

            Node temp = n[idx1];
            n[idx1] = n[idx2];
            n[idx2] = temp;
            routes[i].SetNodes(n);           
        }
    }

    Rank();
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Travelling Salesman Problem");        
    reset();           
    bool started = false;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            
            if (event.type == sf::Event::KeyPressed) {    
                if (event.key.code == sf::Keyboard::R) {

                    reset();                    
                    std::cout << "Points reset. Route Distance: " << bestRoute.GetDistance() <<"\n";
                }
            }            
        }        
        
        if (started) SolveWithGA();

        window.clear(sf::Color::White);

        if (bestRoute.GetDistance() > routes[0].GetDistance())bestRoute = routes[0];
        bestRoute.BuildRoute();
        std::cout << "\nbest distance: " << bestRoute.GetDistance();
       
        for (auto& e : edges) { e.Draw(window); }
        bestRoute.Draw(window);
        for (auto& n : nodes) { n.Draw(window); }        
       
        window.display();
        if(!started) started = true;
    }

    return 0;
}