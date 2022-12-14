#include "tm.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <string.h>
using namespace std;
const vector<string> badlabels = {" ", "\t\n", ",", ";", "{", "}", "*", "_"};
void TuringMachine::handleerr(string msg, int line_num, string tmp_line)
{
  cerr << msg << endl;

  if (this->verbose)
  {
    std::cerr << "Error: syntax error at line: " << line_num << std::endl;
    std::cerr << "In line: " << tmp_line << endl;
  }
  exit(-1);
}
void TuringMachine::handleerr1(string msg, vector<string> items)
{
  cerr << msg << endl;
  if (this->verbose)
  {
    cerr << "items:";
    for (string item : items)
    {
      cerr << item << ' ';
    }
    cerr << endl;
  }
}
set<string> split(string &str, char delimiter)
{
  set<string> result;
  str += delimiter;
  string tmp;
  int flag = 0;
  while (str.length() > 0)
  {
    tmp = str.substr(flag, str.find(delimiter));
    // cout<<tmp<<" ";
    result.insert(tmp);
    str.erase(0, tmp.length() + 1);
  }
  // cout<<endl;
  return result;
}
void TuringMachine::printerr(string err_in)
{
  if (this->verbose)
  {
    cerr << "Input: " << err_in << endl;
    cerr << "==================== ERR ====================" << endl;
  }
}
void TuringMachine::printerrdetail(string err_input, string blk, char c)
{
  if (this->verbose)
  {
    cerr << "error: '" << c
         << "' was not declared in the set of input symbols" << endl;
    cerr << "Input: " << err_input << endl;
    cerr << blk << endl;
  }
}
void TuringMachine::printend()
{
  if(this->verbose)
  {
    cerr << "==================== END ====================" << endl;
  }
}
vector<int> getBound(const vector<char> tape, const int head,
                     const char blank)
{
  int lBound = 0;
  int rBound = tape.size() - 1;

  for (; lBound < tape.size() && tape[lBound] == blank && lBound < head;
       lBound++)
    ;
  for (; rBound >= 0 && tape[rBound] == blank && rBound > head; rBound--)
    ;

  vector<int> res = {lBound, rBound};
  return res;
}
int TuringMachine::init_tm(string &TMDef)
{
  ifstream tm_def;
  tm_def.open(TMDef);
  if (!tm_def.is_open())
  {
    std::cerr << "No such file or directory:" << TMDef << std::endl;
    return -1;
  }

  string tmp_line;
  string erro_msg;
  int line_num = 0;
  while (getline(tm_def, tmp_line))
  {
    line_num++;
    string ans;
    ans = tmp_line.substr(0, tmp_line.find(';'));
    auto end = ans.find_last_not_of(" \n\r\t");
    if (end == string::npos)
      tmp_line = "";
    else
      tmp_line = tmp_line.substr(0, end + 1);
    if (tmp_line.length() < 1)
    {
      continue;
    }

    if (tmp_line.find('#', 0) == 0)
    {
      switch (tmp_line[1])
      {
      case 'Q':
      {
        size_t lBrack = tmp_line.find('{');
        size_t rBrack = tmp_line.find('}');
        if (lBrack == string::npos || rBrack == string::npos)
        {
          erro_msg = "Q not including by { }";
          handleerr(erro_msg, line_num, tmp_line);
        }
        tmp_line = tmp_line.substr(lBrack + 1, rBrack - lBrack - 1);
        if (tmp_line.find(' ') != string::npos)
        {
          erro_msg = "whitespace in Q def";
          handleerr(erro_msg, line_num, tmp_line);
        }
        // cout << tmp_line << endl;

        this->states = split(tmp_line, ',');
      }
      break;

      case 'S':
      {
        size_t lBrack = tmp_line.find('{');
        size_t rBrack = tmp_line.find('}');
        if (lBrack == string::npos || rBrack == string::npos)
        {
          erro_msg = "S not included by { }";
          handleerr(erro_msg, line_num, tmp_line);
        }
        tmp_line = tmp_line.substr(lBrack + 1, rBrack - lBrack - 1);

        set<string> tmp_set = split(tmp_line, ',');
        for (string symbol : tmp_set)
        {
          for (string invalidStr : badlabels)
          {
            if (tmp_line.find(invalidStr) != string::npos)
            {
              erro_msg = "illegal str in S def: " + invalidStr;
              handleerr(erro_msg, line_num, tmp_line);
            }
          }
        }

        set<char> res;
        for (string i : tmp_set)
        {
          res.insert(i[0]);
        }

        this->inputSymbols = res;
      }
      break;

      case 'G':
      {
        size_t lBrack = tmp_line.find('{');
        size_t rBrack = tmp_line.find('}');
        if (lBrack == string::npos || rBrack == string::npos)
        {
          erro_msg = "G not included by { }";
          handleerr(erro_msg, line_num, tmp_line);
        }
        tmp_line = tmp_line.substr(lBrack + 1, rBrack - lBrack - 1);
        set<string> tmp_set = split(tmp_line, ',');
        for (string symbol : tmp_set)
        {
          for (string invalidStr : badlabels)
          {
            if (tmp_line.find(invalidStr) != string::npos)
            {
              erro_msg = "illegal str in G def: " + invalidStr;
              handleerr(erro_msg, line_num, tmp_line);
            }
          }
        }

        this->tapeSymbols = tmp_set;
      }
      break;

      case 'q':
      {
        if (tmp_line[2] != '0' || tmp_line.substr(3, 3) != " = ")
        {
          erro_msg = "Syntex Error: Illegal def of state state";
          handleerr(erro_msg, line_num, tmp_line);
        }

        string q0(tmp_line, 6, tmp_line.length() - 5);
        this->startState = q0;
      }
      break;

      case 'B':
      {
        if (tmp_line.substr(2, 3) != " = ")
        {
          erro_msg = "Syntex Error: Illegal def of blank state" +
                     tmp_line.substr(3, 3);
          handleerr(erro_msg, line_num, tmp_line);
        }
        // string blank(tmp_line, 5, 1);
        this->blankSymbol = tmp_line[5];
      }
      break;

      case 'F':
      {
        size_t lBrack = tmp_line.find('{');
        size_t rBrack = tmp_line.find('}');
        if (lBrack == string::npos || rBrack == string::npos)
        {
          erro_msg = "Final set should included by { }";
          handleerr(erro_msg, line_num, tmp_line);
        }
        tmp_line = tmp_line.substr(lBrack + 1, rBrack - lBrack - 1);
        if (tmp_line.find(' ') != string::npos)
        {
          erro_msg = "whitespace in Final set def";
          handleerr(erro_msg, line_num, tmp_line);
        }

        set<string> tmp_set = split(tmp_line, ',');

        for (string state : tmp_set)
        {
          if (this->states.find(state) == this->states.end())
          {
            erro_msg = "illegal state in Final states set: " + state;
            handleerr(erro_msg, line_num, tmp_line);
          }
        }

        this->finalStates = tmp_set;
      }
      break;

      case 'N':
      {
        if (tmp_line.substr(2, 3) != " = ")
        {
          erro_msg = "Syntex Error: illegal in tape Num def";
          handleerr(erro_msg, line_num, tmp_line);
        }

        // tmp_line = tmp_line[5];
        // this->numOfTapes = stoi(tmp_line);
        string cur = tmp_line.substr(5);
        this->numOfTapes = stoi(cur);
      }
      break;

      default:
        erro_msg = "Syntex Error: Undefined symbol type start with #";
        handleerr(erro_msg, line_num, tmp_line);
      }
    }
    else
    { // transition
      int ret_code = init_delta(tmp_line);
      if (ret_code != 0)
      {
        erro_msg = "Running Error: transition function def";
        handleerr(erro_msg, line_num, tmp_line);
      }
    }
  }

  for (int i = 0; i < this->numOfTapes; i++)
  {
    vector<char> tmp_tape;
    tmp_tape.push_back(this->blankSymbol);

    this->tape.push_back(tmp_tape);
    this->tapeHead.push_back(0);
    this->initHead.push_back(0);
  }
  this->curState = this->startState;
  this->globalStep = 0;

  return 0;
}

int TuringMachine::init_delta(string &trans_def)
{
  vector<string> items;
  trans_def += ' ';
  while (trans_def.length() > 0)
  {
    string tmp = trans_def.substr(0, trans_def.find(' '));
    items.push_back(tmp);
    trans_def.erase(0, tmp.length() + 1);
  }
  vector<string> keys;
  vector<string> vals;
  string erro_msg = "Syntex Error: [Transition] ";
  if (items.size() != 5)
  {
    erro_msg += "items size not equal to 5, get" + items.size();
    handleerr1(erro_msg, items);
    return -1;
  }
  for (int idx = 0; idx < 5; idx++)
  {
    string item = items[idx];
    if (1 <= idx && idx <= 3)
    {
      if (item.length() != this->numOfTapes)
      {
        erro_msg += "not equal to tape num\n";
        erro_msg += "expect: " + to_string(this->numOfTapes) +
                    " get: " + to_string(item.length());
        erro_msg += "[" + item + "]";
        handleerr1(erro_msg, items);
        return -1;
      }
    }
    else
    {
      if (this->states.find(item) == this->states.end())
      {
        erro_msg += "illegal TM state [" + item + "]";
        handleerr1(erro_msg, items);
        return -1;
      }
    }

    if (idx < 2)
    {
      keys.push_back(item);
    }
    else
    {
      vals.push_back(item);
    }
  }
  this->transitionFunction[keys] = vals;
  return 0;
}

void TuringMachine::displayTMDef()
{
  vector<set<string>> items = {this->states, this->tapeSymbols,
                               this->finalStates};
  vector<string> names = {"states", "tapeSymbol", "finalStates"};
  for (int idx = 0; idx < 3; idx++)
  {
    cout << names[idx] << " : " << endl;
    for (string item : items[idx])
    {
      cout << item << ' ';
    }
    cout << endl;
  }

  cout << "Input Symbols: ";
  for (char ch : this->inputSymbols)
  {
    cout << ch << ' ';
  }
  cout << endl;

  cout << "startState: " << this->startState << endl;
  cout << "blankSymbol: " << this->blankSymbol << endl;
  cout << "tape num: " << this->numOfTapes << endl;

  vector<string> keys;
  vector<string> vals;
  for (auto const &m : this->transitionFunction)
  {
    keys = m.first;
    vals = m.second;
    for (string i : keys)
    {
      cout << i << ' ';
    }
    for (string i : vals)
    {
      cout << i << ' ';
    }
    cout << endl;
  }
}

int TuringMachine::run(string &input)
{
  /*
   * Run TM on given input string
   */

  if (checkInput(input) != 0)
  {
    exit(-1);
  }
  init_tape(input);

  if (this->verbose)
  {
    cout << "Input: " << input << endl;
    cout << "==================== RUN ====================" << endl;
    stepDisplay();
  }

  int ret_code = singalStep();
  while (ret_code == 0)
  {
    ret_code = singalStep();
  }

  switch (ret_code)
  {
  case 1:
  { // halt
    if (this->verbose)
      cout << "[Halt] Result: ";
  }
  break;
  case 2:
  { // accept
    if (this->verbose)
      cout << "[Accepted] Result: ";
  }
  break;
  default:
    if (this->verbose)
      cerr << "Something Error in Running" << endl;
    exit(-1);
  }

  // display the item on tape[0]
  vector<int> tmp =
      getBound(this->tape[0], this->tapeHead[0], this->blankSymbol);
  int l = tmp[0];
  int r = tmp[1];
  for (int i = l; i <= r; i++)
  {
    char c = this->tape[0][i];
    if (c != this->blankSymbol)
      cout << c; // ignore the blank on the tape
  }
  cout << endl;
  printend();
  return -1;
}

int TuringMachine::checkInput(string &input)
{
  for (auto i = 0; i < input.length(); i++)
  {
    if (this->inputSymbols.find(input[i]) ==
        this->inputSymbols.end())
    { // not in symbol set
      if (this->verbose)
      {
        printerr(input);
        string blk(7 + i, ' ');
        blk += "^";
        printerrdetail(input, blk, input[i]);
        printend();
      }
      else
      {
        cerr << "Input: " << input << endl;
        cerr << input[i] << " was illegal input symbol" << endl;
      }
      return -1;
    }
  }

  return 0;
}

int TuringMachine::init_tape(string &input)
{
  this->tape[0].clear();
  this->tape.push_back({});
  for (char ch : input)
  {
    this->tape[0].push_back(ch);
  }
  return 0;
}

int TuringMachine::singalStep()
{
  this->globalStep++;
  string heads;
  for (int i = 0; i < this->numOfTapes; i++)
  {
    heads += this->tape[i][this->tapeHead[i]];
  }
  vector<string> curkeys;
  curkeys.push_back(this->curState);
  curkeys.push_back(heads);
  vector<string> nextEnv = getNextEnv(curkeys);
  if (nextEnv.size() < 1)
  { // halt, no next state
    if (this->verbose)
    {
      stepDisplay();
    }
    return 1;
  }

  string nextTapeVals = nextEnv[0];
  string nextDircts = nextEnv[1];
  string nextState = nextEnv[2];
  for (int tapeId = 0; tapeId < this->numOfTapes; tapeId++)
  {
    if (nextTapeVals[tapeId] != '*')
    {
      this->tape[tapeId][this->tapeHead[tapeId]] = nextTapeVals[tapeId];
    }

    if (nextDircts[tapeId] == 'r')
    {
      this->tapeHead[tapeId] += 1;
      if (this->tapeHead[tapeId] >= this->tape[tapeId].size())
      {
        this->tape[tapeId].push_back(this->blankSymbol);
      }
    }
    else if (nextDircts[tapeId] == 'l')
    {
      this->tapeHead[tapeId] -= 1;
      if (this->tapeHead[tapeId] < 0)
      {
        this->tapeHead[tapeId] += 1;
        this->tape[tapeId].insert(this->tape[tapeId].begin(),
                                  this->blankSymbol);
        this->initHead[tapeId] += 1; // for tape index
      }
    }
    else
    {
      continue;
    }
  }
  this->curState = nextState;

  if (this->finalStates.find(this->curState) != this->finalStates.end())
  {
    if (this->verbose)
    {
      stepDisplay();
    }
    return 2;
  }

  if (this->verbose)
  {
    stepDisplay();
  }
  return 0;
}

void TuringMachine::stepDisplay()
{
  cout << "Step  \t: " << this->globalStep << endl;
  for (int tapeId = 0; tapeId < this->numOfTapes; tapeId++)
  {
    auto tmp =
        getBound(this->tape[tapeId], this->tapeHead[tapeId], this->blankSymbol);
    int lBound = tmp[0];
    int rBound = tmp[1];

    cout << "Index" << tapeId << " \t: ";
    vector<int> alignLength;
    for (int i = lBound; i <= rBound; i++)
    {
      int index = i - this->initHead[tapeId];
      index = index >= 0 ? index : -index; // abs
      cout << index << " ";
      alignLength.push_back(to_string(index).length());
    }
    cout << endl;

    cout << "Tape" << tapeId << " \t: ";
    for (int i = lBound; i <= rBound; i++)
    {
      string blanks = string(alignLength[i - lBound], ' ');
      cout << this->tape[tapeId][i] << blanks;
    }
    cout << endl;

    cout << "Head" << tapeId << " \t: ";
    for (int i = lBound; i < this->tapeHead[tapeId] - lBound; i++)
    {
      string blanks = string(alignLength[i], ' ');
      cout << " " << blanks;
    }
    cout << "^" << endl;
  }

  cout << "State \t: " << this->curState << endl;
  cout << "---------------------------------------------" << endl;
}

vector<string> TuringMachine::getNextEnv(const vector<string> keys)
{
  bool isMatch = false;
  // vector<bool> matchingRes;
  // vector<vector<string>> matchingKeys;
  map<vector<string>, int> mathchingKeys2priority;

  for (auto item : this->transitionFunction)
  {
    if (item.first[0] == keys[0])
    { // same state
      // cerr << item.first[0] << keys[0] << item.first[1] << keys[1] << endl;

      for (int i = 0; i < this->numOfTapes; i++)
      {
        if (item.first[1][i] == keys[1][i] || item.first[1][i] == '*')
        {
          isMatch = true;
        }
        else
        {
          isMatch = false;
          break;
        }
      }
      if (isMatch)
      {
        mathchingKeys2priority[item.first] =
            count(item.first[1].begin(), item.first[1].end(), '*');
      }
    }
    isMatch = false;
  }

  int min = this->numOfTapes + 1;
  vector<string> finalKey;
  for (auto item : mathchingKeys2priority)
  {
    if (item.second < min)
    {
      min = item.second;
      finalKey = item.first;
    }
  }
  if (finalKey.size() > 0)
  {
    return this->transitionFunction[finalKey];
  }
  return finalKey;
}