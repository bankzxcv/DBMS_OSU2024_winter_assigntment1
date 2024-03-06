/* This is a skeleton code for Optimized Merge Sort, you can make modifications
   as long as you meet all question requirements*/

#include <bits/stdc++.h>
#include "record_class.h"

using namespace std;

// defines how many blocks are available in the Main Memory
#define buffer_size 22

Records buffers[buffer_size];  // use this class object of size 22 as your main
                               // memory
/***You can change return type and arguments as you want.***/
int B_EMP = 0;   // number of emp file
int B_DEPT = 0;  // number of dept file
vector<string> fn_emp;
vector<string> fn_dept;

void Merge_Runs_Emp() {
  cout << "Merge Run" << endl;
  int count = 0;
  int countItem = fn_emp.size() < 22 ? fn_emp.size() : 21;
  for (int i = 0; i < countItem; i++) {
    fstream run;
    // string stringFileName = "run_" + to_string(i - 1) + ".csv";
    string stringFileName = fn_emp[i];
    run.open(stringFileName, ios::in);
    if (!run.is_open()) {
      cerr << "Failed to open file " + stringFileName << endl;
      continue;
    }
    Records r = Grab_Emp_Record(run);
    buffers[i + 1] = r;
    buffers[i + 1].index = run.tellg();
    buffers[i + 1].runName = stringFileName;
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
      for (int i = 0; i < 22; i++) {
        string filename = "Erun_" + to_string(i) + ".csv";
        remove(filename.c_str());
      }
      fn_emp.clear();
      fn_emp.push_back("EmpSorted.csv");
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
    Records r = Grab_Emp_Record(pickingRun);
    buffers[outPage] = r;
    buffers[outPage].index = pickingRun.tellg();
    buffers[outPage].runName = "Erun_" + to_string(outPage - 1) + ".csv";
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

void Merge_Runs_Dept() {
  cout << "Merge Run" << endl;
  int count = 0;
  int countItem = fn_dept.size() < 22 ? fn_dept.size() : 21;
  for (int i = 0; i < countItem; i++) {
    fstream run;
    // string stringFileName = "run_" + to_string(i - 1) + ".csv";
    string stringFileName = fn_dept[i];

    run.open(stringFileName, ios::in);
    if (!run.is_open()) {
      cerr << "Failed to open file " + stringFileName << endl;
      continue;
    }
    Records r = Grab_Emp_Record(run);
    buffers[i + 1] = r;
    buffers[i + 1].index = run.tellg();
    buffers[i + 1].runName = stringFileName;
    run.close();
    count++;
  }
  cout << "having " << count << " runs" << endl;

  buffers[0].no_values = -1;
  buffers[0].runName = "DeptSorted.csv";
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
      for (int i = 0; i < 22; i++) {
        string filename = "Drun_" + to_string(i) + ".csv";
        remove(filename.c_str());
      }
      fn_dept.clear();
      fn_dept.push_back("DeptSorted.csv");
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
    Records r = Grab_Emp_Record(pickingRun);
    buffers[outPage] = r;
    buffers[outPage].index = pickingRun.tellg();
    buffers[outPage].runName = "Drun_" + to_string(outPage - 1) + ".csv";
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
// Sorting the buffers in main_memory and storing the sorted records into a
// temporary file (runs)
void Sort_Buffer_Emp(Records buffers[], int i, int &runningNumber) {
  cout << "Sort_Buffer_Emp   " << i << endl;
  // Remember: You can use only [AT MOST] 22 blocks for sorting the records /
  // tuples and create the runs
  // create Erun_{runningNumber} file

  // create file
  string filename = "Erun_" + to_string(runningNumber++) + ".csv";
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
  fn_emp.push_back(filename);
  run.close();
}
void Sort_Buffer_Dept(Records buffers[], int i, int &runningNumber) {
  cout << "Sort_Buffer_Dept   " << i << endl;
  // Remember: You can use only [AT MOST] 22 blocks for sorting the records /
  // tuples and create the runs
  // create Erun_{runningNumber} file

  // create file
  string filename = "Drun_" + to_string(runningNumber++) + ".csv";
  fstream run;
  run.open(filename, ios::out | ios::app);

  sort(buffers, buffers + i, [](Records a, Records b) {
    return a.dept_record.managerid < b.dept_record.managerid;
  });
  if (!run.is_open()) {
    cerr << "Failed to open file runs.csv" << endl;
    return;
  }

  for (int j = 0; j < i; j++) {
    run << buffers[j].dept_record.did << "," << buffers[j].dept_record.dname
        << "," << buffers[j].dept_record.budget << ","
        << buffers[j].dept_record.managerid << endl;
  }
  fn_dept.push_back(filename);
  run.close();
}
// Prints out the attributes from empRecord and deptRecord when a join condition
// is met and puts it in file Join.csv
void PrintJoin() {}
Records joinR(Records dept, Records emp) {
  Records ans;
  ans.dept_record.budget = dept.dept_record.budget;
  ans.dept_record.did = dept.dept_record.did;
  ans.dept_record.dname = dept.dept_record.dname;
  ans.dept_record.managerid = dept.dept_record.managerid;
  ans.emp_record.age = emp.emp_record.age;
  ans.emp_record.ename = emp.emp_record.ename;
  ans.emp_record.salary = emp.emp_record.salary;
  ans.emp_record.eid = emp.emp_record.eid;

  return ans;
}
// Use main memory to Merge and Join tuples
// which are already sorted in 'runs' of the relations Dept and Emp
void openf_E(string filename) {
  // fstream run;
  // run.open(filename, ios::out | ios::app);

  // if (!run.is_open())
  // {
  //     cout << "whyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy \n";
  //     cerr << "Failed to open file runs.csv" << endl;
  //     return;
  // }
  // // cout << "gofffffffffffffffff \n";
  // v_emp.push_back(run);
}
void Merge_Runs() {
  cout << "START MERGE ERUN" << endl;
  int x = 0;
  // set up head to buffer
  // let say buffer[0] is the Output buffer
  // buffer[1] to buffer[21] are M-1 buffers
  cout << "Merge Run" << endl;
  int count = 0;
  for (int i = 1; i < 22; i++) {
    fstream run;
    string stringFileName = "Erun_" + to_string(i - 1) + ".csv";
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
  //   cout << "RIGHT HERE" << endl;

  // function min of two integers
  auto min = [](int a, int b) { return a < b ? a : b; };

  // define max Int
  int maxInt = numeric_limits<int>::max();

  // function min of two records
  int minIndex = min(22, count);

  while (true) {
    cout << "x =============" << ++x << endl;
    buffers[0].emp_record.eid = maxInt;
    buffers[0].no_values = -1;
    for (int i = 1; i <= minIndex; i++) {
      cout << "buffer[" << i << "].eid = " << buffers[i].emp_record.eid
           << " file = " << buffers[i].runName << " index: " << buffers[i].index
           << endl;

      if (buffers[i].no_values == -1) {
        cout << "no_values = -1 --------- " << endl;
        break;
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
      for (int i = 0; i < 22; i++) {
        string filename = "Erun_" + to_string(i) + ".csv";
        // remove(filename.c_str());
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
    // cout << "-----" << endl;
    // cout << "Previous Index = " << buffers[outPage].index << endl;
    // cout << "HERE0" << endl;
    fstream pickingRun;
    // cout << "HERE1" << endl;
    pickingRun.open(buffers[outPage].runName, ios::in);
    // cout << "HERE2" << endl;
    pickingRun.seekg(buffers[outPage].index);

    // cout << "HERE3" << endl;
    Records r = Grab_Emp_Record(pickingRun);
    // if (r.no_values == -1) {
    //   cout << "ASDASDASD" << endl;
    //   break;
    // }
    buffers[outPage] = r;
    buffers[outPage].index = pickingRun.tellg();
    buffers[outPage].runName = "Erun_" + to_string(outPage - 1) + ".csv";
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

void Merge_Join_Runs() {
  // loop though fn_emp
  for (auto i : fn_emp) {
    cout << "fn_emp: " << i << endl;
  }
  if (B_DEPT + B_EMP == 0 || B_DEPT == 0 || B_EMP == 0) {
    return;
  }
  Records buffers_Test[buffer_size];
  // set up head to buffer
  // let say buffer[0] is the Output buffer
  // buffer[1] to buffer[21] are M-1 buffers
  cout << "Merge_Join_Runs" << endl;
  cout << "--------------" << endl;
  // fn_dept, fn_emp
  int empFiles = fn_emp.size();
  int deptFiles = fn_dept.size();
  cout << "empFiles = " << empFiles << " deptFiles = " << deptFiles << endl;

  fstream out;
  out.open("Join.csv", ios::out | ios::app);
  if (!out.is_open()) {
    cerr << "Failed to open file Join.csv" << endl;
    return;
  }
  for (int i = 0; i < empFiles; i++) {
    cout << "fn_emp " << fn_emp[i] << endl;
    fstream run;
    // string stringFileName = "Erun_" + to_string(i) + ".csv";
    string stringFileName = fn_emp[i];
    cout << stringFileName << endl;
    run.open(stringFileName, ios::in);
    if (!run.is_open()) {
      cout << "HERE1" << endl;
      cerr << "Failed to open file " + stringFileName << endl;
      continue;
    }
    Records r = Grab_Emp_Record(run);
    buffers[i + 1] = r;
    buffers[i + 1].index = run.tellg();
    buffers[i + 1].runName = stringFileName;
    run.close();
  }

  for (int i = 0; i < deptFiles; i++) {
    cout << "fn_dept " << fn_dept[i] << endl;
    fstream run;
    // string stringFileName = "Drun_" + to_string(i) + ".csv";
    string stringFileName = fn_dept[i];
    cout << stringFileName << endl;
    run.open(stringFileName, ios::in);
    if (!run.is_open()) {
      cout << "HERE2" << endl;
      cerr << "Failed to open file " + stringFileName << endl;
      continue;
    }
    Records r = Grab_Dept_Record(run);
    buffers[i + empFiles + 1] = r;
    buffers[i + empFiles + 1].index = run.tellg();
    buffers[i + empFiles + 1].runName = stringFileName;
    run.close();
  }

  buffers[0].runName = "Join.csv";

  // function min of two integers
  auto min = [](int a, int b) { return a < b ? a : b; };
  int maxInt = numeric_limits<int>::max();

  int isMark = 0;

  while (true) {
    for (int i = 0; i < empFiles; i++) {
      int pos = i + 1;
      cout << "buffer[" << pos << "].eid = " << buffers[pos].emp_record.eid
           << " file = " << buffers[pos].runName
           << " index: " << buffers[pos].index << endl;
    }
    for (int i = 0; i < deptFiles; i++) {
      int pos = i + empFiles + 1;
      cout << "buffer[" << pos
           << "].managerid = " << buffers[pos].dept_record.managerid
           << " file = " << buffers[pos].runName
           << " index: " << buffers[pos].index << endl;
    }

    int minEmpPosBuffer = 1;
    for (int i = 1; i < empFiles; i++) {
      int pos = i + 1;
      int a = buffers[pos].emp_record.eid;
      int b = buffers[pos - 1].emp_record.eid;
      if (buffers[pos].index == -1 && buffers[pos - 1].index == -1) {
        continue;
      }
      if (buffers[pos].index == -1 && buffers[pos - 1].index != -1) {
        minEmpPosBuffer = pos - 1;
        continue;
      }
      if (buffers[pos].index != -1 && buffers[pos - 1].index == -1) {
        minEmpPosBuffer = pos;
        continue;
      }
      if (a < b) {
        minEmpPosBuffer = pos;
      }
    }

    int minDeptPosBuffer = empFiles + 1;
    for (int i = 1; i < deptFiles; i++) {
      int pos = i + empFiles + 1;
      int a = buffers[pos].dept_record.managerid;
      int b = buffers[pos - 1].dept_record.managerid;
      if (a < b) {
        minDeptPosBuffer = pos;
      }
    }

    if (buffers[minEmpPosBuffer].no_values == -1 ||
        buffers[minDeptPosBuffer].no_values == -1) {
      break;
    }

    if (buffers[minEmpPosBuffer].emp_record.eid <
        buffers[minDeptPosBuffer].dept_record.managerid) {
      // shift emp
      cout << "eid < managerid" << buffers[minEmpPosBuffer].emp_record.eid
           << buffers[minDeptPosBuffer].dept_record.managerid << endl;
      int pos = minEmpPosBuffer;
      int filePos = pos - 1;
      fstream run;
      // string stringFileName = "Erun_" + to_string(filePos) + ".csv";
      string stringFileName = buffers[pos].runName;
      run.open(stringFileName, ios::in);
      run.seekg(buffers[pos].index);
      Records r = Grab_Emp_Record(run);
      buffers[pos] = r;
      buffers[pos].index = run.tellg();
      buffers[pos].runName = stringFileName;
      run.close();
      continue;
    }

    if (buffers[minDeptPosBuffer].index == -1) {
      break;
    }

    if (buffers[minEmpPosBuffer].emp_record.eid >
        buffers[minDeptPosBuffer].dept_record.managerid) {
      // shift dept
      cout << "eid > managerid" << buffers[minEmpPosBuffer].emp_record.eid
           << buffers[minDeptPosBuffer].dept_record.managerid << endl;
      int pos = minDeptPosBuffer;
      int filePos = pos - empFiles - 1;
      fstream run;
      string stringFileName = "Drun_" + to_string(filePos) + ".csv";
      run.open(stringFileName, ios::in);
      run.seekg(buffers[pos].index);
      Records r = Grab_Dept_Record(run);
      buffers[pos] = r;
      buffers[pos].index = run.tellg();
      buffers[pos].runName = stringFileName;
      run.close();
      continue;
    }

    if (buffers[minEmpPosBuffer].emp_record.eid ==
        buffers[minDeptPosBuffer].dept_record.managerid) {
      isMark = 1;
      cout << "Joining " << endl;
      Records r = joinR(buffers[minDeptPosBuffer], buffers[minEmpPosBuffer]);
      buffers[0].emp_record = r.emp_record;
      buffers[0].dept_record = r.dept_record;
      string temp = to_string(buffers[0].emp_record.eid) + "," +
                    buffers[0].emp_record.ename + "," +
                    to_string(buffers[0].emp_record.age) + "," +
                    to_string(buffers[0].emp_record.salary) + "," +
                    to_string(buffers[0].dept_record.did) + "," +
                    buffers[0].dept_record.dname + "," +
                    to_string(buffers[0].dept_record.budget) + "," +
                    to_string(buffers[0].dept_record.managerid);
      fstream out;
      out.open(buffers[0].runName, ios::out | ios::app);
      out << temp << endl;
      // ship dept

      fstream run;
      string stringFileName = buffers[minDeptPosBuffer].runName;
      run.open(stringFileName, ios::in);
      run.seekg(buffers[minDeptPosBuffer].index);
      Records reading = Grab_Dept_Record(run);
      buffers[minDeptPosBuffer].dept_record = reading.dept_record;
      buffers[minDeptPosBuffer].index = run.tellg();
      run.close();
    }
  }

  // define buffers[0] as an output buffer
  // define buffers[1] to buffers[21] as M-1 buffers
  out.close();
}

int main() {
  // Open file streams to read and write
  // Opening out two relations Emp.csv and Dept.csv which we want to join
  fstream empin;
  fstream deptin;
  empin.open("Emp.csv", ios::in);
  deptin.open("Dept.csv", ios::in);

  // Creating the Join.csv file where we will store our joined results
  fstream joinout;
  joinout.open("Join.csv", ios::out | ios::app);

  // 1. Create runs for Dept and Emp which are sorted using Sort_Buffer()

  string line;

  int runningNumber = 0;
  int i = 0;
  while (true) {
    Records rEmp = Grab_Emp_Record(empin);

    if (rEmp.no_values == -1) {
      Sort_Buffer_Emp(buffers, i, runningNumber);
      B_EMP++;
      break;
    }
    // push rEmp to buffers
    buffers[i++] = rEmp;  // buffers[i] = rEmp;
    // buffers.push_back(rEmp);
    if (i == buffer_size) {
      Sort_Buffer_Emp(buffers, i, runningNumber);
      i = 0;
      B_EMP++;
      // memset(buffers, 0, sizeof(buffers));
      //  clear buffers to empty
    }
  }
  runningNumber = 0;
  i = 0;
  while (true) {
    Records rDept = Grab_Dept_Record(deptin);

    if (rDept.no_values == -1) {
      Sort_Buffer_Dept(buffers, i, runningNumber);
      B_DEPT++;
      break;
    }
    // push rDept to buffers
    buffers[i++] = rDept;  // buffers[i] = rDept;
    // buffers.push_back(rDept);
    if (i == buffer_size) {
      Sort_Buffer_Dept(buffers, i, runningNumber);
      i = 0;
      B_DEPT++;
      // memset(buffers, 0, sizeof(buffers));
      //  clear buffers to empty
    }
  }

  // 2. Use Merge_Join_Runs() to Join the runs of Dept and Emp relations

  // if size of dept + emp > 22
  if (fn_dept.size() + fn_emp.size() > 22) {
    Merge_Runs_Dept();
  }
  if (fn_dept.size() + fn_emp.size() > 22) {
    Merge_Runs_Emp();
  }

  Merge_Join_Runs();

  // remove
  for (int i = 0; i < fn_emp.size(); i++) {
    remove(fn_emp[i].c_str());
  }
  for (int i = 0; i < fn_dept.size(); i++) {
    remove(fn_dept[i].c_str());
  }
  return 0;
}