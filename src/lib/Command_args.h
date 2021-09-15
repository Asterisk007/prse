#pragma once
#include <map>
#include <string>
#include <vector>

using namespace std;

class Command_args {
public:
    bool get_arg(const string& argument);
    bool set_arg(const string& argument);
    void print_help_text();
    static Command_args& instance();
    bool VERBOSE();
private:
    map<string, bool> args;
    vector<string> valid_args;
    map<string, string> arg_help;
    Command_args(){
        args = map<string, bool>();
        // Add valid arguments to the list.
        valid_args.insert(valid_args.end(),
            {
                "--cpp",
                "--check",
                "-g",
                "-h",
                "--help",
                "-o",
                "-v",
                "--verbose",
                "--sacrifice",
                "--sacrifice=anyways",
                "--version",
                "-V",
                "--nocodes",
                "-N",
                "-w",
                "-Wall",
                "-Werr"
            });
        
        /*--cpp*/
        arg_help["--cpp"] = "Only output equivalent C++ files";
        /*--check*/
        arg_help["--check"] = "Only check code for errors and warnings";
        /*-g*/
        arg_help["-g"] = "Enable debugging flags for use with gdb";
        /*-h, --help*/
        arg_help["-h, --help"] = "Prints help text";
        /*-o*/
        arg_help["-o"] = "Specify output file name";
        /*-v,--verbose*/
        arg_help["-v, --verbose"] = "Enable verbose output";
        /*--sacrifice*/
        arg_help["--sacrifice"] = "!UNIMPLEMENTED!--Output a binary or C++ file if code is valid with no warnings or errors. Deletes input source file(s) otherwise";
        /*--sacrifice=anyways*/
        arg_help["--sacrifice=anyways"] = "!UNIMPLEMENTED!--Same as above, but deletes the file(s) regardless";
        /*--version*/
        arg_help["--version, -V"] = "Prints compiler version";
        /*--nocodes*/
        arg_help["--nocodes, -N"] = "Do not display \"hat codes\" for warnings or errors (i.e. derby, fedora, etc.)";
        /*-w*/
        arg_help["-w"] = "Strict mode. If warnings are generated, final compilation will not commence.";
        /*-Wall*/
        arg_help["-Wall"] = "Enable all warnings";

        for (auto a : valid_args){
            args[a] = false;
        }
    }
};