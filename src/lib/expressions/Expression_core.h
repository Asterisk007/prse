/**
 * Expression_core.h
 * Contains core includes and definitions for the expressions component of lib.
 * Much more centralized than including each of these components
 * separately per file, plus with #pragma once used everywhere it hopefully shouldn't
 * clog up the works.
 */

#pragma once
#include "Expression.h"
#include "Program_expression.h"
#include "Constant.h"
#include "../Table_handler.h"
#include "../Declared_list.h"
#include "../Error.h"
#include "../PRSE_stdlib.h"
#include <iostream>

//static int VALID_MATH_TYPES = (PRSE_type::T_DOUBLE | PRSE_type::T_INT);