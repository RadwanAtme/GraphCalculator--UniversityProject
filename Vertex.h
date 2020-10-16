
#ifndef FINALPROJECT_VERTEX_H
#define FINALPROJECT_VERTEX_H

#include <string>
using std::string;

class Vertex{
    string name;
public:

    explicit Vertex(const string& name);
    Vertex(const Vertex& vertex);
    ~Vertex() = default;


    Vertex& operator=(const string& str);
    bool operator==(const Vertex& vertex) const;
    bool operator<(const Vertex& v) const;

    friend std::ostream& operator<<(std::ostream& os,const Vertex& v);

    string getName() const;
    static bool checkLetter(const char letter);
    static string cleanSpaces(string str);
    static void checkWord(string );
    static string cleanAfterSpaces(string);


    class Error : public std::exception{
    string error;
public:
    explicit Error(string str):error(str){}
    string getError(){return error;}
};



};







#endif //FINALPROJECT_VERTEX_H
