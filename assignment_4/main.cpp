/* This is a skeleton code for External Memory Sorting, you can make
   modifications as long as you meet all question requirements*/

#include <bits/stdc++.h>
#include "record_class.h"
using namespace std;

// defines how many blocks are available in the Main Memory
#define buffer_size 22

Records buffers[buffer_size];  // use this class object of size 22 as your main
                               // memory

/***You can change return type and arguments as you want.***/

// PASS 1
// Sorting the buffers in main_memory and storing the sorted records into a
// temporary file (Runs)
void Sort_Buffer(Records buffers[], int i, int &runningNumber) {
  // Remember: You can use only [AT MOST] 22 blocks for sorting the records /
  // tuples and create the runs
  // create run_{runningNumber} file

  // create file
  string filename = "run_" + to_string(runningNumber++) + ".csv";
  fstream run;
  run.open(filename, ios::out | ios::app);

  sort(buffers, buffers + i, [](Records a, Records b) {
    return a.emp_record.eid < b.emp_record.eid;
  });
  if (!run.is_open()) {
    cerr << "Failed to open file runs.csv" << endl;
    return;
  }

  run << "eid,ename,age,salary" << endl;
  for (int j = 0; j < i; j++) {
    run << buffers[j].emp_record.eid << "," << buffers[j].emp_record.ename
        << "," << buffers[j].emp_record.age << ","
        << buffers[j].emp_record.salary << endl;
  }
  run.close();
}

// PASS 2
// Use main memory to Merge the Runs
// which are already sorted in 'runs' of the relation Emp.csv
void Merge_Runs() {
  // and store the Sorted results of your Buffer using PrintSorted()
  return;
}

void PrintSorted() {
  // Store in EmpSorted.csv
  return;
}

int main() {
  // Open file streams to read and write
  // Opening out the Emp.csv relation that we want to Sort
  fstream empin;
  empin.open("Emp.csv", ios::in);
  fstream SortOut;
  SortOut.open("EmpSorted.csv", ios::out | ios::app);

  // 1. Create runs for Emp which are sorted using Sort_Buffer()
  // read file into buffer each bach each 22 line of file then sort it and write
  // to runs.csv
  string line;

  int runningNumber = 0;
  int i = 0;
  while (true) {
    Records r = Grab_Emp_Record(empin);
    if (r.no_values == -1) {
      cout << "STARTING SORTING" << endl;
      Sort_Buffer(buffers, i, runningNumber);
      break;
    }
    // push r to buffers
    buffers[i++] = r;  // buffers[i] = r;
    // buffers.push_back(r);
    if (i == buffer_size) {
      cout << "STARTING SORTING" << endl;
      Sort_Buffer(buffers, i, runningNumber);
      i = 0;
      memset(buffers, 0, sizeof(buffers));
      // clear buffers to empty
    }
  }

  // 2. Use Merge_Runs() to Sort the runs of Emp relations

  // Please delete the temporary files (runs) after you've sorted the Emp.csv

  return 0;
}
