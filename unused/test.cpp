#include <cstring>
#include <fstream>
#include <cstdlib>

#define BLOCK_SIZE sizeof(int)
int totalPages = 10;

int main() {
  int* memory = (int*)malloc(totalPages * BLOCK_SIZE);

  // This loop sets each integer to a value, but remember, this won't make 'a's
  // in a binary file.
  for (int i = 0; i < totalPages; i++) {
    memory[i] = 'a';  // Hexadecimal representation of 'aaaa' if you really want
                      // to see 'a' in a binary file viewed as text.
  }

  std::ofstream outFile("memory.bin", std::ios::out | std::ios::binary);
  if (outFile) {
    outFile.write((char*)memory, totalPages * BLOCK_SIZE);
    outFile.close();
  }
  free(memory);

  
  return 0;
}