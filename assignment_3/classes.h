#include <string>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <bitset>
#include <fstream> // Include the necessary header file for the getline function
#include <map>
using namespace std;
#define MAX 216
class Record
{
public:
  int id, manager_id;
  std::string bio, name;

  Record(vector<std::string> fields)
  {
    id = stoi(fields[0]);
    name = fields[1];
    bio = fields[2];
    manager_id = stoi(fields[3]);
  }

  std::vector<char> serializeToString()
  {
    cout << "name: " << name << endl;
    string serialStr = to_string(id) + "$" + name + "$" + bio + "$" +
                       to_string(manager_id) + "$";
    vector<char> serial = {};

    for (int j = 0; j < serialStr.size(); j++)
    {
      serial.push_back(serialStr[j]);
    }
    // Add the record to the memory
    return serial;
  }

  void print()
  {
    cout << "\tID: " << id << "\n";
    cout << "\tNAME: " << name << "\n";
    cout << "\tBIO: " << bio << "\n";
    cout << "\tMANAGER_ID: " << manager_id << "\n";
  }
};

string toBinary(int n)
{
  if (n == 0)
    return "0";
  else if (n == 1)
    return "1";
  else if (n % 2 == 0)
    return toBinary(n / 2) + "0";
  else if (n % 2 != 0)
    return toBinary(n / 2) + "1";
}

class LinearHashIndex
{

private:
  const int BLOCK_SIZE = 4096;
  const int MAX_PAGE = 3;
  map<string, int> mp;
  vector<int> blockDirectory; // Map the least-significant-bits of h(id) to a bucket location in EmployeeIndex (e.g., the jth bucket)
                              // can scan to correct bucket using j*BLOCK_SIZE as offset (using seek function)
                              // can initialize to a size of 256 (assume that we will never have more than 256 regular (i.e., non-overflow) buckets)
  int n;                      // The number of indexes in blockDirectory currently being used
  int i;                      // The number of least-significant-bits of h(id) to check. Will need to increase i once n > 2^i
  int numRecords;             // Records currently in index. Used to test whether to increase n
  int nextFreeBlock;          // Next place to write a bucket. Should increment it by BLOCK_SIZE whenever a bucket is written to EmployeeIndex
  string fName;               // Name of index file
  int currentPageSize = 0;    // not more tehn 4096
  int currentPage = 0;        // 0,1,2

  std::ofstream file;
  std::ifstream fileReader;

  void setNextPage()
  {
    currentPage++;
    currentPageSize = 0;
    // memory = pageBuffer[currentPage];
    // setLastRecord();
  }

  int hashCode(int key, int SIZE)
  {
    return key % SIZE;
  }

  void printbinchar(char character)
  {
    char output[9];
    // itoa(character, output, 2);
    // printf("%s - %c\n", output,character);
  }

  string IdBinary = "";

  void convertToBinary(unsigned int n)
  {
    if (n / 2 != 0)
    {
      convertToBinary(n / 2);
    }
    IdBinary = IdBinary + to_string(n % 2);
    // printf("%d", n % 2);
  }

  int ni = 0;
  // Insert new record into index
  void insertRecord(Record record)
  {
    ni++;
    printf("insert_Each_data (%d)\n", ni);
    std::vector<char> r = record.serializeToString();
    convertToBinary(record.id);
    cout << IdBinary << endl;
    string NewIdBinaryAfterMod = "";
    for (int j = 0; j < IdBinary.length(); j++)
    {
      if (IdBinary.length() - j < 9)
      {
        NewIdBinaryAfterMod = NewIdBinaryAfterMod + IdBinary[j];
      }
    }
    cout << "NewIdBinaryAfterMod :" << NewIdBinaryAfterMod << endl;

    IdBinary = "";

    string indexAfterCut = NewIdBinaryAfterMod.substr(NewIdBinaryAfterMod.length() - i, NewIdBinaryAfterMod.length());
    cout << indexAfterCut << endl;
    map<string, int>::iterator it = mp.begin();
    
    // Iterate through the map and print the elements

    while (it != mp.end())
    {
      //cout << "it->first: " << it->first << ", NewIdBinaryAfterMod: " << indexAfterCut << endl;
      if (it->first == indexAfterCut)
      {
        cout << "Key: " << it->first
             << ", Value: " << it->second << endl;
      }

      ++it;
    }

    NewIdBinaryAfterMod = "";
    //----------------------------------------------------------------------
    // Insert some values into the map

    //----------------------------------------------------------------------

    // record.print();
    //  r to binary
    //  rBianry = after to binary

    // No records written to index yet
    if (numRecords == 0)
    {
      // // Initialize index with first blocks (start with 4)

      // struct DataItem *item = (struct DataItem *)malloc(sizeof(struct DataItem));

      // item->data = data;         // offset in page
      // item->key = rBianry % 216; // rBianry mod to get key
      // int hashIndex = hashCode(key);
      // // move in array until an empty or deleted cell
      // while (hashArray[hashIndex] != NULL && hashArray[hashIndex]->key != -1)
      // {
      //     // go to next cell
      //     ++hashIndex;

      //     // wrap around the table
      //     hashIndex %= SIZE;
      // }

      // hashArray[hashIndex] = item;
    }
    else
    {

      // Serialize the record and insert record to memory

      // check Size of Data + currentSizePage < Size of Total page
      if (r.size() + currentPageSize > BLOCK_SIZE)
      {
        // hashCode()
        //  DataItem x = record;
        //************** do forget to check after add that we need to to extention or not (n++) number of pages******************
        //  add Data to Page

        // change record to binary
        // add record to page use hash function(mod 216) that means have 216 slots
        // check i last digit that can fill in what page index
        // if(it mod and have free slot add it)
        // add to that page index normally
        // if not might do overflow page

        //**************** function check number of data more than 70% or some thing ****************
        // if(add and page more than 70% do extention increase n (number of page) and if effect to I number (do it))
        // n++
        // if j++
        // change n Ex. n=4 , i =2 and n++, j++, (00,01,10,11) ---> n=5 , i =3 (000,001,010,011,100)
        // check Bitfilp
        // reslot data
        // else i not ++
        // change only n -> n++
        //  check Bitfilp
        //  reslot data
        // else
        // passssss
      }
      // check Size of Data + currentSizePage > Size of Total page
      else
      {
        if (MAX_PAGE == currentPage && currentPage < 3)
        {
          // Push 3 Page to Disk
          // Reset to default
        }
        else
        { // not yet thrid page
          setNextPage();
        }
      }
    }

    // Add record to the index in the correct block, creating a overflow block if necessary

    // Take neccessary steps if capacity is reached:
    // increase n; increase i (if necessary); place records in the new bucket that may have been originally misplaced due to a bit flip
  }

public:
  LinearHashIndex(string indexFileName)
  {
    n = 4; // Start with 4 buckets in index
    i = 2; // Need 2 bits to address 4 buckets
    numRecords = 0;
    nextFreeBlock = 0;
    fName = indexFileName;

    // Create your EmployeeIndex file and write out the initial 4 buckets
    // make sure to account for the created buckets by incrementing nextFreeBlock appropriately
    for (int x = 0; x < n; x++)
    {
      string qwe = toBinary(x);
      cout << "qwe.length()  " << qwe.length() << endl;
      cout << "i  " << i << endl;
      if (qwe.length() < i)
      {
        while (qwe.length() != i)
        {
          qwe = "0" + qwe;
        }
      }
      // size_t sizeRange = i;
      // std::string binary = std::bitset<2>(x).to_string(); // to binary
      // std::cout << binary << "\n";

      // unsigned long decimal = std::bitset<2>(binary).to_ulong();
      // std::cout << decimal << "\n";
      // // 00,01,10,11 and offset
      // string asdasd = x + " ";
      // cout << "asdasdasdasdasdasda " << asdasd << endl;
      mp[qwe] = x;
    }
  }

  // Read csv file and add records to the index
  void createFromFile(string csvFName)
  {
    // Add records to the EmployeeRelation

    std::ifstream csvFile(csvFName);
    // Read the file and add records to the EmployeeRelation
    file = std::ofstream(fName, std::ios::out | std::ios::binary);
    if (!csvFile.is_open())
    {
      return;
    }
    string line;
    cout << "CREATED FILE" << endl;
    while (std::getline(csvFile, line, '\n'))
    {
      // Split the line into fields
      vector<string> fields;
      stringstream ss(line);
      string field;
      while (std::getline(ss, field, ','))
      {
        fields.push_back(field);
      }
      // cout << fields[0] << endl;
      // Create a record from the fields
      Record record(fields);
      // Insert the record into the EmployeeRelation
      insertRecord(record);
    }
    file.close();
    // Close the file
    csvFile.close();
  }
  // Given an ID, find the relevant record and print it
  Record findRecordById(int index)
  {
    cout << "Finding record with id: " << index << endl;
    // load file
    // get hash index table
    // find input in table

    // return record;
  }
};
