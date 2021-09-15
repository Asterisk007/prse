#include "Bulk_assign.h"
#include "Expression_core.h"
#include "Valid_math_types.h"

const Constant* Bulk_assign::as_const() const {
    string assign_type = "";
    switch (op_type) {
        case Bulk_assign_type::BULK_ASSIGN:
            assign_type = "=";
            break;
        case Bulk_assign_type::BULK_INCREMENT:
            assign_type = "++";
            break;
        case Bulk_assign_type::BULK_DECREMENT:
            assign_type = "--";
            break;
        case Bulk_assign_type::BULK_PLUS_ASSIGN:
            assign_type = "+=";
            break;
        case Bulk_assign_type::BULK_MINUS_ASSIGN:
            assign_type = "-=";
            break;
        case Bulk_assign_type::BULK_MULTIPLY_ASSIGN:
            assign_type = "*=";
            break;
        case Bulk_assign_type::BULK_DIVIDE_ASSIGN:
            assign_type = "/=";
            break;
        default:
            #ifdef DEBUG
                printf("Bulk assign type has no case in switch in Bulk_assign.cpp");
            #endif
            Error::error(Error::OTHER, "CRITICAL ERROR: No case in switch statement for bulk assign type " + op_type);
    }

    if (lhs != nullptr && rhs != nullptr && assign_type != "") {
    // Return string for =, +=, -=, *=, /=
        auto l = lhs->as_const();
        auto r = rhs->as_const();
        if (!(l->type() & VALID_MATH_TYPES)){
            Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "lhs", prse_type_to_string(l->type()), "bulk assign", line);
        }
        if (!(r->type() & VALID_MATH_TYPES)){
            Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "rhs", prse_type_to_string(l->type()), "bulk assign", line);
        }
        string it_name = l->value() + "_iterator";
        string t = "for (int " + it_name + " = 0; i < " + it_name + ".size(); " + it_name + "++";
        t += l->value() + assign_type + r->value();
        return ret(new Constant(l->type(), t));
    } else if (lhs != nullptr && assign_type != "") {
    // Return string for ++, --
        auto l = lhs->as_const();
        if (!(l->type() & VALID_MATH_TYPES)){
            Error::error(Error::INVALID_EXPRESSION_TYPE_FOR_OPERATION, "lhs", prse_type_to_string(l->type()), "bulk assign", line);
        }
        string it_name = l->value() + "_iterator";
        string t = "for (int " + it_name + " = 0; i < " + it_name + ".size(); " + it_name + "++";
        t += l->value() + assign_type;
        return ret(new Constant(l->type(), t));
    } else {
        return ret(new Constant(PRSE_type::NO_TYPE, ""));
    }
}

vector<const Constant*> Bulk_assign::as_list() const {
    vector<const Constant*> cl = vector<const Constant*>();
    const Constant* c = this->as_const();
    if (c->type() != PRSE_type::NO_TYPE)
        cl.push_back(c);
    return cl;
}