#pragma once

#include "symbol_table.h"

class parse_state_t {
	char const *pos;
	symbol_base_t *token;
	symbol_table_t const &st;

	//struct parse_tree *first;

	enum char_type {
		CHAR_TYPE_OPERATION,
		CHAR_TYPE_BRACE,
		CHAR_TYPE_NAME,
		CHAR_TYPE_NUMBER,
		CHAR_TYPE_SPACE
	};

	inline enum char_type get_char_type(char c) {
		enum char_type t = CHAR_TYPE_OPERATION;

		switch ((unsigned char)c) {
			case 'a' ... 'z':
			case 'A' ... 'Z':
			case '_':
				t = CHAR_TYPE_NAME;
				break;
			case '0' ... '9':
				t = CHAR_TYPE_NUMBER;
				break;
			case ' ': case '\f': case '\n': case '\r': case '\t' : case '\v':
				t = CHAR_TYPE_SPACE;
				break;
			case '(' : case ')': case ',':
				t = CHAR_TYPE_BRACE;
				break;
		}
		return t;
	}	
	virtual symbol_base_t *get_next_token();

//	int err;
public:
	parse_state_t(symbol_table_t const &symb) : pos(NULL), token(NULL), st(symb)	 { }
	virtual ~parse_state_t() { }

	symbol_base_t *expression(int rbp = 0);
	symbol_base_t *parse(char const *str);
};