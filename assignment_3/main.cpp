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

int main(int argc, char* const argv[]) {
  // Create the index
  LinearHashIndex emp_index("EmployeeIndex");
  emp_index.createFromFile("Employee.csv");

  // Create the EmployeeRelation file from Employee.csv
  // StorageBufferManager manager("EmployeeRelation");
  // manager.createFromFile("Employee.csv");

  // Loop to lookup IDs until user is ready to quit
  // int id;
  // cout << "Enter employee ID to search: ";
  // cin >> id;
  // Record result = emp_index.findRecordById(id);
  // if (result.id != -1) {
  //     cout << "Record found:\n";
  //     result.print();
  // } else {
  //     cout << "Record not found.\n";
  // }

  return 0;
}
