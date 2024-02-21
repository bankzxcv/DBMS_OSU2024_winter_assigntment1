#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <bitset>
#include <math.h>
#include <fstream>
#include <stdexcept>

using namespace std;

class Record {
 public:
  int id, manager_id;
  std::string bio, name;

  Record(vector<std::string> fields) {
    id = stoi(fields[0]);
    name = fields[1];
    bio = fields[2];
    manager_id = stoi(fields[3]);
  }

  void print() {
    cout << "\tID: " << id << "\n";
    cout << "\tNAME: " << name << "\n";
    cout << "\tBIO: " << bio << "\n";
    cout << "\tMANAGER_ID: " << manager_id << "\n";
  }
};

class LinearHashIndex {
 private:
  const int BLOCK_SIZE = 4096;
  const double REACHED_CAPACITY = 0.7;
  int bucketSize = 0;
  vector<int> bucket;
  int numberOfBlock = 0;
  int i = 0;
  int numRecords = 0;
  int nextFreeBlock;
  string fName;

  void insertRecord(Record record) {
    string block;
    fstream FileIndex;
    FileIndex.open(fName, ios::in | ios::out | ios_base::binary);

    /// Check if there are no records in the index yet
    if (numRecords == 0) {
      // Initialize index with first blocks (start with 4)
      nextFreeBlock = 4;
      numberOfBlock = 4;
      i = 2;
      // hash block
      for (int j = 0; j < 4; ++j) {
        bucket.push_back(0);
        bucket.push_back(1);
        bucket.push_back(2);
        bucket.push_back(3);
      }
      // Initialize the first four blocks in the index file
      for (int j = 0; j < 4; ++j) {
        FileIndex.seekg(j * BLOCK_SIZE);
        FileIndex << "00" << endl << "$";
      }

      bucketSize = 6;
    }
    // Calculate the hash key of the record id
    int hash_key = hash_val(record.id);
    // Get the index in the block directory using the hash key
    int index = bucket[hash_key];
    int overflowId, lengthOfBlock, record_size;
    string overFlow, data;
    // cout << "Hash key: " << hash_key << ", Index: " << index << endl;

    // Read the block from the index file
    FileIndex.seekg(index * BLOCK_SIZE);
    getline(FileIndex, block, '$');
    std::stringstream indexFileStream(block);
    indexFileStream >> overflowId;

    // Handle the overflow blocks if any
    while (overflowId > 0) {
      index = overflowId;
      FileIndex.seekg(index * BLOCK_SIZE);
      getline(FileIndex, block, '$');
      std::stringstream indexFileStream(block);
      indexFileStream >> overflowId;
      // break;
    }

    // Calculate the current block size and the size of the new record
    FileIndex.seekp(index * BLOCK_SIZE);
    lengthOfBlock = block.length();
    data = to_string(record.id) + "," + record.name + "," + record.bio + "," +
           to_string(record.manager_id);
    record_size = data.length() + 1;
    // cout << "Block size: " << block_Size << ", Record size: " << record_size
    // << endl;  //for debugging

    // If the block has enough space, add the record directly also if page
    // capacity is less than or = 70%

    if (lengthOfBlock + record_size <= BLOCK_SIZE) {
      // cout << "Block has enough space. Adding record directly." << endl;
      // //for debugging
      FileIndex.seekp(index * BLOCK_SIZE);
      FileIndex << block << data << endl << "$";
      bucketSize += record_size;
      numRecords++;
    } else {
      /// If the block does not have enough space, create an overflow block
      // cout << "Block does not have enough space. Creating overflow block." <<
      // endl;  //for debugging
      FileIndex.seekg(index * BLOCK_SIZE);
      overflowId = nextFreeBlock++;
      overFlow = to_string(overflowId);
      if (overFlow.length() == 1) {
        overFlow = "0" + overFlow;
      }
      index = overflowId;
      FileIndex << overFlow << endl;
      FileIndex.seekp(index * BLOCK_SIZE);
      FileIndex << "00" << endl << data << endl << "$";
      bucketSize += record_size;
      numRecords++;
    }
    // Checking reaching over capacity
    if (bucketSize / numberOfBlock >= (BLOCK_SIZE * REACHED_CAPACITY)) {
      int index_new;
      if (numberOfBlock >= pow(2, i) - 1) {
        i++;
      }
      index_new = numberOfBlock & ~(~0 << i - 1);
      numberOfBlock++;
      bucket.push_back(nextFreeBlock++);  // nextFreeBlock
      // cout << "Record push completed.: 179" << endl; //for debugging
      FileIndex.seekp((nextFreeBlock - 1) * BLOCK_SIZE);  // nextFreeBlock
      // cout << "Record push completed.: 181" << endl; //for debugging
      FileIndex << "00" << endl << "$";  // index_file
      // cout << "sent to overflow : 183" << endl;
      overflow_blocks(FileIndex, index_new);  // index_file
      // cout << "recieved from overflow  completed.: 185" << endl; //for
      // debugging
    }
    // cout << "Record insertion completed." << endl; //for debugging
    return;
  }

  int hash_val(int id) {
    // int div = pow(2, 16);
    int div = 216;

    int hash_key = id % div;
    // Perform a bitwise AND operation with the hash key and a binary mask to
    // ensure the number of bits in the hash key is limited to 'i'
    hash_key = hash_key & ~(~0 << i);

    // If the hash key is greater than or equal to 'n' (the number of blocks),
    // recalculate the hash key using 'i - 1' bits
    if (hash_key >= numberOfBlock) {
      hash_key = hash_key & ~(~0 << i - 1);
    }
    // Return the calculated hash key
    return hash_key;
  }

  void overflow_blocks(fstream& employeeIndexFile, int index) {
    // cout << "Inside overflow_blocks function with index: " << index << endl;
    // //for debugging

    string currentBlockData, new_block, recordLine, substr;
    vector<std::string> field;
    int over_flow = 1;
    int index_page = bucket[index];

    while (over_flow > 0) {
      // Prepare new block and clear the field vector for each iteration

      // cout << "Overflow loop, overflow: " << over_flow << endl; //for
      // debugging
      new_block = "";
      field.clear();

      // Seek to the position of the index_page in the file
      employeeIndexFile.seekg(index_page * BLOCK_SIZE);
      // cout << "Seeking to position: " << index_page * BLOCK_SIZE << endl;
      // //for debugging

      // Get all data in the current block
      getline(employeeIndexFile, currentBlockData, '$');
      // cout << "currentBlockData read: " << currentBlockData << endl; //for
      // debugging

      // Extract overflow block id from the current block data
      stringstream Overflow_line(currentBlockData);
      Overflow_line >> over_flow;
      // cout << "Overflow read from currentBlockData: " << Overflow_line <<
      // endl;  //for debugging cout << "currentBlockData : 189 :  " <<
      // currentBlockData << endl; //for debugging

      // Remove overflow block id from the block data
      currentBlockData = currentBlockData.substr(3, currentBlockData.length());
      // cout << "currentBlockData : 191 :  " << currentBlockData << endl; //for
      // debugging

      // Format overflow block id as a string
      string overFlow = to_string(over_flow);
      // cout << "Overflow converted to string: " << overFlow << endl; //for
      // debugging

      if (overFlow.length() == 1) {
        overFlow = "0" + overFlow;
      }
      // cout << "Overflow padded with 0 if needed: " << overFlow << endl; //for
      // debugging

      // Add the overflow block id as the first line of the new block
      new_block += overFlow + "\n";
      // Process each record in the current block
      stringstream lineRecord(currentBlockData);
      vector<string> row;

      // re hash  and remap the new key in the correct currentBlockData
      while (getline(lineRecord, recordLine)) {
        // Parse recordLine to extract individual fields of the record
        // cout << "Processing recordLine: " << recordLine << endl;  //for
        // debugging
        stringstream lineStream(recordLine);
        getline(lineStream, substr, ',');  // employee_id
        // cout << "Employee ID read: " << substr << endl; //for debugging
        field.push_back(substr);
        getline(lineStream, substr, ',');  // employee_name
        // cout << "Employee name read: " << substr << endl; //for debugging
        field.push_back(substr);
        getline(lineStream, substr, ',');  // empployee_bio
        // cout << "Employee bio read: " << substr << endl; //for debugging
        field.push_back(substr);
        getline(lineStream, substr, ',');  // Manager_id
        // cout << "Manager ID read: " << substr << endl; //for debugging
        field.push_back(substr);
        Record employee(field);
        if (employee.id != 0) {
          // If the hash key of the record's id does not match the current index
          if (index != hash_val(employee.id)) {
            // cout << "Rehashing and remapping record with id: " << employee.id
            // << endl; Decrement the total number of records and re-insert the
            // record
            numRecords--;
            insertRecord(employee);

          } else {
            // If the hash key matches, add the record to the new block
            new_block += recordLine + "\n";
          }
        }
        // Clear the field vector for the next iteration
        field.clear();
      }
      // overwrite new block
      new_block += "$";

      // Write the new block to the file
      employeeIndexFile.seekp(index_page * BLOCK_SIZE);
      employeeIndexFile << new_block;
      // cout << "New block written: " << new_block << endl; //for debugging

      int new_len = currentBlockData.length() - new_block.length();
      if (new_len < 0) {
        new_len = ~new_len;
      }
      std::string message(new_len, '\0');
      employeeIndexFile << message;

      // Move to the next overflow block
      index_page = over_flow;
      // cout << "Overflow block handled successfully." << endl; //for debugging
    }
  }

 public:
  LinearHashIndex(string indexFileName) {
    numberOfBlock = 4;  // Start with 4 buckets in index
    i = 2;              // Need 2 bits to address 4 buckets
    numRecords = 0;
    nextFreeBlock = 0;
    bucketSize = 0;
    fName = indexFileName;
  }
  Record finding_records(fstream& employee_index) {
    string employeeDataLine, employeeField;
    vector<std::string> fields;

    // Read a line from the file
    if (getline(employee_index, employeeDataLine)) {
      stringstream lineStream(employeeDataLine);  // is used to parse or process
                                                  // each line from the file.
      // reading employee_id from the file
      if (!getline(lineStream, employeeField, ',')) {
        throw runtime_error("Failed to read employee_id from the file");
      }
      fields.push_back(employeeField);

      if (!getline(lineStream, employeeField, ',')) {
        throw runtime_error("Failed to read employee_name from the file");
      }
      fields.push_back(employeeField);

      if (!getline(lineStream, employeeField, ',')) {
        throw runtime_error("Failed to read empployee_bio from the file");
      }
      fields.push_back(employeeField);

      if (!getline(lineStream, employeeField, ',')) {
        throw runtime_error("Failed to read Manager_id from the file");
      }
      fields.push_back(employeeField);

      return Record(fields);
    } else {
      // Check if we've reached the end of the file
      if (employee_index.eof()) {
        // cout << "Employee Index Created! Total records: " << numRecords <<
        // endl;
        throw runtime_error("End of file reached");
      } else {
        throw runtime_error("Failed to read a line from the file");
      }
    }
  }

  // Read csv file and add records to the index
  void createFromFile(const string& csvFName) {
    // cout << "Entered createFromFile with file: " << csvFName << endl;

    fstream employee_index, outputFile;

    employee_index.open(csvFName, ios::in);
    if (!employee_index.is_open()) {
      throw runtime_error("Failed to open file: " + csvFName);
    }

    // cout << "Successfully opened file: " << csvFName << endl;

    // Truncate the file to start with an empty file
    outputFile.open(fName, ios::out | ios::trunc | ios::binary);
    if (!outputFile.is_open()) {
      throw runtime_error("Failed to open file: " + fName);
    }
    outputFile.close();

    while (true) {
      try {
        Record employee = finding_records(employee_index);
        if (employee.id == -1) {
          cout << "Encountered record with id -1, exiting loop." << endl;
          break;
        }
        insertRecord(employee);
      } catch (const runtime_error& e) {
        if (string(e.what()) == "End of file reached") {
          break;
        }
      }
    }
    employee_index.close();
    // cout << "File processing completed." << endl;
  }

  // Given an ID, find the relevant record and print it
  Record findRecordById(int id) {
    // std::cout << "Initiating findRecordById with id: " << id << std::endl;

    bool recordFound = false;
    std::vector<std::string> fields;
    std::string idrecordLine, recordBlock, fieldValue;
    int hashedId = hash_val(id);
    int recordPageNumber = bucket[hashedId];
    int overflowIndicator = 1;

    std::fstream employee_index;
    // cout << fName<<endl;
    employee_index.open(fName, std::ios::in | std::ios::binary);
    if (!employee_index.is_open()) {
      throw std::runtime_error("Failed to open the file");
    }
    while (overflowIndicator > 0 && id != -1) {
      fields.clear();
      employee_index.seekg(recordPageNumber * BLOCK_SIZE);
      std::getline(employee_index, recordBlock, '$');

      std::stringstream block_stream(recordBlock);
      block_stream >> overflowIndicator;
      std::stringstream line_stream(recordBlock);

      while (std::getline(line_stream, idrecordLine)) {
        // std::cout << "Processing idrecordLine: " << idrecordLine <<
        // std::endl;
        std::stringstream record_stream(idrecordLine);

        for (int i = 0; i < 4; i++) {
          std::getline(record_stream, fieldValue, ',');
          fields.push_back(fieldValue);
        }
        Record employee(fields);
        // std::cout << "Created employee record with id: " << employee.id <<
        // std::endl;

        if (employee.id == id) {
          std::cout << "\n Found employee record with id: " << id << "\n"
                    << std::endl;
          employee_index.close();
          return employee;
        }
        fields.clear();
        recordPageNumber = overflowIndicator;
      }
    }
    if (!recordFound) {
      employee_index.close();
      throw std::runtime_error("\n No data found for id: " +
                               std::to_string(id));
    }
  }
};
