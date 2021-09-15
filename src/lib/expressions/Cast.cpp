#include "Cast.h"
#include "Constant.h"
#include "Expression_core.h"

// Variable casting
Cast::Cast(int line, const Expression* lhs, PRSE_type cast_to)
    :Unary_operator(lhs), line(line), cast_type(cast_to){}

const Constant* Cast::as_const() const {
    /*
        Casting chart:
        bool ==> int, double, char, string
        int ==> string, double, bool (Values that are exactly equal to 0, such as 0, 0.0, and so on, are interpreted as false. Values higher than 0 are interpreted as true.)
        double ==> string, int, bool (ditto as above)
        char ==> string, int, double
        string ==> string, int, double
     */
    string ret_s = "";
    if (lhs != nullptr){
        switch (cast_type){
            case T_BOOL:
                ret_s = "static_cast<bool>(";
                ret_s += lhs->as_const()->value();
                ret_s += ")";
                break;
            case T_INT:
                ret_s = "static_cast<int>(";
                ret_s += lhs->as_const()->value();
                ret_s += ")";
                break;
            case T_DOUBLE:
                ret_s = "static_cast<double>(";
                ret_s += lhs->as_const()->value();
                ret_s += ")";
                break;
            case T_CHAR:
                ret_s = "static_cast<char>(";
                ret_s += lhs->as_const()->value();
                ret_s += ")";
                break;
            case T_STRING:
                ret_s = "string(";
                ret_s += lhs->as_const()->value();
                ret_s += ")";
                break;
            default:
                cerr << "Warning on line " << line << ": this operation is currently not supported. The original expression will be used." << endl;
                ret_s = lhs->as_const()->value();
                break;
        }
    }
    return ret(new Constant(cast_type, ret_s));
}

string Cast::value() const {
    return "";
}

vector<const Constant*> Cast::as_list() const {
    return vector<const Constant*>(0);
}
