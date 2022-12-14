#include <iostream>
#include <string>
#include <set>
#include <vector>
#include "tm.h"
using namespace std;
int main(int argc, char *argv[])
{
  string HelpMsg = "usage: turing [-v|--verbose] [-h|--help] <tm> <input>";
  bool verbose = false;
  string tm_def_path = "";
  string input = "";
  if (argc == 1)
  {
    std::cout << HelpMsg << std::endl;
    return 0;
  }
  else
  {
    for (int i = 1; i < argc; i++)
    {
      string arg = argv[i];
      if (arg == "-v" || arg == "--verbose")
      {
        // verbose mode
        verbose = true;
      }
      else if (arg == "-h" || arg == "--help")
      {
        cout << HelpMsg << endl;
        return 0;
      }
      else
      {
        // tm definition file
        if (tm_def_path == "")
        {
          tm_def_path = arg;
        }
        // tm input string
        else
        {
          input = arg;
        }
      }
    }
  }

  if (tm_def_path == "" || input == "")
  {
    std::cerr << "Error: no tm def file at" << tm_def_path << "or empty input"
              << std::endl;
    return 0;
  }

  TuringMachine tm = TuringMachine(tm_def_path, verbose);
  tm.run(input);
  return 0;
}