#ifndef FINALPROJECT_GRAPH_H
#define FINALPROJECT_GRAPH_H

#include <istream>
#include <string>
#include <set>

#include "Vertex.h"
#include "Edge.h"

using std::string;
using std::set;


class Graph{
    set<Vertex> Vertices;
    set<Edge> Edges;
    string name;

public:
    explicit Graph(const std::string& str,const std::string& name="default");
    Graph(const Graph& graph);
    ~Graph()=default;

    Graph& operator=(const Graph& graph);

    friend Graph operator+(const Graph& g1,const Graph& g2);
    friend Graph operator^(const Graph& g1,const Graph& g2);
    friend Graph operator-(const Graph& g1,const Graph& g2);
    friend Graph operator*(const Graph& g1,const Graph& g2);
    friend Graph operator!(const Graph& g1);//one variable operator

    set<Vertex> getVertices() const;
    set<Edge> getEdges() const;
    string getName() const;
    void changeName(const string& str);

    void fillVertices(string vertices);
    void fillEdges(string edges);
    void saveToFile(const string& filename)const;

    bool edgeIsValid(const Edge& edge)const;

    void print(std::ostream& os)const;
    static bool checkBrackets(const std::string& str);
    static bool wordIsEmpty(const std::string& str);
    static bool isSpace(const string& str);


        void Union(const Graph& g2);

    bool operator<(const Graph& g)const;
    bool operator==(const Graph& g)const;

};
#endif //FINALPROJECT_GRAPH_H
