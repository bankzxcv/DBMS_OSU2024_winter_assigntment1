/*
Skeleton code for linear hash indexing
*/

#include <string>
#include <ios>
#include <fstream>
#include <vector>
#include <string>
#include <string.h>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cmath>
#include "classes.h"
using namespace std;

int main(int argc, char *const argv[])
{
  // Create the index

  // Create the EmployeeRelation file from Employee.csv
  // StorageBufferManager manager("EmployeeRelation");
  // manager.createFromFile("Employee.csv");

  LinearHashIndex emp_index("EmployeeIndex");

  emp_index.createFromFile("Employee.csv");
  // Loop to lookup IDs until user is ready to quit
  while (true)
  {
    try
    {
      /* code */

      int id;
      cout << "Enter employee ID to search: (-1) Exit : ";
      cin >> id;
      emp_index.findRecordById(id);
      if (id == -1)
      {
        break;
      }
    }
    catch (const std::exception &e)
    {
      std::cerr << e.what() << '\n';
    }
    catch (const std::exception &e)
    {
      std::cerr << e.what() << '\n';
    }
  }
  return 0;
}
