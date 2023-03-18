#include "tm.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <string.h>
using namespace std;
const vector<string> badlabels = {" ", "\t\n", ",", ";", "{", "}", "*", "_"};
void TuringMachine::handleerr(string msg, int line_num, string cur_line)
{
  cerr << msg << endl;

  if (this->verbose)
  {
    cerr << "Error: syntax error at line: " << line_num << endl;
    cerr << "In line: " << cur_line << endl;
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
set<string> split(string &str, char delim)
{
  set<string> ans;
  str += delim;
  string tmp;
  int flag = 0;
  while (str.length() > 0)
  {
    tmp = str.substr(flag, str.find(delim));
    ans.insert(tmp);
    str.erase(0, tmp.length() + 1);
  }
  // cout<<endl;
  return ans;
}
void TuringMachine::printerr(string err_in)
{
  if (this->verbose)
  {
    cerr << "Input: " << err_in << endl;
    cerr << "==================== ERR ====================" << endl;
  }
}
void TuringMachine::printend()
{
  if (this->verbose)
  {
    cerr << "==================== END ====================" << endl;
  }
}
int TuringMachine::init_tm(string &tm_def_path)
{
  ifstream tm_def;
  tm_def.open(tm_def_path);
  if (!tm_def.is_open())
  {
    cerr << "No such file or directory:" << tm_def_path << endl;
    return -1;
  }

  string cur_line;
  string error_msg;
  int line_num = 0;
  while (getline(tm_def, cur_line))
  {
    string ans;
    line_num++;
    ans = cur_line.substr(0, cur_line.find(';'));
    auto end = ans.find_last_not_of(" \n\r\t");
    if (end == string::npos)
      cur_line = "";
    else
      cur_line = cur_line.substr(0, end + 1);
    if (cur_line.length() < 1)
    {
      continue;
    }

    if (cur_line.find('#', 0) == 0)
    {
      if (cur_line[1] == 'Q')
      {
        auto left_brack = cur_line.find('{');
        auto right_brack = cur_line.find('}');
        if (left_brack == string::npos || right_brack == string::npos)
        {
          error_msg = "Q not including by { }";
          handleerr(error_msg, line_num, cur_line);
        }
        cur_line = cur_line.substr(left_brack + 1, right_brack - left_brack - 1);
        if (cur_line.find(' ') != string::npos)
        {
          error_msg = "whitespace in Q def";
          handleerr(error_msg, line_num, cur_line);
        }
        // cout << cur_line << endl;

        this->states = split(cur_line, ',');
      }
      else if (cur_line[1] == 'S')
      {
        auto left_brack = cur_line.find('{');
        auto right_brack = cur_line.find('}');
        if (left_brack == string::npos || right_brack == string::npos)
        {
          error_msg = "S not included by { }";
          handleerr(error_msg, line_num, cur_line);
        }
        cur_line = cur_line.substr(left_brack + 1, right_brack - left_brack - 1);
        set<string> tmp_set = split(cur_line, ',');
        for (string symbol : tmp_set)
        {
          for (string invalidStr : badlabels)
          {
            if (cur_line.find(invalidStr) != string::npos)
            {
              error_msg = "illegal str in S def: " + invalidStr;
              handleerr(error_msg, line_num, cur_line);
            }
          }
        }
        set<char> res;
        for (string i : tmp_set)
        {
          res.insert(i[0]);
        }
        this->input_alphbet = res;
      }
      else if (cur_line[1] == 'G')
      {
        auto left_brack = cur_line.find('{');
        auto right_brack = cur_line.find('}');
        if (left_brack == string::npos || right_brack == string::npos)
        {
          error_msg = "G not included by { }";
          handleerr(error_msg, line_num, cur_line);
        }
        cur_line = cur_line.substr(left_brack + 1, right_brack - left_brack - 1);
        set<string> tmp_set = split(cur_line, ',');
        for (string symbol : tmp_set)
        {
          for (string invalidStr : badlabels)
          {
            if (cur_line.find(invalidStr) != string::npos)
            {
              error_msg = "illegal str in G def: " + invalidStr;
              handleerr(error_msg, line_num, cur_line);
            }
          }
        }
        this->tape_alphabet = tmp_set;
      }
      else if (cur_line[1] == 'q')
      {
        if (cur_line[2] != '0' || cur_line.substr(3, 3) != " = ")
        {
          error_msg = "Syntex Error: Illegal def of state state";
          handleerr(error_msg, line_num, cur_line);
        }
        string q0(cur_line, 6, cur_line.length() - 5);
        this->startState = q0;
      }
      else if (cur_line[1] == 'B')
      {
        if (cur_line.substr(2, 3) != " = ")
        {
          error_msg = "Syntex Error: Illegal def of blank state" +
                      cur_line.substr(3, 3);
          handleerr(error_msg, line_num, cur_line);
        }
        char blank_s = cur_line[5];
        this->blankSymbol = blank_s;
      }
      else if (cur_line[1] == 'F')
      {
        auto left_brack = cur_line.find('{');
        auto right_brack = cur_line.find('}');
        if (left_brack == string::npos || right_brack == string::npos)
        {
          error_msg = "Final set should included by { }";
          handleerr(error_msg, line_num, cur_line);
        }
        cur_line = cur_line.substr(left_brack + 1, right_brack - left_brack - 1);
        if (cur_line.find(' ') != string::npos)
        {
          error_msg = "whitespace in Final set def";
          handleerr(error_msg, line_num, cur_line);
        }
        set<string> tmp_set = split(cur_line, ',');
        for (string state : tmp_set)
        {
          if (this->states.find(state) == this->states.end())
          {
            error_msg = "illegal state in Final states set: " + state;
            handleerr(error_msg, line_num, cur_line);
          }
        }
        this->finalStates = tmp_set;
      }
      else if (cur_line[1] == 'N')
      {
        if (cur_line.substr(2, 3) != " = ")
        {
          error_msg = "Syntex Error: illegal in tape Num def";
          handleerr(error_msg, line_num, cur_line);
        }
        string cur = cur_line.substr(5);
        this->tape_num = stoi(cur);
      }
      else
      {
        error_msg = "Syntex Error: Undefined symbol type start with #";
        handleerr(error_msg, line_num, cur_line);
      }
    }
    else
    {
      int run_flag = init_delta(cur_line);
      if (run_flag != 0)
      {
        error_msg = "Running Error: transition function def";
        handleerr(error_msg, line_num, cur_line);
      }
    }
  }
  for (int i = 0; i < this->tape_num; i++)
  {
    vector<char> tmp_tape;
    tmp_tape.push_back(this->blankSymbol);
    this->init_heads.push_back(0);
    this->tape_heads.push_back(0);
    this->tape.push_back(tmp_tape);
  }
  this->totalstep = 0;
  this->curState = this->startState;
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
    trans_def = trans_def.substr(tmp.length() + 1);
  }
  vector<string> keys;
  vector<string> vals;
  string error_msg = "Syntex Error: [Transition] ";
  if (items.size() != 5)
  {
    error_msg += "one line size less than 5";
    handleerr1(error_msg, items);
    return -1;
  }
  for (int idx = 0; idx < 5; idx++)
  {
    string item = items[idx];
    if (1 <= idx && idx <= 3)
    {
      if (item.length() != this->tape_num)
      {
        error_msg += "not equal to tape num\n";
        error_msg += "expect: " + to_string(this->tape_num) +
                     " get: " + to_string(item.length());
        error_msg += "[" + item + "]";
        handleerr1(error_msg, items);
        return -1;
      }
    }
    else
    {
      if (this->states.find(item) == this->states.end())
      {
        error_msg += "illegal TM state [" + item + "]";
        handleerr1(error_msg, items);
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
  this->transitions[keys] = vals;
  return 0;
}
int TuringMachine::run(string &input)
{
  if (checkInput(input) == false)
  {
    exit(-1);
  }
  init_tape(input);
  if (this->verbose)
  {
    cout << "Input: " << input << endl;
    cout << "==================== RUN ====================" << endl;
    showstep();
  }
  int run_flag = onestep();
  while (run_flag == 0)
  {
    run_flag = onestep();
  }
  if (run_flag == 1)
  { // halt
    if (this->verbose)
      cout << "[Halt] Result: ";
  }
  else if (run_flag == 2)
  { // accept
    if (this->verbose)
      cout << "[Accepted] Result: ";
  }
  else
  {
    if (this->verbose)
      cerr << "Something Error in Running" << endl;
    exit(-1);
  }

  int l = 0;
  int r = this->tape[0].size() - 1;
  while (l < this->tape[0].size() && this->tape[0][l] == this->blankSymbol && l < this->tape_heads[0])
    l++;
  while (r >= 0 && this->tape[0][r] == this->blankSymbol && r > this->tape_heads[0])
    r--;

  for (int i = l; i <= r; i++)
  {
    char c = this->tape[0][i];
    if (c != this->blankSymbol)
      cout << c;
  }
  cout << endl;
  printend();
  return 0;
}

bool TuringMachine::checkInput(string &input)
{
  for (auto i = 0; i < input.length(); i++)
  {
    if (this->input_alphbet.find(input[i]) ==
        this->input_alphbet.end())
    {
      if (this->verbose)
      {
        printerr(input);
        string blk(7 + i, ' ');
        blk = blk + "^";
        if (this->verbose)
        {
          cerr << "error: '" << input[i] << "' was not declared in the set of input symbols" << endl;
          cerr << "Input: " << input << endl;
          cerr << blk << endl;
        }
        printend();
      }
      else
      {
        cerr << "Input: " << input << endl;
        cerr << input[i] << " was illegal input symbol" << endl;
      }
      return false;
    }
  }
  return true;
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

int TuringMachine::onestep()
{
  this->totalstep++;
  string heads;
  for (int i = 0; i < this->tape_num; i++)
  {
    heads += this->tape[i][this->tape_heads[i]];
  }
  vector<string> curkeys;
  vector<string> next_env;
  bool isMatch = false;
  curkeys.push_back(this->curState);
  curkeys.push_back(heads);
  map<vector<string>, int> match_keys;
  for (auto &item : this->transitions)
  {
    if (item.first[0] == curkeys[0])
    {
      for (int i = 0; i < this->tape_num; i++)
      {
        if (item.first[1][i] == curkeys[1][i] || item.first[1][i] == '*')
        {
          isMatch = true;
        }
        else
        {
          isMatch = false;
          break;
        }
      }
      if (isMatch == true)
      {
        match_keys[item.first] = count(item.first[1].begin(), item.first[1].end(), '*');
      }
    }
    isMatch = false;
  }
  int temp = this->tape_num + 1;
  vector<string> last_key;
  for (auto item : match_keys)
  {
    if (item.second < temp)
    {
      last_key = item.first;
      temp = item.second;
    }
  }
  if (last_key.size() > 0)
  {
    next_env = this->transitions[last_key];
  }
  else
    next_env = last_key;

  if (next_env.size() == 0)
  { // halt
    if (this->verbose)
    {
      showstep();
    }
    return 1;
  }
  string next_tape_val = next_env[0];
  string next_dir = next_env[1];
  string next_state = next_env[2];
  for (int tape_idx = 0; tape_idx < this->tape_num; tape_idx++)
  {
    if (next_tape_val[tape_idx] != '*')
    {
      this->tape[tape_idx][this->tape_heads[tape_idx]] = next_tape_val[tape_idx];
    }

    if (next_dir[tape_idx] == 'l')
    {
      this->tape_heads[tape_idx] -= 1;
      if (this->tape_heads[tape_idx] < 0)
      {
        this->tape_heads[tape_idx] += 1;
        this->tape[tape_idx].insert(this->tape[tape_idx].begin(), this->blankSymbol);
        this->init_heads[tape_idx] += 1;
      }
    }
    else if (next_dir[tape_idx] == 'r')
    {
      this->tape_heads[tape_idx] += 1;
      if (this->tape_heads[tape_idx] >= this->tape[tape_idx].size())
        this->tape[tape_idx].push_back(this->blankSymbol);
    }
    else
      continue;
  }
  this->curState = next_state;
  if (this->finalStates.find(this->curState) != this->finalStates.end())
  {
    if (this->verbose)
      showstep();
    return 2;
  }
  if (this->verbose)
    showstep();
  return 0;
}

void TuringMachine::showstep()
{
  cout << "Step \t: " << this->totalstep << endl;
  for (int tape_idx = 0; tape_idx < this->tape_num; tape_idx++)
  {
    int l = 0;
    int r = this->tape[0].size() - 1;
    while (l < this->tape[0].size() && this->tape[0][l] == this->blankSymbol && l < this->tape_heads[0])
      l++;
    while (r >= 0 && this->tape[0][r] == this->blankSymbol && r > this->tape_heads[0])
      r--;

    cout << "Index" << tape_idx << " \t: ";
    vector<int> formal_blank;
    for (int i = l; i <= r; i++)
    {
      int index = abs(i - this->init_heads[tape_idx]);
      cout << index << " ";
      formal_blank.push_back(to_string(index).length());
    }
    cout << endl
         << "Tape" << tape_idx << " \t: ";
    for (int i = l; i <= r; i++)
    {
      string blanks = string(formal_blank[i - l], ' ');
      cout << this->tape[tape_idx][i] << blanks;
    }
    cout << endl
         << "Head" << tape_idx << " \t: ";
    for (int i = l; i < this->tape_heads[tape_idx] - l; i++)
    {
      string blanks = string(formal_blank[i], ' ');
      cout << " " << blanks;
    }
    cout << "^" << endl;
  }

  cout << "State \t: " << this->curState << endl;
  cout << "---------------------------------------------" << endl;
}