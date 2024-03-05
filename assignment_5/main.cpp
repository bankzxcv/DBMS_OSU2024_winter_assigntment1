/* This is a skeleton code for Optimized Merge Sort, you can make modifications as long as you meet
   all question requirements*/

#include <bits/stdc++.h>
#include "record_class.h"

using namespace std;

// defines how many blocks are available in the Main Memory
#define buffer_size 22

Records buffers[buffer_size]; // use this class object of size 22 as your main memory
Records buffers_2[buffer_size];
/***You can change return type and arguments as you want.***/
int B_EMP = 0;  // number of emp file
int B_DEPT = 0; // number of dept file
vector<string> fn_emp;
vector<string> fn_dept;
vector<fstream> v_emp;
vector<fstream> v_dept;
// Sorting the buffers in main_memory and storing the sorted records into a temporary file (runs)
void Sort_Buffer_Emp(Records buffers[], int i, int &runningNumber)
{

    cout << "Sort_Buffer_Emp   " << i << endl;
    // Remember: You can use only [AT MOST] 22 blocks for sorting the records /
    // tuples and create the runs
    // create Erun_{runningNumber} file

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
    fn_emp.push_back(filename);
    run.close();
}
void Sort_Buffer_Dept(Records buffers[], int i, int &runningNumber)
{

    cout << "Sort_Buffer_Dept   " << i << endl;
    // Remember: You can use only [AT MOST] 22 blocks for sorting the records /
    // tuples and create the runs
    // create Erun_{runningNumber} file

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
    fn_dept.push_back(filename);
    run.close();
}
// Prints out the attributes from empRecord and deptRecord when a join condition is met
// and puts it in file Join.csv
void PrintJoin()
{
}
Records joinR(Records dept, Records emp)
{
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
void openf_E(string filename)
{
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
void Merge_Runs()
{
    // set up head to buffer
    // let say buffer[0] is the Output buffer
    // buffer[1] to buffer[21] are M-1 buffers
    cout << "Merge Run" << endl;
    int count = 0;
    for (int i = 1; i < 22; i++)
    {
        fstream run;
        string stringFileName = "Erun_" + to_string(i - 1) + ".csv";
        run.open(stringFileName, ios::in);
        if (!run.is_open())
        {
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
    auto min = [](int a, int b)
    { return a < b ? a : b; };

    // define max Int
    int maxInt = numeric_limits<int>::max();

    // function min of two records
    int minIndex = min(22, count);

    while (true)
    {
        buffers[0].emp_record.eid = maxInt;
        buffers[0].no_values = -1;
        for (int i = 1; i <= minIndex; i++)
        {
            cout << "buffer[" << i << "].eid = " << buffers[i].emp_record.eid
                 << " file = " << buffers[i].runName << " index: " << buffers[i].index
                 << endl;

            if (buffers[i].no_values == -1)
            {
                break;
            }
            if (buffers[i].emp_record.eid < buffers[0].emp_record.eid)
            {
                buffers[0].emp_record = buffers[i].emp_record;
                buffers[0].currentMPage = i - 1;
                buffers[0].no_values = 1;
            }
        }

        cout << "outputBuffer value = " << buffers[0].emp_record.eid << endl;
        cout << "currentMPage = " << buffers[0].currentMPage << endl;
        if (buffers[0].no_values == -1)
        {
            cout << "no_values = -1" << endl;
            // remove all run files
            for (int i = 0; i < 22; i++)
            {
                string filename = "Erun_" + to_string(i) + ".csv";
                // remove(filename.c_str());
            }
            return;
        }
        // open file to write
        fstream out;
        out.open(buffers[0].runName, ios::out | ios::app);
        if (!out.is_open())
        {
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
        cout<< "HERE0" <<endl;
        fstream pickingRun;
        cout<< "HERE1" <<endl;
        pickingRun.open(buffers[outPage].runName, ios::in);
        cout<< "HERE2" <<endl;
        pickingRun.seekg(buffers[outPage].index);

        cout<< "HERE3" <<endl;
        Records r = Grab_Emp_Record(pickingRun);
        if (r.no_values == -1)
        {
            cout << "ASDASDASD" << endl;
            break;
        }
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

void Merge_Join_Runs()
{
    if (B_DEPT + B_EMP == 0 || B_DEPT == 0 || B_EMP == 0)
    {
        return;
    }
    Records buffers_Test[buffer_size];
    // set up head to buffer
    // let say buffer[0] is the Output buffer
    // buffer[1] to buffer[21] are M-1 buffers
    cout << "Merge_Join_Runs" << endl;
    cout << "--------------" << endl;

    // for (auto element : fn_dept)
    // {
    //     cout << element << endl;
    //     // openf_D(element);
    // }
    fstream farray_DEPT[B_DEPT];
    fstream farray_EMP[B_EMP];
    int fi = 0;

    buffers_Test[0].no_values = -1; // output

    for (auto element : fn_dept)
    {
        cout << element << endl;
        fstream run;
        run.open(element, ios::in);

        if (!run.is_open())
        {

            cerr << "Failed to open file runs.csv" << endl;
            return;
        }

        // farray_DEPT[fi] = run;
        farray_DEPT[fi].swap(run);
        fi++;
    }
    fi = 0;

    for (auto element : fn_emp)
    {
        cout << element << endl;
        fstream run;
        run.open(element, ios::in);

        if (!run.is_open())
        {

            cerr << "Failed to open file runs.csv" << endl;
            return;
        }
        // farray_EMP[fi] = run;
        farray_EMP[fi].swap(run);
        fi++;
    }
    Merge_Runs();

    fstream out;
    out.open("Join.csv", ios::out | ios::app);
    if (!out.is_open())
    {
        cerr << "Failed to open file Join.csv" << endl;
        return;
    }

    int nub_D = 1;
    for (int i = 0; i < B_DEPT; i++)
    {

        while (true)
        {
            if (nub_D == 22)
            {
            }
            Records rD = Grab_Dept_Record(farray_DEPT[i]);
            cout << "nub_D " << nub_D << endl;
            rD.print();

            if (rD.no_values == -1)
            {
                break;
            }
            else
            {
                buffers_Test[nub_D] = rD;

                nub_D++;
            }
        }
    }

    int nub_E = nub_D + 1;

    fstream testttttE;
    string fnammmm = "EmpSorted.csv";
    testttttE.open(fnammmm, ios::in);
    if (!testttttE.is_open())
    {

        cerr << "Failed to open file runs.csv" << endl;
        return;
    }
    int dfsfsjhf = 1;
    int nub_run_dept = 1; // 1-nub_D

    // while (true)
    // {

    //     Records rE_empS = Grab_Emp_Record(testttttE);
    //     if (rE_empS.emp_record.eid == NULL)
    //     {

    //         cout << "NULL" << endl;
    //     }
    //     if (rE_empS.no_values == -1)
    //     {
    //         break;
    //     }
    //     else
    //     {

    //         for (int i = 0; i < nub_E; i++)
    //         {
    //             // if (buffers_Test[i].dept_record.managerid > buffers_Test[nub_E].emp_record.eid)
    //             // {
    //             //     //get new record
    //             //     continue;
    //             // }

    //             buffers_Test[nub_E] = rE_empS;
    //             cout << "----------------------------------------------------------------" << dfsfsjhf << "\n";
    //             cout << "buffers_Test[nub_E].emp_record.eid - " << buffers_Test[nub_E].emp_record.eid << "\n";
    //             cout << "buffers_Test[1].dept_record.managerid - " << buffers_Test[i].dept_record.managerid << "\n";
    //             if (buffers_Test[nub_E].emp_record.eid == buffers_Test[i].dept_record.managerid)
    //             {
    //                 cout << "_AASD_AD_A_SDAS_DA_SD_ASD_A_SD_ASD_AS_D_ASD_ASD_SA__SA \n";
    //                 buffers_Test[0] = joinR(buffers_Test[i], buffers_Test[nub_E]); // this line

    //                 out << buffers_Test[0].emp_record.eid << ","
    //                     << buffers_Test[0].emp_record.ename << ","
    //                     << buffers_Test[0].emp_record.age << ","
    //                     << buffers_Test[0].emp_record.salary << ","
    //                     << buffers_Test[0].dept_record.did << ","
    //                     << buffers_Test[0].dept_record.dname << ","
    //                     << buffers_Test[0].dept_record.budget << ","
    //                     << buffers_Test[0].dept_record.managerid << endl;
    //             }
    //             else
    //             {

    //             }
    //         }
    //     }
    //     dfsfsjhf++;
    // }
    int mark = NULL;
    int nub_D_S = 1;

    Records testttt = Grab_Emp_Record(testttttE);

    if (testttt.emp_record.eid == NULL)
    {
    }
    if (testttt.no_values == -1)
    {
        return;
    }
    buffers_Test[nub_E] = testttt;
    do
    {
        cout << "asd" << endl;
        if (mark == NULL)
        {
            cout << "mark = NULL" << endl;
            while (buffers_Test[nub_E].emp_record.eid < buffers_Test[1].dept_record.managerid)
            {

                Records sdasdasdasd = Grab_Emp_Record(testttttE);
                if (sdasdasdasd.emp_record.eid == NULL)
                {
                }
                if (sdasdasdasd.no_values == -1)
                {
                    return;
                }
                buffers_Test[nub_E] = sdasdasdasd;
            }
            while (buffers_Test[nub_E].emp_record.eid > buffers_Test[nub_D_S].dept_record.managerid)
            {
                nub_D_S++;
                if (nub_D_S == nub_E)
                {
                    //
                    cout << "nub_D_S == nub_E" << endl;
                    return;
                }
            }
            mark = nub_D_S;
        }
        if (buffers_Test[nub_E].emp_record.eid == buffers_Test[nub_D_S].dept_record.managerid)
        {
            cout << "match" << endl;
            buffers_Test[0] = joinR(buffers_Test[nub_D_S], buffers_Test[nub_E]); // this line

            out << buffers_Test[0].emp_record.eid << ","
                << buffers_Test[0].emp_record.ename << ","
                << buffers_Test[0].emp_record.age << ","
                << buffers_Test[0].emp_record.salary << ","
                << buffers_Test[0].dept_record.did << ","
                << buffers_Test[0].dept_record.dname << ","
                << buffers_Test[0].dept_record.budget << ","
                << buffers_Test[0].dept_record.managerid << endl;

            nub_D_S++;
        }
        else
        {
            nub_D_S = mark;
            mark = NULL;
            Records sdasdasdasd = Grab_Emp_Record(testttttE);
            if (sdasdasdasd.emp_record.eid == NULL)
            {
            }
            if (sdasdasdasd.no_values == -1)
            {
                return;
            }
            buffers_Test[nub_E] = sdasdasdasd;
        }

    } while (true);

    out.close();
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
            B_EMP++;
            break;
        }
        // push rEmp to buffers
        buffers[i++] = rEmp; // buffers[i] = rEmp;
        // buffers.push_back(rEmp);
        if (i == buffer_size)
        {
            Sort_Buffer_Emp(buffers, i, runningNumber);
            i = 0;
            B_EMP++;
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
            B_DEPT++;
            break;
        }
        // push rDept to buffers
        buffers[i++] = rDept; // buffers[i] = rDept;
        // buffers.push_back(rDept);
        if (i == buffer_size)
        {
            Sort_Buffer_Dept(buffers, i, runningNumber);
            i = 0;
            B_DEPT++;
            // memset(buffers, 0, sizeof(buffers));
            //  clear buffers to empty
        }
    }

    // clear
    cout << "---------------clear--------------------------- \n";

    Merge_Join_Runs();

    // 2. Use Merge_Join_Runs() to Join the runs of Dept and Emp relations
    // Merge_Join_Runs();

    // for (int i = 0; i < buffer_size; i++)
    // {
    //     fstream deptin;
    //     string stringFileName = "Drun_" + to_string(i) + ".csv";
    //     deptin.open(stringFileName, ios::in);
    //     if (!deptin.is_open())
    //     {
    //         cerr << "Failed to open file " + stringFileName << endl;
    //     }
    //     Records rD = Grab_Dept_Record(deptin);
    //     buffers[i] = rD;
    //     rD.print();

    //     for (int j = 0; j < buffer_size; j++)
    //     {

    //         fstream empin;
    //         string stringFileName = "Erun_" + to_string(j) + ".csv";
    //         empin.open(stringFileName, ios::in);
    //         if (!empin.is_open())
    //         {
    //             cerr << "Failed to open file " + stringFileName << endl;
    //             continue;
    //         }
    //         Records rE = Grab_Emp_Record(empin);
    //         rE.print();
    //         if (rE.emp_record.eid = rD.dept_record.managerid)
    //         {
    //             cout << "Match" << endl;
    //         }
    //         empin.close();
    //     }
    //     deptin.close();
    // }
    // Please delete the temporary files (runs) after you've joined both Emp.csv and Dept.csv
    for (int i = 0; i < buffer_size; i++)
    {
        string filename = "Erun_" + to_string(i) + ".csv";
        // remove(filename.c_str());
        filename = "Drun_" + to_string(i) + ".csv";
        // remove(filename.c_str());
    }

    cout << "ENDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD \n";
    return 0;
}