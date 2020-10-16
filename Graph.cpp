#include<iostream>

#include "Graph.h"
#include "Edge.h"
#include "Vertex.h"
#include <fstream>

typedef set<Vertex>::const_iterator Vertex_Iterator;
typedef set<Edge>::const_iterator Edge_Iterator;
typedef std::string::const_iterator Iterator;

Graph::Graph(const std::string &str,const std::string& name) {
    if (!checkBrackets(str)) {
        throw Vertex::Error("Error: Illegal name "+name);
    }

    if (!wordIsEmpty(str)) {
        int begin = str.find('{');
        int mid = str.find('|');
        int end = str.find('}');

        std::string vertices = str.substr(begin + 1, mid - 1);
        std::string edges = str.substr(mid + 1, end - mid - 1);

        fillVertices(vertices);
        fillEdges(edges);
        this->name=name;
    }
}

bool Graph::wordIsEmpty(const std::string &str) {
    for(Iterator iterator=str.begin();iterator!=str.end();++iterator){
        if((!std::isspace(*iterator))&&(*iterator!='{')&&(*iterator!='}')&&(*iterator!='|')){
            return false;
        }
    }
    return true;
}

Graph::Graph(const Graph &graph):Vertices(graph.getVertices()),Edges(graph.getEdges()) {
    name=graph.getName();
}


void Graph::fillVertices(string vertices) {
    if(Graph::isSpace(vertices)||vertices==""){
        return;
    }
    string command=vertices;
    while(vertices.length()!=0){
        signed int index=vertices.find(',');
        if(index==(signed int)string::npos||index==-1){
            Vertex v_end(vertices);
            this->Vertices.insert(v_end);
            break;
        }
        std::string sub=vertices.substr(0,index);
        vertices=vertices.substr(index+1);
        if(sub==""||isSpace(sub)){
            throw Vertex::Error("Error: Illegal command"+command);
        }
        Vertex v(sub);
        Vertices.insert(v);
    }

}
bool Graph::isSpace(const string& str){
    for(unsigned int index=0;index<str.length();index++){
        if(!std::isspace(str[index])){
            return false;
        }
    }
    return true;
}

void Graph::fillEdges(string edges){
    if(Graph::isSpace(edges)||edges==""){
        return;
    }
    while(edges.length()!=0){
        int index_temp=edges.find('>');//find the first '>'
        if(index_temp==-1){
            throw Vertex::Error("Error: Illegal name"+ edges);
        }
        std::string sub=edges.substr(index_temp+1);
        int index=index_temp + sub.find(',');
        if(sub==""){
            Edge e_end(edges);
            if(Vertices.find(e_end.getSource())==Vertices.end()||Vertices.find(e_end.getDestination())==Vertices.end()){
                throw Vertex::Error("Error: Illegal edge"+edges);
            }
            Edges.insert(e_end);
            break;
        }
        sub=edges.substr(0,index+1);
        edges=edges.substr(index+2);
        Edge e(sub);
        if(Vertices.find(e.getSource())==Vertices.end()||Vertices.find(e.getDestination())==Vertices.end()){
            throw Vertex::Error("Error: Illegal edge"+sub);
        }
        Edges.insert(e);
    }
}
Graph& Graph::operator=(const Graph &graph) {
    Edges=graph.getEdges();
    Vertices=graph.getVertices();
    name=graph.getName();
    return *this;
}

set<Vertex> Graph::getVertices()const  {
    return Vertices;
}

set<Edge> Graph::getEdges()const {
    return Edges;
}

Graph operator+(const Graph& g1,const Graph &g2){
    Graph g(g1);
    g.Vertices.insert(g2.Vertices.begin(),g2.Vertices.end());
    g.Edges.insert(g2.Edges.begin(),g2.Edges.end());
    return g;
}

Graph operator^(const Graph& g1,const Graph &g2){
    Graph g(g1);
    for(Vertex_Iterator iterator=g.Vertices.begin();iterator!=g.Vertices.end();iterator++){
        if(g2.Vertices.find(*iterator)==g2.Vertices.end()){
            g.Vertices.erase(iterator);
            iterator=g.Vertices.begin();
        }
    }
    for(Edge_Iterator iterator=g1.Edges.begin();iterator!=g1.Edges.end();iterator++){
        if(g2.Edges.find(*iterator)==g2.Edges.end()){
            g.Edges.erase(*iterator);
        }
    }
    return g;
}

bool Graph::edgeIsValid(const Edge &edge) const {
    bool source=Vertices.find(edge.getSource())!=Vertices.end();
    bool destenation=Vertices.find(edge.getDestination())!=Vertices.end();
    return source && destenation;
}

Graph operator-(const Graph& g1,const Graph &g2){
    Graph g("{|}");
    for(Vertex_Iterator iterator=g1.Vertices.begin();iterator!=g1.Vertices.end();iterator++){
        if(g2.Vertices.find(*iterator)==g2.Vertices.end()){
            g.Vertices.insert(*iterator);
        }
    }
    for(Edge_Iterator iterator=g1.Edges.begin();iterator!=g1.Edges.end();iterator++){
        if(g.edgeIsValid(*iterator)){
            g.Edges.insert(*iterator);
        }
    }
    return g;
}

Graph operator*(const Graph& g1,const Graph &g2){
    Graph g("{|}");
    for(Vertex_Iterator iterator1=g1.Vertices.begin();iterator1!=g1.Vertices.end();iterator1++){
        for(Vertex_Iterator iterator2=g2.Vertices.begin();iterator2!=g2.Vertices.end();iterator2++){
            Vertex v1(*iterator1);
            Vertex v2(*iterator2);
            Vertex v("["+v1.getName()+";"+v2.getName()+"]");
            g.Vertices.insert(v);
        }
    }
    for(Edge_Iterator iterator1=g1.Edges.begin();iterator1!=g1.Edges.end();iterator1++){
        for(Edge_Iterator iterator2=g2.Edges.begin();iterator2!=g2.Edges.end();iterator2++){
            Edge e1(*iterator1);
            Edge e2(*iterator2);
            Vertex v1("["+e1.getSource().getName()+";"+e2.getSource().getName()+"]");
            Vertex v2("["+e1.getDestination().getName()+";"+e2.getDestination().getName()+"]");
            Edge e(v1,v2);
            g.Edges.insert(e);
        }
    }
    return g;
}

Graph operator!(const Graph& g1){
    Graph g(g1);
    g.Edges.erase(g.Edges.begin(),g.Edges.end());
    for(Vertex_Iterator iterator=g.Vertices.begin();iterator!=g.Vertices.end();++iterator){
        Vertex_Iterator iterator_temp=g.Vertices.begin();
        while(iterator_temp!=g.Vertices.end()){
            if(iterator!=iterator_temp) {
                Edge e(*iterator, *iterator_temp);
                if (g1.Edges.find(e) == g1.Edges.end()) {
                    g.Edges.insert(e);
                }
            }
            iterator_temp++;
        }
    }
    return g;

}

void Graph::print(std::ostream& os)const{
    for(Vertex_Iterator iterator=Vertices.begin();iterator!=Vertices.end();++iterator){
        os<<*iterator<<std::endl;
    }
    os<<"$"<<std::endl;
    for(Edge_Iterator iterator=Edges.begin();iterator!=Edges.end();++iterator){
        os<<*iterator<<std::endl;
    }
}

bool Graph::checkBrackets(const std::string &str) {
    int counter1=0,counter2=0,counter3=0;
    for(Iterator iterator=str.begin();iterator!=str.end();++iterator){
        if(*iterator=='{'){counter1++;}
        if(*iterator=='|'){counter2++;}
        if(*iterator=='}'){counter3++;}
    }
    return counter1==1 && counter2==1 && counter3==1;
}

bool Graph::operator<(const Graph &g) const {
    return name<g.getName();
}

string Graph::getName() const {
    return name;
}

bool Graph::operator==(const Graph &g) const {
    return name==g.getName();
}

void Graph::changeName(const string& str) {
    name=str;
}



void Graph::saveToFile(const string& filename)const{
    std::ofstream outfile(filename, std::ios_base::binary);
    int num_vertices=Vertices.size();
    int num_edges=Edges.size();
    outfile.write((const char*)&num_vertices, sizeof(int));
    outfile.write((const char*)&num_edges, sizeof(int));

    for(std::set<Vertex>::const_iterator iterator=Vertices.begin();iterator!=Vertices.end();
        ++iterator){
        Vertex v=*iterator;
        string name=v.getName();
        int name_length=name.length();
        outfile.write((const char*)&name_length, sizeof(int));
        for(int char_index=0;char_index<name_length;++char_index){
            outfile.write((const char*)&name[char_index], 1);
        }
    }
    for(std::set<Edge>::const_iterator iterator=Edges.begin();iterator!=Edges.end();
        ++iterator){
        Edge e=*iterator;
        string source=e.getSource().getName();
        string destination=e.getDestination().getName();
        int source_length=source.length();
        outfile.write((const char*)&source_length, sizeof(int));
        for(int char_index=0;char_index<source_length;++char_index){
            outfile.write((const char*)&source[char_index], 1);
        }
        int destination_length=destination.length();
        outfile.write((const char*)&destination_length, sizeof(int));
        for(int char_index=0;char_index<destination_length;++char_index){
            outfile.write((const char*)&destination[char_index], 1);
        }
    }
}