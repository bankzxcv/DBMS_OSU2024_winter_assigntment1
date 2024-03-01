/* This is a skeleton code for Optimized Merge Sort, you can make modifications as long as you meet
   all question requirements*/

#include <bits/stdc++.h>
#include "record_class.h"

using namespace std;

// defines how many blocks are available in the Main Memory
#define buffer_size 22

Records buffers[buffer_size]; // use this class object of size 22 as your main memory

/***You can change return type and arguments as you want.***/

// Sorting the buffers in main_memory and storing the sorted records into a temporary file (runs)
void Sort_Buffer_Emp(Records buffers[], int i, int &runningNumber)
{

    cout << "Sort_Buffer_Emp   " << i << endl;
    // Remember: You can use only [AT MOST] 22 blocks for sorting the records /
    // tuples and create the runs
    // create run_{runningNumber} file

    // create file
    string filename = "Erun_" + to_string(runningNumber++) + ".csv";
    fstream run;
    run.open(filename, ios::out | ios::app);

    sort(buffers, buffers + i, [](Records a, Records b)
         { return a.emp_record.eid < b.emp_record.eid; });
    if (!run.is_open())
    {
        cerr << "Failed to open file runs.csv" << endl;
        return;
    }

    for (int j = 0; j < i; j++)
    {
        run << buffers[j].emp_record.eid << "," << buffers[j].emp_record.ename
            << "," << buffers[j].emp_record.age << ","
            << buffers[j].emp_record.salary << endl;
    }
    run.close();
}
void Sort_Buffer_Dept(Records buffers[], int i, int &runningNumber)
{

    cout << "Sort_Buffer_Dept   " << i << endl;
    // Remember: You can use only [AT MOST] 22 blocks for sorting the records /
    // tuples and create the runs
    // create run_{runningNumber} file

    // create file
    string filename = "Drun_" + to_string(runningNumber++) + ".csv";
    fstream run;
    run.open(filename, ios::out | ios::app);

    sort(buffers, buffers + i, [](Records a, Records b)
         { return a.dept_record.managerid < b.dept_record.managerid; });
    if (!run.is_open())
    {
        cerr << "Failed to open file runs.csv" << endl;
        return;
    }

    for (int j = 0; j < i; j++)
    {
        run << buffers[j].dept_record.did << "," << buffers[j].dept_record.dname
            << "," << buffers[j].dept_record.budget << ","
            << buffers[j].dept_record.managerid << endl;
    }
    run.close();
}
// Prints out the attributes from empRecord and deptRecord when a join condition is met
// and puts it in file Join.csv
void PrintJoin()
{

    return;
}

// Use main memory to Merge and Join tuples
// which are already sorted in 'runs' of the relations Dept and Emp
void Merge_Join_Runs()
{

    // and store the Joined new tuples using PrintJoin()
    return;
}

int main()
{

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
    while (true)
    {
        Records rEmp = Grab_Emp_Record(empin);

        if (rEmp.no_values == -1)
        {

            Sort_Buffer_Emp(buffers, i, runningNumber);

            break;
        }
        // push rEmp to buffers
        buffers[i++] = rEmp; // buffers[i] = rEmp;
        // buffers.push_back(rEmp);
        if (i == buffer_size)
        {
            Sort_Buffer_Emp(buffers, i, runningNumber);
            i = 0;
            // memset(buffers, 0, sizeof(buffers));
            //  clear buffers to empty
        }
    }
    runningNumber = 0;
    i = 0;
    while (true)
    {
        Records rDept = Grab_Dept_Record(deptin);

        if (rDept.no_values == -1)
        {

            Sort_Buffer_Dept(buffers, i, runningNumber);

            break;
        }
        // push rDept to buffers
        buffers[i++] = rDept; // buffers[i] = rDept;
        // buffers.push_back(rDept);
        if (i == buffer_size)
        {
            Sort_Buffer_Dept(buffers, i, runningNumber);
            i = 0;
            // memset(buffers, 0, sizeof(buffers));
            //  clear buffers to empty
        }
    }
    // 2. Use Merge_Join_Runs() to Join the runs of Dept and Emp relations

    // Please delete the temporary files (runs) after you've joined both Emp.csv and Dept.csv

    return 0;
}
