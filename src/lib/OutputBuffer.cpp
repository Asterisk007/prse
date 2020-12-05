#include "OutputBuffer.h"
#include "Command_args.h"
#include <iostream>

// Default constructor
OutputBuffer::OutputBuffer(){
   buffer = vector<string>();
}

OutputBuffer& OutputBuffer::instance(){
    static OutputBuffer instance;
    return instance;
}

// Adds one line of code to the output buffer
// @param text:
void OutputBuffer::add_line(string text){
   buffer.push_back(text);
}

// Clears the current buffer completely.
void OutputBuffer::clear_buffer(){
   buffer.resize(0);
}

void OutputBuffer::output_to_file(ofstream* file){
    if (Command_args::instance().VERBOSE()){
        for (auto s : buffer) {
            cout << s;
        }
    }
    if (file != nullptr){
        for (auto s : buffer) {
            *file << s;
        }
    }
}
