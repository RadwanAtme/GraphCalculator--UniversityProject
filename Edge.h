
#ifndef FINALPROJECT_EDGE_H
#define FINALPROJECT_EDGE_H

#include "Vertex.h"
#include <string>

class Edge{
    Vertex source;
    Vertex destination;
public:
    explicit Edge(const string& string);
    Edge(const Edge& edge);
    Edge(const Vertex& v1,const Vertex& v2);
    ~Edge() = default;
    Vertex getSource() const;
    Vertex getDestination() const;

    bool operator==(const Edge& edge) const;
    bool operator<(const Edge& e)const;

    static void checkBrackets(const string& str);
    static string cleanSpaces(const string& str);

    friend std::ostream& operator<<(std::ostream& os,const Edge& e);

};
#endif //FINALPROJECT_EDGE_H
