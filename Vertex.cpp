#include "Vertex.h"

Vertex::Vertex(const string& str) {
    string word=cleanSpaces(str);
    checkWord(word);
    word=cleanAfterSpaces(word);
    name=word;
}

string Vertex::cleanAfterSpaces(string str) {
    int counter=0;
    for(string::const_iterator iterator=str.begin();iterator!=str.end();iterator++,counter++) {
        if (std::isspace(*iterator)) {
            return str.substr(0,counter );
        }
    }
    return str;
}

string Vertex::cleanSpaces(string str) {
    int counter=0;
    while(std::isspace(str[counter])){
        counter++;
    }
    string no_start_space=str.substr(counter);
    return no_start_space;

}

Vertex::Vertex(const Vertex &vertex):name(vertex.getName()) {}

string Vertex::getName() const {
        return name;
}

bool Vertex::checkLetter(const char letter){
    if(letter>='0'&&letter<='9'){return true;}
    if(letter>='a'&&letter<='z'){return true;}
    if(letter>='A'&&letter<='Z'){return true;}
    return (letter == '[') || (letter == ']') || (letter == ';');
}

void Vertex::checkWord(string str) {
    int counter=0;
    for(string::const_iterator iterator=str.begin();iterator!=str.end();iterator++){
        if(!checkLetter(*iterator)){
            //check if the letter is not [ or ] or ; or a number or a letter if so ERROR**
            if(std::isspace(*iterator)){//if we found a space  make sure it is not in the middle
                while(iterator!=str.end()){
                    if(!std::isspace(*iterator)){
                        throw Vertex::Error("Error: Illegal name" +str);
                    }
                    iterator++;
                }
                break;
            }
        }
        if(*iterator=='['){counter++;}
        if(*iterator==']'){counter--;}

        if(counter<0){
            throw Vertex::Error("Error: Illegal name checkword" +str);

        }
        if((*iterator==';')&&(counter==0)){
            throw Vertex::Error("Error: Illegal name checkword" +str);

        }
    }
    if(counter!=0){
        throw Vertex::Error("Error: Illegal name checkword" +str);

    }
}
Vertex& Vertex::operator=(const string& str) {
    string word=cleanSpaces(str);
    checkWord(word);
    name=word;
    return *this;
}

bool Vertex::operator==(const Vertex &v) const{
    if(name.compare(v.getName())==0){
        return true;
    }
    return false;
}

std::ostream& operator<<(std::ostream &os, const Vertex &v) {
    os<<v.getName();
    return os;
}

bool Vertex::operator<(const Vertex &v) const {
    return name<v.getName();
}