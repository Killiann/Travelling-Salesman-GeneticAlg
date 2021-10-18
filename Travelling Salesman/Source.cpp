#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <iostream>
#include "Node.h"
#include "NodeManager.h"

const int nodeCount = 25;
const int populationSize = 50;

const int windowWidth = 1200, windowHeight = 800;
const int displayWidth = 800, displayHeight = 600;
const int padding = 50;
const float radius = 250;

std::random_device dev;
std::mt19937 rng(dev());

template<typename T>
void Swap(T& x, T& y) { T temp = y; y = x; x = temp; }

int GetRandom(int min, int max) {    
    std::uniform_int_distribution<std::mt19937::result_type> gen(min, max);
    return gen(rng);
}

void GenerateRandomNodes(std::vector<Node>& nodes) {
    nodes.clear();
    //generate nodes
    for (int i = 0; i < nodeCount; ++i) {
        Node n = Node(sf::Vector2f(GetRandom(padding, displayWidth - padding), GetRandom(padding, displayHeight - padding)), i);
        nodes.push_back(n);
    }
}

void GenerateCircle(std::vector<Node>& nodes) {
    nodes.clear();
    float angle = 0.f;
    float division = (2 * M_PI) / ((float)nodeCount);
    for (int i = 0; i < nodeCount; ++i) {
        int x = (radius * cos(angle)) + (displayWidth / 2);
        int y = (radius * sin(angle)) + (displayHeight / 2);

        Node n = Node(sf::Vector2f(x, y), i);
        nodes.push_back(n);
        angle += division;
    }
}

void Reset(std::vector<Node> &nodes, std::vector<Edge> &edges, std::vector<Route> &routes, Route &bestRoute) {
    edges.clear(); routes.clear();        

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
    std::sort(routes.begin(), routes.end());    
    bestRoute = routes[0];
}

//Genetic Algorith solution
void SolveWithGA(std::vector<Route> &routes) {
    //get top 10%;
    std::vector<Route> topRoutes(routes.begin(), routes.begin() + (populationSize / 10));

    //create new generation using parents
    routes.clear();
    for (int i = 0; i < populationSize; ++i) {
        //pick two random parents + fetch nodes
        int id1 = GetRandom(0, topRoutes.size() - 1);
        int id2 = id1;
        while (id1 == id2)
            id2 = GetRandom(0, topRoutes.size() - 1);

        std::vector<Node> nodes1 = topRoutes[id1].GetNodes();
        std::vector<Node> nodes2 = topRoutes[id2].GetNodes();;
        std::vector<Node> result(nodes1.size());

        //pick random range to take from nodes1
        id1 = GetRandom(0, nodes1.size() - 1);
        id2 = id1;
        while (id2 == id1)
            id2 = GetRandom(0, nodes1.size() - 1);
        
        if (id1 > id2) 
            Swap(id1, id2);

        //copy first section into result
        for (int i = id1; i <= id2; ++i)  result[i] = nodes1[i];
        
        //fill with remaining
        for (int x = 0; x < result.size(); ++x) {
            if (x < id1 || x > id2) {
                bool found = false;
                int n = 0;
                while (!found && n < nodes2.size()) {
                    if (std::find(result.begin(), result.end(), nodes2[n]) == result.end()) {
                        result[x] = nodes2[n];
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
        if (GetRandom(0, 1000) < 500) { // 50%
            std::vector<Node> n = routes[i].GetNodes();
            int id1 = GetRandom(0, n.size() - 1);
            int id2 = id1;
            while (id1 == id2)
                id2 = GetRandom(0, n.size() - 1);

            Swap(n[id1], n[id2]);
            routes[i].SetNodes(n);           
        }
    }

    std::sort(routes.begin(), routes.end());
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Travelling Salesman Problem");   

    static Route bestRoute, prevBest;
    static std::vector<Node> nodes;
    static std::vector<Edge> edges;
    static std::vector<Route> routes;

    GenerateCircle(nodes);
    Reset(nodes, edges, routes, bestRoute);
    prevBest = bestRoute;

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
                    //reset
                    GenerateRandomNodes(nodes);
                    Reset(nodes, edges, routes, bestRoute);
                    prevBest = bestRoute;
                    std::cout << "\n\nPoints reset. Current best distance: " << bestRoute.GetDistance() <<"\n";
                }
            }            
        }        
        
        //solve
        if (started) SolveWithGA(routes);
        if (bestRoute.GetDistance() > routes[0].GetDistance())bestRoute = routes[0];
        bestRoute.BuildRoute();
        if (bestRoute.GetDistance() < prevBest.GetDistance()) {
            std::cout << "\nbest distance: " << bestRoute.GetDistance();
            prevBest = bestRoute;
        }
       
        //draw
        window.clear(sf::Color::White);
        for (auto& e : edges) { e.Draw(window); }
        bestRoute.Draw(window);
        for (auto& n : nodes) { n.Draw(window); }               
        window.display();

        if(!started) started = true;
    }

    return 0;
}