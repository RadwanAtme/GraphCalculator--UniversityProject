#include <string>

#include "Edge.h"
#include "Vertex.h"

using std::string;

Edge::Edge(const string& word):source("s"),destination("d") {
    string str=cleanSpaces(word);
    checkBrackets(str);

    int begin=str.find('<');
    int mid=str.find(',');
    int end=str.find('>');

    int steps= begin==0? mid+1:mid;
    source=str.substr(begin+1,steps-2);
    destination=str.substr(mid+1,end-mid-1);
    if(source==destination){
        throw Vertex::Error("Error: Self loop not allowed"+word);
    }
}
string Edge::cleanSpaces(const string &str) {
    int end=str.find('>');
    string word=str.substr(end+1);
    for(string::const_iterator iterator=word.begin();iterator!=word.end();++iterator){
        if(!std::isspace(*iterator)){
            throw Vertex::Error("Error: Illegal name"+str);
        }
        continue;
    }
    return str.substr(0,end);
}
void Edge::checkBrackets(const string &str) {
    bool first_letter=true;
    signed int index=0;
    int counter=0;
    for(string::const_iterator iterator=str.begin();iterator!=str.end();iterator++,index++){
        //we found a letter at the begining of the word insted of '<',ERROR*
        if(std::isspace(*iterator)){continue;}
        if((*iterator!='<')&&(first_letter==true)){
            throw Vertex::Error("Error: Illegal name"+str);
        }
        if((*iterator=='<')&&(first_letter==false)){
            //we found a '<' that is not at the beginning,ERROR**
            throw Vertex::Error("Error: Illegal name"+str);

        }
        if((*iterator=='<')&&(first_letter==true)){
            first_letter=false;
            continue;
        }
        if(*iterator==','){counter++;}
        if((*iterator == '>')&&(index!=(signed)(str.length()-1))){
            //we found a '>' that is not at the end,ERROR
            throw Vertex::Error("Error: Illegal name "+str);
//add if we dont have >
        }
    }
    if(counter!=1){
        //there is no ',' or there is more than one ',' ,ERROR
        throw Vertex::Error("Error: Illegal name "+str);

    }
}

Edge::Edge(const Edge &edge):source(edge.getSource()),destination(edge.getDestination()) {}

Edge::Edge(const Vertex &v1, const Vertex &v2):source(v1),destination(v2) {}

Vertex Edge::getSource() const {
        return source;
}

Vertex Edge::getDestination() const {
    return destination;
}

bool Edge::operator==(const Edge &edge)const {
    return ((destination==edge.getDestination()) && (source==edge.getSource()));
}

std::ostream& operator<<(std::ostream& os,const Edge& e){
    string space=" ";
    os<<e.getSource()<<space<<e.getDestination();
    return os;
}

bool Edge::operator<(const Edge &e) const {
    if (getSource()<e.getSource()){
        return true;
    }
    if(getSource()==e.getSource()){
        return getDestination()<e.getDestination();
    }
    return false;
}