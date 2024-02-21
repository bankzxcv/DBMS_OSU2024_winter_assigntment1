#include <string>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <sstream>
#include <bitset>
#include <fstream> // Include the necessary header file for the getline function
#include <map>
#include <cstdio>
#include <cstdlib>
#include <list>
#define intSize sizeof(int) // int size
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
  vector<int>
      blockDirectory;      // Map the least-significant-bits of h(id) to a bucket
                           // location in EmployeeIndex (e.g., the jth bucket) can
                           // scan to correct bucket using j*BLOCK_SIZE as offset
                           // (using seek function) can initialize to a size of 256
                           // (assume that we will never have more than 256 regular
                           // (i.e., non-overflow) buckets)
  int n;                   // The number of indexes in blockDirectory currently being used
  int i;                   // The number of least-significant-bits of h(id) to check. Will need
                           // to increase i once n > 2^i
  int numRecords;          // Records currently in index. Used to test whether to
                           // increase n
  int nextFreeBlock;       // Next place to write a bucket. Should increment it by
                           // BLOCK_SIZE whenever a bucket is written to
                           // EmployeeIndex
  string fName;            // Name of index file
  int currentPageSize = 0; // not more tehn 4096
  int currentPage = 0;     // 0,1,2

  std::ofstream file;
  std::ifstream fileReader;

  void setNextPage()
  {
    currentPage++;
    currentPageSize = 0;
    // memory = pageBuffer[currentPage];
    // setLastRecord();
  }

  int hashCode(int key, int SIZE) { return key % SIZE; }

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
    //-------------------------------------- mod , cut string
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

    string indexAfterCut = NewIdBinaryAfterMod.substr(
        NewIdBinaryAfterMod.length() - i, NewIdBinaryAfterMod.length());
    cout << indexAfterCut << endl;
    // cout << "sdfsfsdfsdfsdfsdfsdf" << endl;
    //--------------------------------------
      // if (true) // <total_number_of_bytes_stored> / (4KB*<number_of_non_overflow_pages>
      // {
      //   list<string> testlist;
      //   list<int> testlist2;

      //   n++;
      //   if (n > 2 ^ i)
      //   {
      //     i++;

      //     int q = 0;
      //     for (auto itr = mp.begin(); itr != mp.end(); ++itr)
      //     {

      //       mp["0" + itr->first] = itr->second;
      //       //cout << "sdfsfsdfsdfsdfsdfsdf" << endl;
      //       string s = itr->first;
      //       testlist.push_back(s);

      //       testlist2.push_back(itr->second);
      //       q++;
      //     }

      //     mp[indexAfterCut] = n;

      //     // string remove = [];
      //     // how to delete the old one?
      //     int jj = 0;
      //     for (auto i : testlist)
      //     {
      //       auto l_front = testlist2.begin();

      //       std::advance(l_front, jj);

      //       int ans = *l_front;
      //       mp["0" + i] = ans;
      //       mp.erase(i);
      //       jj++;
      //     }
      //   }
      // }
    map<string, int>::iterator it = mp.begin();

    // Iterate through the map and print the elements

    while (it != mp.end())
    {
      // cout << "it->first: " << it->first << ", NewIdBinaryAfterMod: " <<
      // indexAfterCut << endl;
      // if (it->first == indexAfterCut)
      // {
      //   cout << "Key: " << it->first << ", Value: " << it->second << endl;
      // }
      cout << "Key: " << it->first << ", Value: " << it->second << endl;

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

      // struct DataItem *item = (struct DataItem *)malloc(sizeof(struct
      // DataItem));

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
        //************** do forget to check after add that we need to to
        // extention or not (n++) number of pages******************
        //  add Data to Page

        // change record to binary
        // add record to page use hash function(mod 216) that means have 216
        // slots check i last digit that can fill in what page index if(it mod
        // and have free slot add it) add to that page index normally if not
        // might do overflow page

        //**************** function check number of data more than 70% or some
        // thing ****************
        // if(add and page more than 70% do extention increase n (number of
        // page) and if effect to I number (do it)) n++ if j++ change n Ex. n=4
        // , i =2 and n++, j++, (00,01,10,11) ---> n=5 , i =3
        // (000,001,010,011,100) check Bitfilp reslot data else i not ++ change
        // only n -> n++
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

    // Add record to the index in the correct block, creating a overflow block
    // if necessary

    // Take neccessary steps if capacity is reached:
    // increase n; increase i (if necessary); place records in the new bucket
    // that may have been originally misplaced due to a bit flip
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
    // make sure to account for the created buckets by incrementing
    // nextFreeBlock appropriately
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

class StorageBufferManager
{
private:
  const int BLOCK_SIZE = 4096; // initialize the  block size allowed in main
  const int MAX_PAGE = 3;
  // You may declare variables based on your need
  int numRecords = 0;

  // pointer of page of memory with having 4096 bytes each
  // memory that store the variable-length records as unsigned byte array

  // create array of pointers to the last record in each page

  unsigned char *pageBuffer[4];

  unsigned char *memory;
  int *lastRecord;
  int *recordCount;

  bool isWritten = false;
  int targetId = NULL;
  int isFound = 0;

  // static_cast<unsigned char*>(std::malloc(BLOCK_SIZE * 3));
  // Create a file to store the records
  string fileName;
  std::ofstream file;
  std::ifstream fileReader;

  // create number of Position in each pages
  // create a current page number to show that what is the current page
  int currentPage = 0; // page 0, 1, 2 due to 3 pages possible
  int currentLength = 0;

  // Insert new record
  void setNextPage()
  {
    currentPage++;
    currentLength = 0;
    memory = pageBuffer[currentPage];
    setLastRecord();
  }

  void setLastRecord()
  {
    lastRecord = (int *)(memory + BLOCK_SIZE - sizeof(int));
    recordCount = (int *)(memory + BLOCK_SIZE - sizeof(int) * 2);
    *lastRecord = 0;
    *recordCount = 0;
  }

  void initializeMemory()
  {
    // Initialize the memory
    for (int i = 0; i < MAX_PAGE + 1; i++)
    {
      // free(pageBuffer[i]);
      pageBuffer[i] = static_cast<unsigned char *>(std::malloc(BLOCK_SIZE));
    }
    memory = pageBuffer[currentPage];
    setLastRecord();
  }

  void insertToMemory(std::vector<char> record)
  {
    size_t recordLen = record.size();
    for (int i = 0; i < recordLen; i++)
    {
      memory[currentLength + i] = record[i];
    }

    currentLength += recordLen;

    unsigned char *tmp = (unsigned char *)malloc(recordLen);
    for (int i = 0; i < recordLen; i++)
    {
      tmp[i] = memory[currentLength - recordLen + i];
    }

    *recordCount += 1;
    // save the free space
    memcpy(memory + BLOCK_SIZE - intSize, &currentLength, intSize);

    // save the record count
    // update the last slot in page which is the free position
    // update the length of the record to slot
    memcpy(memory + BLOCK_SIZE - intSize, &currentLength, intSize);

    // update record count to slot
    memcpy(memory + BLOCK_SIZE - intSize * 2, recordCount, intSize);

    // update the length of the block to slot
    int lengthOfBlock = (int)record.size();
    memcpy(memory + BLOCK_SIZE - intSize * 2 - intSize * (*recordCount),
           &lengthOfBlock, intSize);

    int *val = (int *)(memory + BLOCK_SIZE - intSize);
    int *val2 = (int *)(memory + BLOCK_SIZE - intSize * 2);
    int *val3 =
        (int *)(memory + BLOCK_SIZE - intSize * 2 - intSize * (*recordCount));
    // cout << "VALUE =" << *val << " " << *val2 << " " << *val3 << endl;
  }

  void insertRecord(Record record)
  {
    if (numRecords == 0)
    {
      initializeMemory();
    }
    numRecords++;
    isWritten = false;

    // Check if the current page is full

    // Serialize the record and insert record to memory
    std::vector<char> r = record.serializeToString();
    if (r.size() + currentLength >
        BLOCK_SIZE - sizeof(int) * (3 + (*recordCount)))
    {
      cout << numRecords << " " << r.size() << " " << currentLength << " "
           << BLOCK_SIZE - sizeof(int) * (3 + (*recordCount)) << endl;
      setNextPage();
    }

    if (currentPage == MAX_PAGE)
    {
      cout << "Write Pages buffer to file" << endl;
      writePageBufferToFile();
      isWritten = true;
      clearPages();
    }
    insertToMemory(r);
  }

  void clearPages()
  {
    // Clear the page
    // Set the page to 0

    currentPage = 0;

    // for (int i = 0; i < MAX_PAGE+1; i++)
    // {
    //   free(pageBuffer);
    // }
    initializeMemory();
  }

public:
  StorageBufferManager(string NewFileName)
  {
    // initialize your variables
    fileName = NewFileName;
  }

  // loop print value in pageBuffer[2]

  void writePageBufferToFile()
  {
    for (int i = 0; i < MAX_PAGE; i++)
    {
      cout << "Writing to file at Page " << i + 1 << endl;
      file.write((char *)(pageBuffer[i]), BLOCK_SIZE);
    }
  }

  // Read csv file (Employee.csv) and add records to the (EmployeeRelation)
  void createFromFile(string csvFName)
  {
    // Add records to the EmployeeRelation
    std::ifstream csvFile(csvFName);
    // Read the file and add records to the EmployeeRelation
    file = std::ofstream(fileName, std::ios::out | std::ios::binary);
    if (!csvFile.is_open())
    {
      return;
    }
    string line;
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

    if (isWritten == false)
    {
      cout << "Write Pages buffer to file...." << endl;
      writePageBufferToFile();
      isWritten = true;
    }

    file.close();
    // Close the file
    csvFile.close();
    clearPages();
  }

  void findValue(int pgCount, int target, unsigned char *pageBufferTmp)
  {
    for (int n = 0; n < pgCount; n++)
    {
      // cout << "PAGE COUNT: " << pgCount << " " << n << endl;
      memory = pageBufferTmp;
      int *val = (int *)(memory + BLOCK_SIZE - intSize);
      int *val2 = (int *)(memory + BLOCK_SIZE - intSize * 2);
      int itemCount = *val2;
      // if (*val == 0) {
      //   break;
      // }
      // cout << "Print Size val: " << *val << " __" << *val2 << endl;

      int sum = 0;
      for (int i = 0; i < itemCount; i++)
      {
        int *val3 = (int *)(memory + BLOCK_SIZE - intSize * 3 - intSize * i);
        // cout << "Print Size val3 : " << *val3 << " __" << endl;
        char *val4 = (char *)malloc(*val3);
        memcpy(val4, memory + sum, *val3);
        sum += *val3;
        // cout << val4 << endl;

        vector<string> fields;
        string str = string(val4);
        stringstream ss(str);
        string field;
        while (std::getline(ss, field, '$'))
        {
          fields.push_back(field);
        }
        // cout << "ID: " << fields[0] << endl;
        Record record(fields);
        if (record.id == target)
        {
          isFound = 1;
          record.print();
          return;
        }
      }
    }
  }

  void readFromFile(int target)
  {
    for (int i = 0; i < MAX_PAGE; i++)
    {
      free(pageBuffer[i]);
      pageBuffer[i] = static_cast<unsigned char *>(std::malloc(BLOCK_SIZE));
    }
    isFound = 0;
    // initializeMemory();
    clearPages();
    // cout << "READ FROM FILE" << endl;
    const std::size_t ChunkSize = BLOCK_SIZE; // Define the chunk size. 4KB
    std::ifstream inFile(fileName,
                         std::ios::binary); // Open the file for reading.

    if (!inFile)
    {
      std::cerr << "Cannot open file for reading: " << fileName << std::endl;
      return;
    }

    std::vector<unsigned char> buffer(ChunkSize);
    bool isRead = false;
    int pgCount = 0;

    unsigned char *pageBufferTmp =
        static_cast<unsigned char *>(std::malloc(BLOCK_SIZE));
    while (inFile.read(reinterpret_cast<char *>(buffer.data()), ChunkSize))
    {
      // memcpy(pageBuffer[pgCount++], buffer.data(), ChunkSize);
      memcpy(pageBufferTmp, buffer.data(), ChunkSize);
      findValue(1, target, pageBufferTmp);
    }
    inFile.close(); // Close the file.
  }

  // Given an ID, find the relevant record and print it
  void findRecordById(int id)
  {
    cout << "Finding record with id: " << id << endl;
    readFromFile(id);
  }
};
