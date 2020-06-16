#pragma once
#include <string>

enum Type {
	BOOL,
	INT,
	CHAR,
	DOUBLE,
	STRING,
	VOID
};

class Constant {
public:
	Constant();

private:
	Type consant_type;
	union {
		bool union_bool;
		int union_int;
		char union_char;
		double union_double;
		std::string* union_string;
	};

};