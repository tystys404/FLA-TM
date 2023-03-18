#include <map>
#include <set>
#include <string>
#include <vector>
#include <algorithm>
#include <stdlib.h>
using namespace std;

class TuringMachine
{
public:
  int totalstep = 0;
  TuringMachine(string &tm_def_path, bool verbose)
  {
    this->verbose = verbose;
    init_tm(tm_def_path);
  }

  int run(string &input);
  void handleerr(string msg, int line_num, string tmp_line);
  void handleerr1(string msg, vector<string> item);
  void printerr(string err_in);
  void printend();

  bool verbose;
  set<string> states;
  set<char> input_alphbet;
  set<string> tape_alphabet;
  string startState;
  char blankSymbol;
  set<string> finalStates;
  int tape_num;
  map<vector<string>, vector<string>> transitions;

  vector<vector<char>> tape;
  vector<int> tape_heads;
  string curState;
  vector<int> init_heads;

  int init_tm(string &tm_def_path);
  int init_delta(string &delta_def_path);

  bool checkInput(string &input);
  int init_tape(string &input);
  int onestep();
  void showstep();
};
set<string> split(string &str, char delim);