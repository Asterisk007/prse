// The static output buffer for the parser
/* 
 * Stores a list of strings, which will be what the buffer
 * output writes.
*/

#pragma once

#include <vector>
#include <string>
#include <fstream>

using namespace std;

class OutputBuffer {
private:
   vector<string> buffer;
public:
   OutputBuffer();
   void add_line(string text);
   void clear_buffer();
   static OutputBuffer& instance();
   void output_to_file(ofstream* file);
};
