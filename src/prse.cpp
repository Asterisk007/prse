#include "parser.h"
#include <iostream>
#include <map>
#include "lib/Command_args.h"

using namespace std;

extern int yylex();
extern int yyparse();
extern int yylex_destroy();
extern void yyrestart( FILE *new_file );
extern int line_count;

void yyerror(const char* error){
    fprintf(stderr, "%s on line %d\n", error, line_count);
}

void help_text(){
    printf( "Usage: prsec [OPTIONS] [FILE(S)]\n"
            "OPTIONS: \n" );
    Command_args::instance().print_help_text();
}

void version(){
    printf("prsec -- official PRSE compiler\n");
    if (BETA) {
        printf("Beta compiler, intended to test new features\n");
    }
    printf( "Version %s.%s.%s\n", MAJOR, MINOR, PATCH);
            //"Documentation available online at https://gitlab.com/Asterisk007/prse/-/wikis/docs/home\n", MAJOR, MINOR, PATCH);
}

int main(int argc, char** argv){
    #ifdef DEBUG
        printf(
            "================================================================\n"
            "DEBUG BUILD HAS BEEN ENABLED. ENSURE YOU DISABLE IT FOR RELEASE.\n"
            "use: $ cmake -DCMAKE_BUILD_TYPE=Release ..\n"
            "(If you are seeing this message from a public build, let me know: https://github.com/asterisk007)\n"
            "================================================================\n\n"
        );
    #endif
    
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
        if (
            file_candidate.length() > filetype.length()
            // '!' is necessary to reverse the logical FALSE return value, which means it matches.
            && !file_candidate.compare(file_candidate.length() - filetype.length(), filetype.length(), filetype)
        ){
            FILE* file = fopen(file_candidate.c_str(), "r");
            if (!file){
                fprintf(stderr, "Could not open file \"%s\"\n", file_candidate.c_str());
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
                    printf("Please specify a filename\n");
                    return 1;
                }
            } else {
                // Finally, if this is not a valid input file nor a recognized argument,
                // output an error and continue.
                printf("Unrecognized argument '%s'", arg.c_str());
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
    if (cmd_args.get_arg("--explain") || cmd_args.get_arg("--docs") || cmd_args.get_arg("--prsedoc")) {
        cout << "Haven't implemented this yet." << endl;
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
        // Go through each input file and PRSE it
        for (int i = 0; i < (int)input_files.size(); i++){
            Function_definition::main_in_current_file = false;
            if (VERBOSE){
                cout << "Processing file " << input_files[i] << endl;
            }
            yyin = fopen(input_files[i].c_str(), "r");
            // reset the Flex buffer for the next file.
            yyrestart(yyin);
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
                    printf("%d error%s found in program. No output will be produced.\n", Error::num_errors(), (Error::num_errors() > 1 ? "s" : ""));
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
                    // If current file does not have a main function, create a header file with
                    // Function and class declarations
                    if (!Function_definition::main_in_current_file){
                        ofstream header;
                        size_t prse_ext = input_files[i].find(".prse");
                        string header_name = input_files[i].substr(0, prse_ext) + ".hpp";
                        
                        if (VERBOSE) {
                            cout << "Opening " << header_name << " as header file" << endl;
                        }
                        
                        header.open(header_name, fstream::out);
                        if (!header){
                            cout << "Error: Could not open " << header_name << " to create C++ declarations." << endl;
                        } else {
                            header << "#pragma once" << endl;

                            /*
                            For each PRSE function definition, create a C++ equivalent.
                            Format is `<return type> <name>(<params>);`
                            */
                            for (int k = 0; k < (int)Function_definition::definitions_in_current_file.size(); k++){
                                Function_definition fd = *(Function_definition::definitions_in_current_file[k]);

                                PRSE_type func_return_type = fd.get_type();
                                string func_name = fd.get_id();
                                vector<PRSE_type> func_params = fd.get_parameters();
                                
                                string line = prse_type_to_string(func_return_type) + " " + func_name + "(";
                                    
                                for (int param_it = 0; param_it < (int)func_params.size(); param_it++){
                                    line += prse_type_to_string(func_params[param_it]);
                                    
                                    if (param_it < (int)func_params.size()-1) {
                                        line += ", ";
                                    }
                                }
                                
                                line += ");\n";
                                
                                header << line;
                                
                                if (VERBOSE) {
                                    cout << line;
                                }
                            }
                        }
                        header.close();
                        output_files.push_back(header_name);
                    }
                    output.clear_buffer();
                } else {
                    if (VERBOSE){
                        cout << "File " << input_files[i] << " compiled successfully." << endl;
                    }
                }
            }
            // Clear function definitions in current file.
            Function_definition::definitions_in_current_file.resize(0);
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
        string compilation_files = "";
        for (int i = 0; i < (int)output_files.size(); i++){
            // Prevent created .hpp files from being included in compilation.
            if (output_files[i].find(".hpp") == string::npos) {
                compilation_files += (output_files[i] + " ");
            }
            
            files += (output_files[i] + " ");
        }

        if (output_filename != ""){
            string p = string("clang++ -std=c++17 -o " + output_filename + " " + compilation_files);
            if (cmd_args.VERBOSE()) cout << p << endl;
            system(p.c_str());
        } else {
            string p = string("clang++ -std=c++17 " + compilation_files);
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