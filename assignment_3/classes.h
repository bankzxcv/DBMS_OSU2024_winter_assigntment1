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
class BucketIndex
{
public:
  int Id, Offset;

  BucketIndex(int id, int offset)
  {
    Id = id;
    Offset = offset;
  }
  void setOffset(int offset) { Offset = offset; }
  void setId(int id) { Id = id; }
  int getOffset() { return Offset; }
  int getId() { return Id; }

  void print()
  {
    cout << "\tID: " << Id << "\n";
    cout << "\tOffset: " << Offset << "\n";
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

class StorageBufferManager
{
private:
  const int BLOCK_SIZE = 4096; // initialize the  block size allowed in main
  const int MAX_PAGE = 3;
  // You may declare variables based on your need
  int numRecords = 0;
  int overallSize = 0;

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
  std::fstream file;

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
    recordCount = (int *)(memory + BLOCK_SIZE - sizeof(int) * 3);
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

    int isPageOverFlow = -1;
    // update record count to slot
    memcpy(memory + BLOCK_SIZE - intSize * 2, &isPageOverFlow, intSize);

    // update record count to slot
    memcpy(memory + BLOCK_SIZE - intSize * 3, recordCount, intSize);

    // update the length of the block to slot
    int lengthOfBlock = (int)record.size();
    memcpy(memory + BLOCK_SIZE - intSize * 3 - intSize * (*recordCount),
           &lengthOfBlock, intSize);

    int *val = (int *)(memory + BLOCK_SIZE - intSize);
    int *val2 = (int *)(memory + BLOCK_SIZE - intSize * 3);
    int *val3 =
        (int *)(memory + BLOCK_SIZE - intSize * 3 - intSize * (*recordCount));
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
        BLOCK_SIZE - sizeof(int) * (4 + (*recordCount)))
    {
      cout << numRecords << " " << r.size() << " " << currentLength << " "
           << BLOCK_SIZE - sizeof(int) * (4 + (*recordCount)) << endl;
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
    currentPage = 0;
    initializeMemory();
  }

public:
  StorageBufferManager(string NewFileName)
  {
    // initialize your variables
    fileName = NewFileName;
  }

  // loop print value in pageBuffer[2]

  void writeFileAt(unsigned char *buffer, int page)
  {
    // int offsetAt = 4096 * page;
    int offsetAt = 4096 * page;
    file.open(fileName, std::ios::binary | std::ios::in | std::ios::out);
    file.seekp(offsetAt, std::ios::beg);
    file.write((char *)buffer, BLOCK_SIZE);
    file.close();
  }

  bool findRecordById(int id, int page)
  {
    unsigned char *buffer =
        static_cast<unsigned char *>(std::malloc(BLOCK_SIZE));
    int offsetAt = 4096 * page;
    file.open(fileName, std::ios::binary | std::ios::in);
    file.seekg(offsetAt);
    file.read((char *)buffer, BLOCK_SIZE);
    int *freeBlock = (int *)(buffer + BLOCK_SIZE - intSize);
    int *itemCount = (int *)(buffer + BLOCK_SIZE - intSize * 3);
    if (*itemCount == NULL || *itemCount == 0)
    {
      return false;
    }
    int currentPosition = 0;
    for (int i = 0; i < *itemCount; i++)
    {
      int *recordLen = (int *)(buffer + BLOCK_SIZE - intSize * 4 - intSize * i);
      cout << "Record Length: " << *recordLen << endl;
      char *record = (char *)malloc(*recordLen);
      memcpy(record, buffer + currentPosition, *recordLen);
      vector<string> fields;
      string str = string(record);
      stringstream ss(str);
      string field;
      while (std::getline(ss, field, '$'))
      {
        fields.push_back(field);
      }
      cout << "ID: " << fields[0] << " Target = " << id << " i=" << i << endl;
      free(record);
      if (stoi(fields[0]) == id)
      {
        return true;
      }
      // check if it has overflow page
      currentPosition += *recordLen;
    }
    // check overflow page
    int *isPageOverFlow = (int *)(buffer + BLOCK_SIZE - intSize * 2);
    if (*isPageOverFlow != -1)
    {
      int *nextPage = (int *)(buffer + BLOCK_SIZE - intSize * 2);
      return findRecordById(id, page + 216);
    }
    return false;
  }
  // find the value of the page
  int getSizeOfPage(int page)
  {
    unsigned char *buffer =
        static_cast<unsigned char *>(std::malloc(BLOCK_SIZE));

    int offsetAt = 4096 * page;
    // cout << "asdasdasdasd" << endl;
    file.open(fileName, std::ios::binary | std::ios::in);
    file.seekg(offsetAt);
    file.read((char *)buffer, BLOCK_SIZE);
    file.close();
    int *itemCount = (int *)(buffer + BLOCK_SIZE - intSize * 3);
    int *nextFreeBlock = (int *)(buffer + BLOCK_SIZE - intSize);
    int sizeOfOffset = intSize * 3 + intSize * (*itemCount);
    int *isPageOverFlow = (int *)(buffer + BLOCK_SIZE - intSize * 2);
    cout << "isPageOverFlow " << *isPageOverFlow << "*ITEM COunt" << *itemCount
         << endl;
    if (*isPageOverFlow != -1)
    {
      sizeOfOffset += getSizeOfPage(page + 216);
    }

    return int(sizeOfOffset + *nextFreeBlock);
  }

  void insertToMemoryPage(Record item, int page)
  {
    vector<char> r = item.serializeToString();
    unsigned char *buffer =
        static_cast<unsigned char *>(std::malloc(BLOCK_SIZE));
    int offsetAt = 4096 * page;
    file.open(fileName, std::ios::binary | std::ios::in);
    file.seekg(offsetAt);
    file.read((char *)buffer, BLOCK_SIZE);
    file.close();
    int *lastBlock = (int *)(buffer + BLOCK_SIZE - intSize);
    int *isOverflow = (int *)(buffer + BLOCK_SIZE - intSize * 2);
    int *itemCount = (int *)(buffer + BLOCK_SIZE - intSize * 3);
    int currentPosition = 0;
    // if new page
    if (*itemCount == NULL || *itemCount == 0)
    {
      int *lastBlock = (int *)(buffer + BLOCK_SIZE - intSize);
      int *itemCount = (int *)(buffer + BLOCK_SIZE - intSize * 3);
      memcpy(buffer + currentPosition, item.serializeToString().data(),
             item.serializeToString().size());
      *lastBlock = item.serializeToString().size();
      *itemCount = 1;
    }
    else
    {
      for (int i = 0; i < *itemCount; i++)
      {
        int *recordLen =
            (int *)(buffer + BLOCK_SIZE - intSize * 4 - intSize * i);
        currentPosition += *recordLen;
      }
      // if overflow
      int sizeOfOffset = intSize * 3 + intSize * (*itemCount);
      if (currentPosition + item.serializeToString().size() + sizeOfOffset >
          BLOCK_SIZE)
      {
        *isOverflow = 1;
        int *nextPage = (int *)(buffer + BLOCK_SIZE - intSize * 2);
        if (*nextPage == -1)
        {
          *nextPage = page + 216;
        }
        writeFileAt(buffer, page);
        free(buffer);
        insertToMemoryPage(item, page + 216);
        return;
      }

      memcpy(buffer + currentPosition, item.serializeToString().data(),
             item.serializeToString().size());
      *lastBlock += item.serializeToString().size();
      *itemCount += 1;
    }
    writeFileAt(buffer, page);
  }

  void removeRecordFromMemoryPage(int id, int page)
  {
    int isOverflow = page > 216 ? 1 : 0;
    unsigned char *buffer =
        static_cast<unsigned char *>(std::malloc(BLOCK_SIZE));
    int offsetAt = 4096 * page;
    file.open(fileName, std::ios::binary | std::ios::in);
    file.seekg(offsetAt);
    file.read((char *)buffer, BLOCK_SIZE);
    file.close();
    int *freeBlock = (int *)(buffer + BLOCK_SIZE - intSize);
    int *itemCount = (int *)(buffer + BLOCK_SIZE - intSize * 3);
    cout << "Free Block: " << *freeBlock << endl;
    cout << "Item Count: " << *itemCount << endl;
    int currentPosition = 0;
    for (int i = 0; i < *itemCount; i++)
    {
      int *recordLen = (int *)(buffer + BLOCK_SIZE - intSize * 4 - intSize * i);
      cout << "Record Length: " << *recordLen << endl;
      char *record = (char *)malloc(*recordLen);
      memcpy(record, buffer + currentPosition, *recordLen);
      vector<string> fields;
      string str = string(record);
      stringstream ss(str);
      string field;
      while (std::getline(ss, field, '$'))
      {
        fields.push_back(field);
      }
      cout << "ID: " << fields[0] << " Target = " << id << " i=" << i << endl;
      free(record);
      if (stoi(fields[0]) == id)
      {
        int shift = currentPosition + *recordLen;
        int shiftLen = *freeBlock - shift;
        memcpy(buffer + currentPosition, buffer + shift, shiftLen);

        *freeBlock -= *recordLen;
        *itemCount -= 1;
        for (int j = i; j < *itemCount; j++)
        {
          int *recordLen =
              (int *)(buffer + BLOCK_SIZE - intSize * 4 - intSize * j);
          int *recordLenNext =
              (int *)(buffer + BLOCK_SIZE - intSize * 4 - intSize * (j + 1));
          memcpy(buffer + BLOCK_SIZE - intSize * 4 - intSize * j, recordLenNext,
                 intSize);
        }
        memcpy(buffer + BLOCK_SIZE - intSize, freeBlock, intSize);
        break;
      }
      currentPosition += *recordLen;
    }
    writeFileAt(buffer, page);
  }

  void readAtMemoryIndex()
  {
    removeRecordFromMemoryPage(11432120, 0);
    return;

    int page = 0;
    int offsetAt = 4096 * page;

    // declare local page buffer
    unsigned char *pageBufferTmp =
        static_cast<unsigned char *>(std::malloc(BLOCK_SIZE));

    file.open(fileName, std::ios::binary | std::ios::in);
    file.seekg(offsetAt);
    file.read((char *)pageBufferTmp, BLOCK_SIZE);
    file.close();
    removeRecordFromMemoryPage(11432120, 0);
    removeRecordFromMemoryPage(11432120, 0);
  }

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
    std::fstream csvFile;
    // Read the file and add records to the EmployeeRelation
    // file = std::ofstream(fileName, std::ios::binary | std::ios::out);
    file.open(fileName, std::ios::binary | std::ios::out);
    csvFile.open(csvFName, std::ios::in);
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
      int *val2 = (int *)(memory + BLOCK_SIZE - intSize * 3);
      int itemCount = *val2;
      // if (*val == 0) {
      //   break;
      // }
      // cout << "Print Size val: " << *val << " __" << *val2 << endl;

      int sum = 0;
      for (int i = 0; i < itemCount; i++)
      {
        int *val3 = (int *)(memory + BLOCK_SIZE - intSize * 4 - intSize * i);
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
    std::fstream inFile;
    inFile.open(fileName, std::ios::binary | std::ios::in);

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

StorageBufferManager manager("EmployeeRelation");

class LinearHashIndex
{
private:
  const int BLOCK_SIZE = 4096;
  const int MAX_PAGE = 3;
  map<string, int> mp;
  int overflowpage = 0;
  vector<BucketIndex>
      bucket;              // Map the least-significant-bits of h(id) to a bucket
                           // location in EmployeeIndex (e.g., the jth bucket) can
                           // scan to correct bucket using j*BLOCK_SIZE as offset
                           // (using seek function) can initialize to a size of 256
                           // (assume that we will never have more than 256 regular
                           // (i.e., non-overflow) buckets)
  int n;                   // The number of indexes in bucket currently being used
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

  void setNextPage()
  {
    currentPage++;
    currentPageSize = 0;
    // memory = pageBuffer[currentPage];
    // setLastRecord();
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
  int binaryToDecimal(int n)
  {
    int num = n;
    int dec_value = 0;

    // Initializing base value to 1, i.e 2^0
    int base = 1;

    int temp = num;
    while (temp)
    {
      int last_digit = temp % 10;
      temp = temp / 10;

      dec_value += last_digit * base;

      base = base * 2;
    }

    return dec_value;
  }

  string decToBinaryInStringAndAddZero(int n)
  {
    // array to store binary number
    int binaryNum[32];

    // counter for binary array
    int z = 0;
    while (n > 0)
    {
      // storing remainder in binary array
      binaryNum[z] = n % 2;
      n = n / 2;
      z++;
    }
    string ans = "";

    // printing binary array in reverse order
    for (int j = z - 1; j >= 0; j--)
    {
      auto s = std::to_string(binaryNum[j]);
      ans = ans + "" + s;
      // cout << binaryNum[j];
    }
    // cout << endl
    //      << "ans Before " << ans << endl
    //      << "ans.length() " << ans.length() << endl
    //      << "i " << i << endl;

    if (ans.length() != i)
    {
      // cout << "if (ans.length() != i) " << endl;
      for (int c = ans.length(); c < i; c++)
      {
        ans = "0" + ans;
      }
    }
    // cout << endl << "ans After " << ans << endl;

    return ans;
  }
  string doBitfilp(string inputId)
  {
    string ans = inputId;
    // cout << "!ans[] =  " << (!ans.at(0)) << endl;

    // cout << "befor filp  " << ans << endl;
    if ((ans.at(0)) == 0)
    {
      ans[0] = '1';
    }
    else
    {
      ans[0] = '0';
    }

    // cout << "after filp  " << ans << endl;
    return ans;
  }
  int ni = 0;
  // Insert new record into index
  bool checkBucketidMatchInputid(string inputId)
  {
    cout << endl
         << "checkBucketidMatchInputid" << endl;
    for (auto element : bucket)
    {
      // cout << "element " << element << endl;
      string BucketIndexInBinary =
          decToBinaryInStringAndAddZero(element.getId());
      // cout << endl;
      if (BucketIndexInBinary == inputId)
      {
        cout << "-----------------------------------------------------------"
             << endl;
        cout << endl
             << "I Found it \n inputId :" << inputId
             << " macth with :" << BucketIndexInBinary << endl;
        cout << "-----------------------------------------------------------"
             << endl;
        // add record to that page
        return true;
      }
      else if (doBitfilp(inputId) == BucketIndexInBinary) // doBitfilp
      {
        //======== doBitfilp ========
        cout << "------------------doBitfilp-----------------" << endl;
        cout << endl
             << "I Found it \n inputId :" << inputId
             << " macth with :" << BucketIndexInBinary << endl;
        cout << "-----------------------------------------------------------"
             << endl;
        return true;
        // add record to that page
      }
      else
      {
        //========Not found ========
        return false;
      }
    }
  }

  void insertRecord(Record record)
  {
    //-------------------------------------- mod , cut string
    ni++;
    printf("insert_Each_data (%d)\n", ni);
    std::vector<char> r = record.serializeToString();
    convertToBinary(record.id);
    cout << "IdBinary " << IdBinary << endl;
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

    string ResultIndexAfterCut = NewIdBinaryAfterMod.substr(
        NewIdBinaryAfterMod.length() - i, NewIdBinaryAfterMod.length());
    cout << "Cut only = I " << ResultIndexAfterCut << endl;
    // numRecords /(4096 * (n + overflowpage))
    if (ni == 40 || ni == 45) // <total_number_of_bytes_stored>
                              // /(4KB*<number_of_non_overflow_pages>
    {
      // cout << "before n " << n << endl;
      n++;
      //  cout << "After n " << n << endl;

      //====================================================ADD ID and OFFSET to
      // bucket
      // ADD more
      // bucket==============================================================================
      int id = n-1;
      int offset = n-1;
      BucketIndex Btest(id, offset);
      bucket.push_back(Btest);
      cout << "n  = " << n << endl;
      cout << "pow(2, i) = " << pow(2, i) << endl;

      if (n > (pow(2, i)))
      {
        cout << "i++ " << endl;
        //  cout << "before i " << i << endl;
        i++;

        //  cout << "After i " << i << endl;
        // need to rerange data in page
        cout << "doBitfilp(decToBinaryInStringAndAddZero(id)) " << doBitfilp(decToBinaryInStringAndAddZero(id)) << endl;

        for (auto element : bucket)
        {
          if (doBitfilp(decToBinaryInStringAndAddZero(id)) == decToBinaryInStringAndAddZero(element.getId()))
          {

            cout << "Match ID  doBitfilp(decToBinaryInStringAndAddZero(id)) == decToBinaryInStringAndAddZero(element.getId()) " << doBitfilp(decToBinaryInStringAndAddZero(id)) << endl;
          }
        }
      }
      printBucket();
    }
    else // no need to extension
    {
      if (checkBucketidMatchInputid(ResultIndexAfterCut))
      {
        cout << "TTTTTTTTTTTTTTTTTTTTTTTTTTTTTT" << endl;
        // add in that Bucket
        int tttt = stoi(ResultIndexAfterCut);
        // cout << "Before tttttttttttttttt   " << tttt << endl;

        tttt = binaryToDecimal(tttt);
        // cout << "AFter tttttttttttttttt   " << tttt << endl;
        manager.insertToMemoryPage(record, tttt);
        cout << "--------------------------------------------------------------"
                "-------   "
             << endl;
      }
      else
      {
        // dobitfilpppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppp
        string bitfilpIdCheck = doBitfilp(ResultIndexAfterCut);
        if (checkBucketidMatchInputid(bitfilpIdCheck))
        {
          // add in that Bucket
          int tttt = stoi(bitfilpIdCheck);
          tttt = binaryToDecimal(tttt);
          manager.insertToMemoryPage(record, tttt);
        }
      }
    }

    NewIdBinaryAfterMod = "";
    //----------------------------------------------------------------------
    // Check ID match with BucketID

    //----------------------------------------------------------------------

    // record.print();
    //  r to binary
    //  rBianry = after to binary

    // -----------------------------------------chcek bitfilp of i page and
    // update to correctly

    // No records written to index yet
    if (numRecords == 0)
    {
    }
    else
    {
    }
  }

  void printBucket()
  {
    for (auto element : bucket)
    {
      cout << "ID " << element.getId() << "  "
           << "Offset " << element.getOffset() << endl;
    }
  }
  // add bk index
  void setBucket(int numberOfn)
  {
    for (int x = 0; x < numberOfn; x++)
    {
      //====================================================ADD ID and OFFSET to
      // bucket==============================================================================
      int id = x;
      int offset = (x)*BLOCK_SIZE;
      BucketIndex Btest(id, offset);
      bucket.push_back(Btest);
      // string qwe = toBinary(x);
      // cout << "qwe.length()  " << qwe.length() << endl;
      // cout << "i  " << i << endl;
      // if (qwe.length() < i)
      // {
      //   while (qwe.length() != i)
      //   {
      //     qwe = "0" + qwe;
      //   }
      // }

      // mp[qwe] = x;
    }
    printBucket();
  }

public:
  LinearHashIndex(string indexFileName)
  {
    // info = (struct StorageBufferManager *)malloc(sizeof(struct
    // StorageBufferManager));
    n = 4; // Start with 4 buckets in index
    i = 2; // Need 2 bits to address 4 buckets
    numRecords = 0;
    nextFreeBlock = 0;
    fName = indexFileName;
    // Create your EmployeeIndex file and write out the initial 4 buckets
    // make sure to account for the created buckets by incrementing
    // nextFreeBlock appropriately
    setBucket(n);
  }

  // Read csv file and add records to the index
  void createFromFile(string csvFName)
  {
    // Add records to the EmployeeRelation

    std::ifstream csvFile(csvFName);
    // Read the file and add records to the EmployeeRelation
    // file = std::ofstream(fName, std::ios::out | std::ios::binary);
    file.open(fName, std::ios::out | std::ios::binary);
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

      // //------------sum each pages--------------

      // int dfgdfg = manager.getSizeOfPage(bucket[0].getId()); // this line
      // cout << "dfgdfg " << dfgdfg << endl;
      // for (auto element : bucket)
      // {

      //   // numRecords = numRecords + manager.getSizeOfPage(element.getId()); // this line

      //   // sum each page / 4096 * p
      // }
      // cout << "numRecords " << numRecords << endl;

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
