/* This is a skeleton code for External Memory Sorting, you can make modifications as long as you meet
   all question requirements*/

#include <bits/stdc++.h>
#include "record_class.h"

using namespace std;

// defines how many blocks are available in the Main Memory
#define buffer_size 22

Records buffers[buffer_size]; // use this class object of size 22 as your main memory

/***You can change return type and arguments as you want.***/

// PASS 1
// Sorting the buffers in main_memory and storing the sorted records into a temporary file (Runs)
// void Sort_Buffer()
// {
//     ifstream empin("example.txt");

//     // Check if the file is successfully opened

//     // Remember: You can use only [AT MOST] 22 blocks for sorting the records / tuples and create the runs
//     return;
// }

// PASS 2
// Use main memory to Merge the Runs
// which are already sorted in 'runs' of the relation Emp.csv
void Merge_Runs()
{

    // and store the Sorted results of your Buffer using PrintSorted()
    return;
}

void PrintSorted()
{

    // Store in EmpSorted.csv
    return;
}

void setZero()
{
    // memset(buffers, 0, sizeof(buffers));
    // for (int i = 0; i < 10; i++)
    // {
    //     cout << buffers[i].emp_record.eid << " ";
    // }
    cout << "----------------------------" << endl;
}

void SortTest(Records arr[], int n)
{

    int i, j;
    bool swapped;
    for (i = 0; i < n - 1; i++)
    {
        swapped = false;
        for (j = 0; j < n - i - 1; j++)
        {
            if (arr[j].emp_record.eid > arr[j + 1].emp_record.eid)
            {
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }

        // If no two elements were swapped
        // by inner loop, then break
        if (swapped == false)
            break;
    }
}

int main()
{

    // Open file streams to read and write
    // Opening out the Emp.csv relation that we want to Sort
    fstream empin;
    empin.open("Emp.csv", ios::in);
    if (!empin.is_open())
    {
        cerr << "Error: Could not open the file." << endl;
        return 1; // Return error code
    }

    int set = 1;
    int test = 0;
    int count = 0;

    ofstream outfile("output.txt");

    // Check if the file is opened successfully
    if (!outfile.is_open())
    {
        cerr << "Error: Could not open the file." << endl;
        // Return error code
    }

    while (empin)
    {
        Records r = Grab_Emp_Record(empin);

        if (r.no_values != -1)
        {

            buffers[count] = r;
            if (count == 20)
            {
                buffers[count] = r;

                // do sort
                //  Sort the elements using  sort
                SortTest(buffers, buffer_size - 1); // not mergesort yet

                // Output the sorted elements

                int nub = 1;
                for (int i = 0; i < buffer_size - 1; i++)
                {
                    cout << buffers[i].emp_record.eid << endl;

                    long salary = buffers[i].emp_record.salary;
                    // Write some data to the file
                    outfile << buffers[i].emp_record.eid << "," << buffers[i].emp_record.ename << "," << buffers[i].emp_record.age << "," << salary << endl;
                    nub++;
                }

                // Close the file after writing

                cout << "count :" << count << endl;
                cout << "set :" << set << endl; // Output the line to the console

                cout << endl;

                set++;
                test++;
            }

            count++;
            if (count > 20)
            {
                count = 0;
            }
        }
    }

    // Close the file after reading
    empin.close();
    outfile.close();

    // Creating the EmpSorted.csv file where we will store our sorted results
    fstream SortOut;
    SortOut.open("EmpSorted.csv", ios::out | ios::app);

    fstream empinPass1;
    empinPass1.open("output.txt", ios::in);
    if (!empinPass1.is_open())
    {
        cerr << "Error: Could not open the file." << endl;
        return 1; // Return error code
    }
    ofstream outputPass2("outputPass2.txt");

    // Check if the file is opened successfully
    if (!outputPass2.is_open())
    {
        cerr << "Error: Could not open the file." << endl;
        // Return error code
    }
    // 1. Create runs for Emp which are sorted using Sort_Buffer()
    count = 0;
    int nubLine = 0;
    while (empinPass1)
    {
        Records r = Grab_Emp_Record(empinPass1);
        if (r.no_values != -1)
        {
            buffers[count] = r;

            // Output the line if it's the 21st line or a multiple of 21
            if (nubLine == 21 || nubLine % 21 == 0)
            {

                outputPass2 << buffers[count].emp_record.eid << "," << buffers[count].emp_record.ename << "," << buffers[count].emp_record.age << "," << (int)buffers[count].emp_record.salary << endl;

                r.print();
                cout << "------------------" << endl;
            }

            count++;
            if (count > 20)
            {
                count = 0;
            }
        }
        nubLine++;
    }
    cout << nubLine << endl;
    outputPass2.close();
    // 2. Use Merge_Runs() to Sort the runs of Emp relations

    // Please delete the temporary files (runs) after you've sorted the Emp.csv

    return 0;
}
