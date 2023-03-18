#include <iostream>
#include <string>
#include <set>
#include <vector>
#include "tm.h"
using namespace std;
int main(int argc, char *argv[])
{
  bool verbose = false;
  string HelpMsg = "turing <tm> <input>\nuse [-v|--verbose] to aactivate verbose mode";
  string tm_path = "";
  string input = "";
  if (argc == 1)
  {
    cout << HelpMsg << endl;
    return 0;
  }
  else
  {
    for (int i = 1; i < argc; i++)
    {
      string tmp = argv[i];
      if (tmp == "-v" || tmp == "--verbose")
      {
        verbose = true;
      }
      else if (tmp == "-h" || tmp == "--help")
      {
        cout << HelpMsg << endl;
        return 0;
      }
      else
      {
        if (tm_path == "")
        {
          tm_path = tmp;
        }
        else
        {
          input = tmp;
        }
      }
    }
  }

  if (tm_path == "" || input == "")
  {
    cerr << "Error: no tm def file at" << tm_path << "or empty input"<< endl;
    return 0;
  }

  TuringMachine tm = TuringMachine(tm_path, verbose);
  tm.run(input);
  return 0;
}