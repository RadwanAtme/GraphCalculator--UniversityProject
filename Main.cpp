#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

#include "Vertex.h"
#include "Graph.h"
#include "MainFunctions.h"

int main(int argc,char** argv) {
        std::vector<Graph> graphs;
        if (argc == 3) {
            std::ifstream input_stream;
            input_stream.open(argv[1]);
            if(input_stream.fail()){
                std::cout<<"Error: failed to open input text"<<std::endl;
            }
            std::ofstream output_stream(argv[2]);
            try {
                processInputBatchMode(graphs, input_stream, output_stream);
            } catch (Vertex::Error &error) {
                output_stream << error.getError() << std::endl;
            }

        } else {
            try {
                processInput(graphs);
            } catch (Vertex::Error &error) {
                std::cout << error.getError() << std::endl;
            }
        }
        return 0;
}

static void showMassege(){
    std::cout<<"Gcalc>";
    std::cout.flush();
}



static void processInput(std::vector<Graph>& graphs){
    string command;
    showMassege();
    std::getline(std::cin,command);
    while(command!="quit"||!std::cin.eof()) {
        if (!std::cin) {
            std::cout << "Error: error reading command" << std::endl;
            showMassege();
            std::getline(std::cin, command);
        } else {
            try {
                processCommand(command, graphs,std::cout);
            } catch (Vertex::Error &error) {
                std::cout << error.getError() << std::endl;
            }
            showMassege();
            //clean buffer if needed
            std::getline(std::cin,command);
        }
    }

}

static void processInputBatchMode(std::vector<Graph>& graphs,std::ifstream& input_stream,
                                  std::ofstream& output_stream){
    string command;
    while( std::getline(input_stream,command)) {
        if (!input_stream) {
            output_stream << "Error: error reading command" << std::endl;
            std::getline(input_stream, command);
        } else {
            try {
                processCommand(command, graphs,output_stream);
            } catch (Vertex::Error &error) {
               output_stream<< error.getError() << std::endl;
            }
        }
    }
}

static void processCommand(string& str,std::vector<Graph>& graphs,std::ostream& os) {
    if(checkBasicCommand(str,graphs,os)){
        return;
    }else {
        //if we reach here then appearantly the command is of the form 'varialbe=...' we clean the string and check
        // if that is the case
        string command=cleanSpaces(str);
        checkCommand(command);
        insertSpaces(command);
        std::stringstream stringcommand(command);
        string variable_name;
        stringcommand >> variable_name;
        if (checkLegalWord(variable_name)) {
            Graph g = graphModifyFunction(graphs, stringcommand, variable_name);
            insertGraph(graphs, g);
            return;
        }
        //if we reach here then appearantly the command dosent match any of the known commands
        throw Vertex::Error("Error: Unrecognized command (processCommand)'" + command + "'");
    }
}

static bool checkBasicCommand(string& str,std::vector<Graph>& graphs,std::ostream& os){
    if(Graph::isSpace(str)||str.empty()){
        throw Vertex::Error("Error: Illegal command "+str);
    }
    string command=cleanSpaces(str);
    if (command == "who") {
        printGraphs(graphs,os);
        return true;
    }
    if (command == "reset") {
        deleteGraphs(graphs);
        return true;
    }
    if(command == "quit"){
        exit(0);
    }
    if (checkPrint(command)) {
        printFunction(graphs, command,os);
        return true;
    }
    if (checkDelete(command)) {
        deleteFunction(graphs, command);
        return true;
    }
    if(checkSave(command)){
        saveGraph(graphs,command);
        return true;
    }
    return false;
}

static Graph graphModifyFunction(std::vector<Graph>& graphs,std::stringstream& stream,const string& variable_name){
    string command;
    stream>>command;
    if(command!="="){
        string str=stream.str();

        throw Vertex::Error("Error: Illegal command "+str);
    }else{
        Graph g("{|}");
        g=graphOperation(graphs,stream);
        g.changeName(variable_name.substr(0,findLastLetter(variable_name)+1));
        return g;
    }
}


static Graph graphOperation(std::vector<Graph>& graphs,std::stringstream& stream) {
    string graph_name;
    string operation;
    stream >> graph_name;
    Graph g1(identifyGraph(graphs, stream,graph_name));
    while (stream >> operation) {
        if(!validOperation(operation)){
            throw Vertex::Error("Error: Illegal operation (graphOperation) "+operation);
        }
        if (stream.eof()) {
            throw Vertex::Error("Error: Illegal command:no variable after the operation");
        } else {
            stream >> graph_name;
            Graph g2(identifyGraph(graphs, stream,graph_name));//identifys what graph is talking about and construct it if needed
            g1 = calculateGraph(g1, g2, operation);//calculates the operation between the two graphs
        }
    }
    return g1;
}


static void insertGraph(std::vector<Graph>& graphs,Graph& g){
    std::vector<Graph>::iterator iterator = checkGraphName(graphs,g.getName());
    if(iterator!=graphs.end()){
        *iterator=g;

    }else{
        insertSortGraph(graphs,g);
    }
}

static void insertSortGraph(std::vector<Graph>& graphs,Graph& g){
    std::vector<Graph>::iterator iterator;
    for(iterator=graphs.begin();iterator!=graphs.end();++iterator){
        Graph g_temp(*iterator);
        if(g_temp.getName()>g.getName()){
            graphs.insert(iterator,g);
            return;
        }
    }
    graphs.push_back(g);
}

static Graph identifyGraph(std::vector<Graph>& graphs,std::stringstream& stream,string& graph_name){
    if (*graph_name.begin() == '(') {
        return calculateOpenBracketGraph(graphs,stream,graph_name);
    }
    if (*graph_name.begin() == '{') {
        return calculateBracketGraph(stream,graph_name);
    }
    if (*graph_name.begin() == '!') {//*
        return calculateNotGraph(graphs,graph_name);

    }
    if(checkLoadWord(graph_name)){
        return calculateLoadGraph(stream,graph_name);
    }
    return identifyGraphByName(graphs,graph_name);
}

static Graph calculateOpenBracketGraph(std::vector<Graph>& graphs,std::stringstream& stream,string& graph_name){
    string str = stream.str();
    str=str.substr(str.find(graph_name));
    int index=findOpenBracket(str);
    string str_temp=str.substr(1,index-1);
    std::stringstream stream_temp(str_temp);
    Graph g=graphOperation(graphs,stream_temp);
    str=str.substr(index+1);
    stream.str(str);
    return g;
}

static Graph calculateBracketGraph(std::stringstream& stream,string& graph_name){
    string str=stream.str();
    str=str.substr(str.find(graph_name));
    int index=findBracket(str);
    string str_temp=str.substr(0,index+1);
    Graph g(str_temp);
    str=str.substr(index+1);
    stream.str(str);
    return g;
}

static Graph calculateNotGraph(std::vector<Graph>& graphs,string& graph_name){
    graph_name.erase(graph_name.begin());
    std::vector<Graph>::iterator iterator = checkGraphName(graphs,graph_name);
    if(iterator == graphs.end()){
        throw Vertex::Error("Error: undefined Identifier '" + graph_name + "'");
    }else{
        Graph g(*iterator);
        return !g;
    }
}

static Graph calculateLoadGraph(std::stringstream& stream,string& graph_name){
    string str=stream.str();
    str=str.substr(str.find(graph_name));
    cleanSpaces(str);
    int bracket_index=str.find('(');
    int index=bracket_index+findOpenBracket(str.substr(bracket_index));
    string str_temp=str.substr(0,index+1);
    str=str.substr(index+1);
    stream.str(str);
    if(checkLoad(str_temp)){
        int beginning=str_temp.find('(');
        int end=str_temp.find(')');
        string loaded_graph_name=str_temp.substr(beginning+1,end-beginning-1);
        Graph g=loadGraph(loaded_graph_name);
        return g;
    }
    throw Vertex::Error("Error: unknown command '"+str_temp+"'");
}
static Graph identifyGraphByName(std::vector<Graph>& graphs,string& graph_name){
    std::vector<Graph>::iterator iterator = checkGraphName(graphs,graph_name);
    if (iterator == graphs.end()) {//check if the graph is indeed defined
        throw Vertex::Error("Error: undefined Identifier '" + graph_name + "'");
    } else {
        return *iterator;
    }
}




static void checkCommand(string& str){
    unsigned int index=0;
    if(!checkLetter(str[index])){
        throw Vertex::Error("Error: Illegal variable name (insertSpaces)" + str);
    }
}

static void insertSpaces(string& str){
    unsigned int index=0;
    for(;index<str.length()-1;index++){
        if((str[index]=='=')||(str[index]=='+')||(str[index]=='-')||(str[index]=='*')||(str[index]=='^')){
            if(!std::isspace(str[index-1])){
                str.insert(index," ");
            }
            if(!std::isspace(str[index+1])){
                str.insert(index+1," ");
            }
        }
        if((str[index]=='(')||(str[index]=='{')){
            if(!std::isspace(str[index-1])){
                str.insert(index," ");
            }
        }
        if((str[index]==')')||(str[index]=='}')){
            if(!std::isspace(str[index+1])){
                str.insert(index+1," ");
            }
        }

    }

}


static void printGraphs(const std::vector<Graph>& graphs,std::ostream& os){
    for(std::vector<Graph>::const_iterator iterator=graphs.begin();iterator!=graphs.end();++iterator){
        Graph g=*iterator;
        os<<g.getName()<<std::endl;
    }
}



static void deleteGraphs(std::vector<Graph>& graphs){
    graphs.clear();
}



static int findLastLetter(const string& str){
    int index=str.size()-1;
    while(std::isspace(str[index])){
        index--;
    }
    return index;
}


static bool checkPrint(const string& str){
    string word=str.substr(0,5);
    if(word=="print"){
        return checkDoubleBrackets(str);
    }
    return false;
}
static bool checkSave(const string& str){
    string word=str.substr(0,4);
    if(word=="save"){
        return checkDoubleBrackets(str);
    }
    return false;
}

static bool checkLoad(const string& str){
    string word=cleanSpaces(str);
    word=word.substr(0,4);
    if(word=="load"){
        return checkDoubleBrackets(str);
    }
    return false;
}

static bool checkDelete(const string& str){
    string word=str.substr(0,6);
    if(word=="delete"){
        return checkDoubleBrackets(str);
    }
    return false;
}

static bool checkDoubleBrackets(const string& str){
    if(str.find('(')!=string::npos){
        int index=findLastLetter(str);
        if(str[index]==')'){
            return true;
        }
    }
    return false;
}

static void printFunction(std::vector<Graph>& graphs,const std::string& command, std::ostream& os){//needs os
    //get the full name between the brackets
    string graph_name=cleanOpenBrackets(command);
    //clean the word from starting and ending spaces and insert spaces between operations if founded
    graph_name=cleanSpaces(graph_name);
    insertSpaces(graph_name);
    //identify the graph(calculate it if needed) and print it
    std::stringstream stream(graph_name);//the stream is only because indentifyGraph calls for it
    Graph g=graphOperation(graphs,stream);
    g.print(os);
}

static void saveGraph(std::vector<Graph>& graphs,const std::string& command){
    //get the full word between the brackets
   string word=cleanOpenBrackets(command);
   //make sure there is only one comma in the name
    if(!oneComma(word)){
        throw Vertex::Error("Error: Illegal command");
    }else{
        //seperate the graph name from the file name
        int index=word.find(',');
        string graph_name=word.substr(0,index);
        string filename=word.substr(index+1);
        //clean starting and ending spaces from the names
        cleanSpaces(filename);
        cleanSpaces(graph_name);
        //identify the graph(calculate it if needed) and save it
        std::stringstream stream(graph_name);//the stream is only because indentifyGraph calls for it
        Graph g=graphOperation(graphs,stream);
        g.saveToFile(filename);
    }
}

static string cleanOpenBrackets(const string& command){
    int last_letter=findLastLetter(command);
    int bracket_index=command.find('(');
    return command.substr(bracket_index+1,last_letter-bracket_index-1);
}

static bool oneComma(const string& str){
    int counter=0;
    for(unsigned int index=0;index<str.size();index++){
        if(str[index]==','){
            counter++;
        }
    }
    return counter==1;
}

static string cleanSpaces(const string& str){
    unsigned int begin_index=0;
    while((std::isspace(str[begin_index]))&&(begin_index<str.length())){
        begin_index++;
    }
    unsigned int end_index=str.size()-1;
    while((end_index>=0)&&(std::isspace(str[end_index]))){
        end_index--;
    }
    return str.substr(begin_index,end_index-begin_index+1);
}

static void deleteFunction(std::vector<Graph>& graphs,const string& command){
    int last_letter=findLastLetter(command);
    string word=command.substr(command.find('(')+1,last_letter-command.find('(')-1);
    string new_str=cleanSpaces(word);
    std::vector<Graph>::iterator iterator=graphs.begin();
    while(iterator!=graphs.end()){
        Graph g=*iterator;
        if(g.getName()==new_str){
            graphs.erase(iterator);
            return;
        }
        iterator++;
    }
    throw Vertex::Error("Error: Undefined Identifier '"+word+"'");
}

static bool checkLegalWord(const string& str){
    string::const_iterator it=str.begin();
    if(!checkLetter(*it)){
        throw Vertex::Error("Error: Illegal variable name "+str);
    }
    it++;
    for(;it!=str.end();++it){
        if(!checkLetterOrNumber(*it)){
            throw Vertex::Error("Error: Illegal variable name "+str);
        }
    }
    return true;
}

static bool checkLetter(const char& letter){
    if((letter>='a')&&(letter<='z')){
        return true;
    }
    if((letter>='A')&&(letter<='Z')){
        return true;
    }
    return false;
}

static bool checkLetterOrNumber(const char& letter){
    if(letter>='0'&&letter<='9'){return true;}
    if(letter>='a'&&letter<='z'){return true;}
    if(letter>='A'&&letter<='Z'){return true;}
    return false;
}



static bool validOperation(const string& operation){
    if(operation=="+"){
        return true;
    }
    if(operation=="-"){
        return true;
    }
    if(operation=="^"){
        return true;
    }
    if(operation=="*"){
        return true;
    }
    return false;
}

static bool checkLoadWord(const string& str){
    string word=cleanSpaces(str);
    word=word.substr(0,4);
    if(word=="load"){
        return true;
    }
    return false;
}

static Graph loadGraph(string& filename){
    if(filename.find(',')!=string::npos){
        throw Vertex::Error("Error: invalid filename '"+filename+"'");
    }
    int num_vertics,num_edges;
    Graph g("{|}");
    cleanSpaces(filename);
    std::ifstream infile(filename, std::ios_base::binary);
    infile.read((char*)&num_vertics, sizeof(unsigned int));
    infile.read((char*)&num_edges, sizeof(unsigned int));

    loadVertices(infile,g,num_vertics);
    loadEdges(infile,g,num_edges);
    return g;
}

static void loadVertices(std::ifstream& infile,Graph& g,int num_vertics){
    char charachter;
    int num_of_chars;
    string source;
    for(int index=0;index<num_vertics;++index){
        infile.read((char*)&num_of_chars, 4);
        source="";
        for(int char_index=0;char_index<num_of_chars;++char_index){
            infile.read((char*)&charachter, 1);
            source+=charachter;
        }
        g.fillVertices(source);
    }
}

static void loadEdges(std::ifstream& infile,Graph& g,int num_edges){
    string source,destination;
    int num_of_chars;
    char charachter;
    for(int index=0;index<num_edges;++index){
        infile.read((char*)&num_of_chars, 4);
        source="";
        for(int char_index=0;char_index<num_of_chars;++char_index){
            infile.read((char*)&charachter, 1);
            source+=charachter;
        }
        infile.read((char*)&num_of_chars, 4);
        destination="";
        for(int char_index=0;char_index<num_of_chars;++char_index){
            infile.read((char*)&charachter, 1);
            destination+=charachter;
        }
        string edge='<'+source+','+destination+'>';
        g.fillEdges(edge);
    }
}




static int findOpenBracket(const string& str){
    int counter=0;
    int index=0;
    for(string::const_iterator iterator=str.begin();iterator!=str.end();++iterator){
        if(*iterator=='('){
            counter++;
        }
        if(*iterator==')'){
            counter--;
        }
        if(counter==0){
            return index;
        }
        index++;
    }
    throw Vertex::Error("Error: Illegal command"+str);
}

static int findBracket(const string& str){
    int counter=0;
    int index=0;
    for(string::const_iterator iterator=str.begin();iterator!=str.end();++iterator){
        if(*iterator=='{'){
            counter++;
        }
        if(*iterator=='}'){
            counter--;
        }
        if(counter==0){
            return index;
        }
        index++;
    }
    throw Vertex::Error("Error: Illegal command:"+str);
}

static std::vector<Graph>::iterator checkGraphName(std::vector<Graph>& graphs,const string& graph_name){
    std::vector<Graph>::iterator iterator;
    for(iterator=graphs.begin();iterator!=graphs.end();++iterator){
       Graph g(*iterator);
        if(g.getName()==graph_name){
           return iterator;
       }
    }
    return iterator;
}


static Graph calculateGraph(Graph& g1,Graph& g2,string& operation){
    if(operation=="+"){
        return g1+g2;
    }
    if(operation=="-"){
        return g1-g2;
    }
    if(operation=="^"){
        return g1^g2;
    }
    if(operation=="*"){
        return g1*g2;
    }

    throw Vertex::Error("Error: Illegal operation (calculateGraph) "+operation);
}

