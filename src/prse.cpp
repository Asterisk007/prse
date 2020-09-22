#include "parser.h"
#include <iostream>
#include <map>

using namespace std;

extern int yylex();
extern int yyparse();
extern int line_count;
bool verbose = false;

void yyerror(const char* error){
	fprintf(stderr, "%s on line %d\n", error, line_count);
}

void help_text(){
	cout << "Usage: prsec [OPTIONS] [FILE(S)]" << endl;
	cout << "OPTIONS: " << endl;
	string options_list[] = {
		"  --cpp                   Output directly to C++ instead of binary file",
        "  -g                      Include debugging symbols in binary, for use with gdb"
        "\n  --help                  Display this help text",
		"  -o,                     Specify an output file name (default a.out)",
        "  -v, --verbose           Verbose mode",
        "To view PRSE documentation, run prsedoc instead."
	};
	for (int i = 0; i < (sizeof(options_list)/sizeof(options_list[0])); i++){
		cout << options_list[i] << endl;	
	}
}

void version(){
    cout << "prsec (PRSE Compiler), written by Daniel Ellingson" << endl;
    if (BETA) {
        cout << "Beta compiler, intended to test new features" << endl;
    }
    cout << "Version " << VERSION << "." << SUBVERSION << "." << SUBSUBVERSION << endl;
    cout << "For help, enter prsedoc into your terminal to open PDF documentation, or use prsec -h" << endl;
}

int main(int argc, char** argv){
    string output_filename = "";
    // Create a library of available bash/command line arguments
    map<string, bool> activated_args;
    activated_args["-cpp"] = false;
    activated_args["-g"] = false;
    activated_args["--help"] = false;
    activated_args["-o"] = false;
    activated_args["-v"] = false;
    activated_args["--verbose"] = false;
    activated_args["--sacrifice"] = false;
    activated_args["--sacrifice=anyways"] = false;
    activated_args["--version"] = false;

    // A vector of input files, which we will parse once we process
    // each argument passed to the program
    vector<string> input_files = vector<string>(); 
    // Process each argument passed
    for (int i = 1; i < argc; i++){
        string file_candidate = string(argv[i]);
        string filetype = ".prse";
        // If string is a file, check that we can open it.
        // '!' is necessary to reverse the logical FALSE return value, which means it matches.
        if (file_candidate.length() > filetype.length() && !file_candidate.compare(file_candidate.length() - filetype.length(), filetype.length(), filetype)){
            FILE* file = fopen(file_candidate.c_str(), "r");
            if (!file){
                cerr << "Could not open file \"" << file_candidate << "\"" << endl;
                return 1;
            } else {
                // Add this file to the list of files
                fclose(file);
                input_files.push_back(file_candidate);
            }
        } else {
        // Otherwise, if this isn't a file, check that it exists in our library of available arguments
            string arg = string(argv[i]);
            if (activated_args.find(arg) != activated_args.end()){
                activated_args[arg] = true;
                // Get output filename
                if (arg == "-o" && i+1 < argc) {
                    output_filename = string(argv[i+1]);
                    i = i+2;
                } else if (arg == "-o" && i+1 >= argc) {
                    cout << "Please specify a filename" << endl;
                    return 1;
                }
            } else {
                // Finally, if this is not a valid input file nor a recognized argument,
                // output an error and continue.
                cout << "Unrecognized argument '" << arg << "'" << endl;
            }
        }
    }
    
    // If --help was passed as an argument, print out the help text
    // and exit.
    if (activated_args["--help"]) {
        help_text();
        return 0;
    }
    // Print out compiler version, if requested.
    if (activated_args["--version"]) {
        version();
        return 0;
    }
    // Activate verbose mode.
    if (activated_args["--verbose"] || activated_args["-v"]) {
        verbose = true;
    }
    extern FILE* yyin;

    // Initialize the standard PRSE library, which contains
    // aliases for each C++ standard library.
    /* cout << "Initializing libraries" << endl;
    // sanity check that libraries are initialized
    Library& library = Library::instance();
    library.list_libs(); */

    // Check that at least one file has been provided as input on the command line.
    if ((int)input_files.size() > 0){
        /* if (activated_args["--sacrifice"] == true){
            cout << "The PRSE compiler accepts your humble sacrifice." << endl;
            cout << "If your code compiles, your file shall be spared." << endl;
            cout << "If not, it will be deleted forever, and you will have to start from scratch!" << endl;
            cout << "We accept take-backsies, however! Confirm that you wish to sacrifice file: " << input_files[i] << "? [Y/n]" << endl;
        } else if (activated_args["--sacrifice=anyways"]) {
            cout << "The PRSE compiler accepts your humble sacrifice." << endl;
            cout << "Your source file, " << input_files[i] << " will be deleted." << endl;
            cout << "If any errors are found, no binary will be produced." << endl;
            cout << "If no errors are found, you will be left with only your binary." << endl;
            cout << "Since you set --sacrifice to 'anyways', we will not ask for confirmation." << endl;
            cout << "Hold your breath!" << endl;
            // TODO: Delay by X seconds.
        }  */
        // Go through each input file and parse it
        for (int i = 0; i < (int)input_files.size(); i++){
            cout << "Processing file " << input_files[i] << endl;
            yyin = fopen(input_files[i].c_str(), "r");
            // reset the Flex buffer for the next file.
            //yyrestart(yyin);
            if (!yyin) {
                cerr << "Could not open " << input_files[i] << endl;
            } else {
                yyparse();
                fclose(yyin);
                if (Error::num_errors() > 0) {
                    // return 1;
                }
                OutputBuffer& output = OutputBuffer::instance();
                string t = input_files[i].substr(0, input_files[i].size()-5); t += ".cpp";
                ofstream outfile;
                outfile.open(t);
                if (outfile.is_open()){
                    cout << "Outputting to " << t << endl;
                    output.output_to_file(&outfile);
                    //output.output_to_file(nullptr);
                    // TODO: Add compilation functionality (CPP to bin)
                    outfile.close();
                } else {
                    cerr << "File " << t << " could not be opened." << endl;
                    return 1;
                }
            }   
        }
    }
    // If no PRSE files are provided, return an error.
    else {
        cerr << "Please enter at least one input file." << endl;
        return 1;
    }
    if ( output_filename != ""){
        cout << "Final output binary name: " << output_filename << endl;
    }
    return 0;
}