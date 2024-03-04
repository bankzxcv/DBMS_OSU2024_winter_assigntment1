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

    cout<<"Sort_Buffer   "<< i <<endl;
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
  // set up head to buffer
  // let say buffer[0] is the Output buffer
  // buffer[1] to buffer[21] are M-1 buffers
  cout << "Merge Run" << endl;
  int count = 0;
  for (int i = 1; i < 22; i++) {
    fstream run;
    string stringFileName = "run_" + to_string(i - 1) + ".csv";
    run.open(stringFileName, ios::in);
    if (!run.is_open()) {
      cerr << "Failed to open file " + stringFileName << endl;
      continue;
    }
    Records r = Grab_Emp_Record(run);
    buffers[i] = r;
    buffers[i].index = run.tellg();
    buffers[i].runName = stringFileName;
    run.close();
    count++;
  }
  cout << "having " << count << " runs" << endl;

  buffers[0].no_values = -1;
  buffers[0].runName = "EmpSorted.csv";
  buffers[0].emp_record.eid = numeric_limits<int>::max();
  cout << "RIGHT HERE" << endl;

  // function min of two integers
  auto min = [](int a, int b) { return a < b ? a : b; };

  // define max Int
  int maxInt = numeric_limits<int>::max();

  // function min of two records
  int minIndex = min(22, count);

  while (true) {
    buffers[0].emp_record.eid = maxInt;
    buffers[0].no_values = -1;
    for (int i = 1; i <= minIndex; i++) {
      cout << "buffer[" << i << "].eid = " << buffers[i].emp_record.eid
           << " file = " << buffers[i].runName << " index: " << buffers[i].index
           << endl;

      if (buffers[i].no_values == -1) {
        continue;
      }
      if (buffers[i].emp_record.eid < buffers[0].emp_record.eid) {
        buffers[0].emp_record = buffers[i].emp_record;
        buffers[0].currentMPage = i - 1;
        buffers[0].no_values = 1;
      }
    }

    cout << "outputBuffer value = " << buffers[0].emp_record.eid << endl;
    cout << "currentMPage = " << buffers[0].currentMPage << endl;
    if (buffers[0].no_values == -1) {
      cout << "no_values = -1" << endl;
      // remove all run files
      for(int i = 0; i < 22; i++) {
        string filename = "run_" + to_string(i) + ".csv";
        remove(filename.c_str());
      }
      return;
    }
    // open file to write
    fstream out;
    out.open(buffers[0].runName, ios::out | ios::app);
    if (!out.is_open()) {
      cerr << "Failed to open file " + buffers[0].runName << endl;
      return;
    }
    out << buffers[0].emp_record.eid << "," << buffers[0].emp_record.ename
        << "," << buffers[0].emp_record.age << ","
        << buffers[0].emp_record.salary << endl;
    out.close();

    // read next of buffer[0].currentMPage
    int outPage = buffers[0].currentMPage + 1;
    cout << "-----" << endl;
    cout << "Previous Index = " << buffers[outPage].index << endl;
    fstream pickingRun;
   
    pickingRun.open(buffers[outPage].runName, ios::in);
    
    pickingRun.seekg(buffers[outPage].index);
    cout << "ERORRRRRRRRRRRRRRRRRRRRRRRRRRR \n"; 
    Records r = Grab_Emp_Record(pickingRun);
    buffers[outPage] = r;
    buffers[outPage].index = pickingRun.tellg();
    buffers[outPage].runName = "run_" + to_string(outPage - 1) + ".csv";
    pickingRun.close();
    
    // cout above
    cout << "---------------- new data buffer " << endl;
    cout << "id = " << buffers[outPage].emp_record.eid << endl;
    cout << "name = " << buffers[outPage].emp_record.ename << endl;
    cout << "next index = " << buffers[outPage].index << endl;
    cout << "file name =  " << buffers[outPage].runName << endl;
    cout << "No value = " << buffers[outPage].no_values << endl;
    int tmp;
    cout << "wait for enter: " << endl;
    // cin >> tmp;
    
  }
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
  // read file into buffer each bach each 22 line of file then sort it and
  // write to runs.csv
  string line;

  int runningNumber = 0;
  int i = 0;
  while (true) {
    Records r = Grab_Emp_Record(empin);
    if (r.no_values == -1) {
      
      Sort_Buffer(buffers, i, runningNumber);
      
      break;
    }
    // push r to buffers
    buffers[i++] = r;  // buffers[i] = r;
    // buffers.push_back(r);
    if (i == buffer_size) {
      Sort_Buffer(buffers, i, runningNumber);
      i = 0;
      //memset(buffers, 0, sizeof(buffers));
      // clear buffers to empty
    }
  }

  // 2. Use Merge_Runs() to Sort the runs of Emp relations
  Merge_Runs();

  // Please delete the temporary files (runs) after you've sorted the Emp.csv

  return 0;
}
