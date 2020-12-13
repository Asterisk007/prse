#include "parser.h"
#include <iostream>
#include <map>

using namespace std;

extern int yylex();
extern int yyparse();
extern int yylex_destroy();
extern int line_count;

void yyerror(const char* error){
    fprintf(stderr, "%s on line %d\n", error, line_count);
}

void help_text(){
    cout << "Usage: prsec [OPTIONS] [FILE(S)]" << endl;
    cout << "OPTIONS: " << endl;
    string options_list[] = {
        "--cpp                   Output directly to C++ instead of binary file",
        "--check                 Check whether code compiles. Runs all compilation steps except for binary compilation",
        "-g                      Include debugging symbols in binary, for use with gdb",
        "-h, --help              Display this help text",
        "-o                      Specify an output file name (default a.out)",
        "-v, --verbose           Verbose mode",
        "--version               Display this program's version number",
        "--nocodes               Do not display error codes for each error found",
        "-w                      Do not display warnings"
    };
    for (int i = 0; i < (sizeof(options_list)/sizeof(options_list[0])); i++){
        cout << "    " << options_list[i] << endl;	
    }
}

void version(){
    cout << "prsec (PRSE compiler), written by Daniel Ellingson" << endl;
    if (BETA) {
        cout << "Beta compiler, intended to test new features" << endl;
    }
    cout << "Version " << VERSION << "." << SUBVERSION << "." << SUBSUBVERSION << endl;
    cout << "Documentation available online at https://asterisk007.gitlab.io/prse/docs/" << endl;
}

int main(int argc, char** argv){
    string output_filename = "";

    Command_args& cmd_args = Command_args::instance();
    // A vector of input files, which we will parse once we process
    // each argument passed to the program
    vector<string> input_files = vector<string>();
    vector<string> output_files(0);
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
            if (cmd_args.set_arg(arg) == true){
                // Get output filename
                if (arg == "-o" && i+1 < argc) {
                    output_filename = string(argv[i+1]);
                    i = i+1;
                    continue;
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

    bool VERBOSE = cmd_args.VERBOSE();
    
    // If --help was passed as an argument, print out the help text
    // and exit.
    if (cmd_args.get_arg("--help") || cmd_args.get_arg("-h")) {
        help_text();
        return 0;
    }
    // Print out compiler version, if requested.
    if (cmd_args.get_arg("--version")) {
        version();
        return 0;
    }
    if (cmd_args.get_arg("--nocodes")){
        Error::print_error_codes = false;
    }
    if (cmd_args.get_arg("-w")){
        Warning::show_warnings = false;
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
        /* if (cmd_args.get_arg("--sacrifice") == true && cmd_args.get_arg("--sacrifice=anyways") == false){
            cout << "The PRSE compiler accepts your humble sacrifice." << endl;
            cout << "If your code compiles, your file shall be spared." << endl;
            cout << "If not, it will be deleted forever, and you will have to start from scratch!" << endl;
            cout << "We accept take-backsies, however! Confirm that you wish to sacrifice file: " << input_files[i] << "? [Y/n]" << endl;
        } else if (cmd_args.get_arg("--sacrifice=anyways")) {
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
            if (VERBOSE){
                cout << "Processing file " << input_files[i] << endl;
            }
            yyin = fopen(input_files[i].c_str(), "r");
            // reset the Flex buffer for the next file.
            //yyrestart(yyin);
            if (!yyin) {
                cerr << "Could not open " << input_files[i] << endl;
            } else {
                if (yyparse() == 1){
                    Function_definition::cleanup();
                    yylex_destroy();
                    return 1;
                }
                fclose(yyin);
                if (Error::num_errors() > 0) {
                    // return 1;
                    cout << Error::num_errors() << " error";
                    if (Error::num_errors() > 1) cout << "s";
                    cout << " found in program. No output will be produced." << endl;
                    Function_definition::cleanup();
                    yylex_destroy();
                    return 1;
                }

                if (!cmd_args.get_arg("--check")){
                    OutputBuffer& output = OutputBuffer::instance();
                    string t = input_files[i].substr(0, input_files[i].size()-5); t += ".cpp";
                    ofstream outfile;
                    outfile.open(t);
                    if (outfile.is_open()){
                        if (VERBOSE){
                            cout << "Outputting to " << t << endl;
                        }
                        output.output_to_file(&outfile);
                        output_files.push_back(t);
                        outfile.close();
                    } else {
                        cout << "File " << t << " could not be opened." << endl;
                        Function_definition::cleanup();
                        yylex_destroy();
                        return 1;
                    }
                } else {
                    if (VERBOSE){
                        cout << "File " << input_files[i] << " compiled successfully." << endl;
                    }
                }
            }   
        }
    }
    // If no PRSE files are provided, return an error.
    else {
        cerr << "Please enter at least one input file." << endl;
        cerr << "For program help, type prsec -h or prsec --help" << endl;
        return 1;
    }
    // If --cpp is not specified at execution time, compile all translated
    // .cpp files into a single binary
    if (!cmd_args.get_arg("--cpp") && !cmd_args.get_arg("--check")){
        string files = "";
        for (int i = 0; i < (int)output_files.size(); i++){
            files += (output_files[i] + " ");
        }

        if (output_filename != ""){
            string p = string("clang++ -std=c++17 -o " + output_filename + " " + files);
            if (cmd_args.VERBOSE()) cout << p << endl;
            system(p.c_str());
        } else {
            string p = string("clang++ -std=c++17 " + files);
            if (cmd_args.VERBOSE()) cout << p << endl;
            system(p.c_str());
        }
        string rm_str = "rm " + files;
        system(rm_str.c_str());
    }
    Function_definition::cleanup();
    yylex_destroy();
    return 0;
}