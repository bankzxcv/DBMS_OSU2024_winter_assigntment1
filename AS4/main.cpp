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
    // Records r = Grab_Emp_Record(empin);
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
            if (count == buffer_size - 2)
            {
                buffers[count] = r;
                // set Zero

                // do sort
                //  Sort the elements using  sort
                SortTest(buffers, buffer_size); // not mergesort yet

                // Output the sorted elements
                // cout << "After sort => ";
                int nub = 1;
                for (int i = 0; i < buffer_size - 1; i++)
                {
                    ostringstream str1;

                    long salary = buffers[i].emp_record.salary;
                    // Write some data to the file
                    outfile << buffers[i].emp_record.eid << "," << buffers[i].emp_record.ename << "," << buffers[i].emp_record.age << "," << salary << endl;
                    // cout << element.emp_record.eid << " nub :" << nub << endl;
                    nub++;
                }

                // Close the file after writing

                cout << "count :" << count << endl
                     << "set :" << set << endl; // Output the line to the console

                cout << endl
                     << endl;
                set++;
                test++;
            }

            // buffers[count].print();
            // outfile << buffers[count].emp_record.ename << endl;

            count++;
            if (count > buffer_size - 2)
            {
                count = 0;
            }
        }
        else
        {
            cout << "--------------------------" << endl;
        }
    }
    // if (count != 0)
    // {

    //     for (size_t i = 0; i == count; i++)
    //     {
    //         buffers[i];
    //     }
    // }
    cout << "test :" << test << endl; // Output the line to the console

    // Close the file after reading
    empin.close();
    outfile.close();

    // Creating the EmpSorted.csv file where we will store our sorted results
    fstream SortOut;
    SortOut.open("EmpSorted.csv", ios::out | ios::app);

    // 1. Create runs for Emp which are sorted using Sort_Buffer()

    // 2. Use Merge_Runs() to Sort the runs of Emp relations

    // Please delete the temporary files (runs) after you've sorted the Emp.csv

    return 0;
}
