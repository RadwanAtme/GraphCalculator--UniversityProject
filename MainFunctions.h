
#ifndef FINALPROJECT_MAINFUNCTIONS_H
#define FINALPROJECT_MAINFUNCTIONS_H

#include <vector>


/*
 * showMassege:this function prints Gcalc>
 */
static void showMassege();
/*
 * processInput:this function is responsibale for getting input from the user and sending the commands to
 *          processCommand to apply the commands
 */
static void processInput(std::vector<Graph>& graphs);
/*
 * processInputBatchMode:this function is essentially the same as processInput only it is adjusted to work
 *          with batch mode
 */
static void processInputBatchMode(std::vector<Graph>& graphs,std::ifstream& input_stream,
                                  std::ofstream& output_stream);
/*
 * processCoammand:this function recives a command(a full line of input) and checks if the command matches
 *      one of the known commands,if so it calls to the needed function to apply it
 */
static void processCommand(string& str,std::vector<Graph>& graphs,std::ostream& os);
/*
 * checkBasicCommand:checks if the command recived is one of the basic commands:
 *      who , reset , quit, print(), delete() and save(), if so this function calls the needed functions
 *      to excecute the command and returns true,otherwise it returns false
 */
static bool checkBasicCommand(string& str,std::vector<Graph>& graphs,std::ostream& os);
/*
 * graphModifyFunction:this function recives a command that is suppose to be of the form G=...
 *      the function first checks if that is the case and then calls to the needed functions to make the
 *      calculations and insert the graph to the graphs vector
 */
static Graph graphModifyFunction(std::vector<Graph>& graphs,std::stringstream& stream,const string& variable_name);
/*
 * graphOperation:this functions recives the full command given in the form of a stream and then applies the needed
 *      operations to get the desired graph and returns it
 */
static Graph graphOperation(std::vector<Graph>& graphs,std::stringstream& stream);
/*
 * insertGraph: this function checks if the graph is already in the vector,if that is the case the functions
 *      replaces the old graph with the new one,if not then the functions calls to insertSortGraph to insert the graph
 *      the vector in a sorted manner
 */
static void insertGraph(std::vector<Graph>& graphs,Graph& g);
/*
 * insertSortGraph:given a graph this functions finds its suitable place in the graphs vector and inserts it there
 */
static void insertSortGraph(std::vector<Graph>& graphs,Graph& g);
/*
 * identifyGraph: given a string of the form {v1,v2|<v1,v2>} or (G+G1*G2) or !G or load() or  'G' this functions
 *      identifys the graph that is called upon using calculate functions and returns it
 */
static Graph identifyGraph(std::vector<Graph>& graphs,std::stringstream& stream,string& graph_name);
/*
 * calculates a graph in the form of (G+G1*G2) and returns it
 */
static Graph calculateOpenBracketGraph(std::vector<Graph>& graphs,std::stringstream& stream,string& graph_name);
/*
 * calculates a graph in the form of {v1,v2|<v1,v2>}  and returns it
 */
static Graph calculateBracketGraph(std::stringstream& stream,string& graph_name);
/*
 * calculates a graph in the form of !G  and returns it
 */
static Graph calculateNotGraph(std::vector<Graph>& graphs,string& graph_name);
/*
 * calculates a graph in the form of  load()  and returns it
 */
static Graph calculateLoadGraph(std::stringstream& stream,string& graph_name);
/*
 * identifys a graph by its name and returns it
 */
static Graph identifyGraphByName(std::vector<Graph>& graphs,string& graph_name);
/*
 * printGraphs:prints all the graphs in the vector
 */
static void printGraphs(const std::vector<Graph>& graphs,std::ostream& os);
/*
 * deleteGraphs:deletes all the graphs in the vector
 */
static void deleteGraphs(std::vector<Graph>& graphs);
static bool checkPrint(const string& str);
static bool checkDelete(const string& str);
static void printFunction(std::vector<Graph>& graphs,const std::string& command, std::ostream& os);
static void deleteFunction(std::vector<Graph>& graphs,const string& command);
static bool checkLegalWord(const string& str);
static bool checkLetter(const char& letter);
static bool checkLetterOrNumber(const char& letter);
static Graph calculateGraph(Graph& g1,Graph& g2,string& operation);
static bool validOperation(const string& operation);
static std::vector<Graph>::iterator checkGraphName(std::vector<Graph>& graphs,const string& graph_name);
static int findOpenBracket(const string& str);
static int findBracket(const string& str);
static Graph identifyGraph(std::vector<Graph>& graphs,std::stringstream& stream,string& graph_name);
static void insertSpaces(string& str);
static int findLastLetter(const string& str);
/*
 * cleanSpaces:given a string this function cleans starting and ending spaces(if found) and returns a cleaner
 *      version of the string
 */
static string cleanSpaces(const string& str);
static bool checkDoubleBrackets(const string& str);
static bool checkLoadWord(const string& str);
static Graph loadGraph(string& filename);
static bool checkSave(const string& str);
/*
 * oneComma:this function checks if the string has exactly one comma in it
 */
static bool oneComma(const string& str);
/*
 * saveGraph:this function is called after getting a save() command to extract the filename and graphname from
 *          the command and then call the member function saveToFile to save the graph in the given file in binary
 *          form
 */
static void saveGraph(std::vector<Graph>& graphs,const std::string& command);
/*
 * cleanOpenBrackets:given a string that is suppose to be print() or load() or save() this function extracts
 *          the full word between the open brackets and returns it to the user
 */
static string cleanOpenBrackets(const string& command);
/*
 * checkCommand: checks that the command starts with a letter(in actual usage it checks if the varialbe name starts
 *      with a letter)
 */
static void checkCommand(string& str);
static bool checkLoad(const string& str);
static void loadVertices(std::ifstream& infile,Graph& g,int num_vertics);
static void loadEdges(std::ifstream& infile,Graph& g,int num_edges);

Graph* createEmptyGraph();
void destroyGraph(Graph* graph);
Graph* addVertex(Graph* graph,string vertex);
Graph* addEdge(Graph* graph,string v1,string v2);
void Display(Graph* graph);
int isNull(Graph* graph);
Graph* graphUnion(Graph* graph_in1,Graph* graph_in2,Graph* graph_out);
Graph*  graphIntersection(Graph* graph_in1,Graph* graph_in2,Graph* graph_out);
Graph*  graphDifference(Graph* graph_in1,Graph* graph_in2,Graph* graph_out);
Graph* graphProduct(Graph* graph_in1,Graph* graph_in2,Graph* graph_out);
Graph* graphComplement(Graph* graph_in,Graph* graph_out);


































#endif //FINALPROJECT_MAINFUNCTIONS_H
