#pragma once

#include <stdlib.h>
#include <string.h>
#include <map>

class parse_state_t;

class symbol_base_t {
public:
	symbol_base_t(char const *str_, size_t size_, int lbp_) :
			str(str_), size(size_), first(NULL), second(NULL), lbp(lbp_)  { }

	virtual ~symbol_base_t() {
		if (first) 
			delete first;
		if (second)
			delete second; 
	}

	char const *str;
	size_t size;
	symbol_base_t *first, *second;
	int lbp;

	virtual symbol_base_t *nud(parse_state_t &ps) = 0;
	virtual symbol_base_t *led(parse_state_t &ps, symbol_base_t *left) = 0;
};

#define SYMBOL(st_, str_, lbp_) \
	st_.add_symbol(str_, sizeof(str_)-1, \
		[](char const *s_, size_t size_) -> symbol_base_t* { return new symbol_value_t(s_, size_, lbp_); }); \

#define INFIX(st_, str_, lbp_) \
	st_.add_symbol(str_, sizeof(str_)-1, \
		[](char const *s_, size_t size_) -> symbol_base_t* { return new symbol_infix_t(s_, size_, lbp_); }); \

#define INFIX_R(st_, str_, lbp_) \
	st_.add_symbol(str_, sizeof(str_)-1, \
		[](char const *s_, size_t size_) -> symbol_base_t* { return new symbol_infix_r_t(s_, size_, lbp_); }); \

#define PREFIX(st_, str_, lbp_) \
	st_.add_symbol(str_, sizeof(str_)-1, \
		[](char const *s_, size_t size_) -> symbol_base_t* { return new symbol_prefix_t(s_, size_, lbp_); }); \

#define PRE_IN_FIX(st_, str_, pre_lbp_, in_lbp_) \
	st_.add_symbol(str_, sizeof(str_)-1, \
		[](char const *s_, size_t size_) -> symbol_base_t* { return new symbol_pre_in_fix_t(s_, size_, pre_lbp_, in_lbp_); }); \

class symbol_infix_t : public symbol_base_t {
public:
	symbol_infix_t(char const *str_, size_t size_, int lbp_) :
			symbol_base_t(str_, size_, lbp_) { }

	virtual symbol_base_t *nud(parse_state_t &);
	virtual symbol_base_t *led(parse_state_t &ps, symbol_base_t *left);
};

class symbol_infix_r_t : public symbol_base_t {
public:
	symbol_infix_r_t(char const *str_, size_t size_, int lbp_) :
			symbol_base_t(str_, size_, lbp_) { }

	virtual symbol_base_t *nud(parse_state_t &);
	virtual symbol_base_t *led(parse_state_t &ps, symbol_base_t *left);
};

class symbol_prefix_t : public symbol_base_t {
public:
	symbol_prefix_t(char const *str_, size_t size_, int lbp_) :
			symbol_base_t(str_, size_, lbp_) { }

	virtual symbol_base_t *nud(parse_state_t &);
	virtual symbol_base_t *led(parse_state_t &ps, symbol_base_t *left);
};

class symbol_pre_in_fix_t : public symbol_base_t {
	int pre_lbp, in_lbp;
public:
	symbol_pre_in_fix_t(char const *str_, size_t size_, int pre_lbp_, int in_lbp_) :
			symbol_base_t(str_, size_, std::max(pre_lbp_, in_lbp_)), pre_lbp(pre_lbp_),in_lbp(in_lbp_) { }

	virtual symbol_base_t *nud(parse_state_t &ps);
	virtual symbol_base_t *led(parse_state_t &ps, symbol_base_t *left);
};

class symbol_value_t : public symbol_base_t {
public:
	symbol_value_t(char const *str_, size_t size_, int lbp_ = 0) :
			symbol_base_t(str_, size_, lbp_) { }

	virtual symbol_base_t *nud(parse_state_t &);
	virtual symbol_base_t *led(parse_state_t &ps, symbol_base_t *left);
};


// factory for producing symbols
class symbol_table_t {
public:
	typedef symbol_base_t *(*create_callback)(char const *str, size_t size);
private:
	struct str_t {
		char const *s;
		size_t size;
		str_t(char const *s_, size_t size_) : s(s_), size(size_) { }
		bool operator < (str_t const &r) const {
			return (size < r.size || (size == r.size && memcmp(s, r.s, size) < 0));
		}
	};
	std::map<str_t, create_callback> table;
public:
	symbol_table_t() { }
	~symbol_table_t() { }

	void add_symbol(char const *str, size_t size, create_callback callback);
	symbol_base_t *get_symbol(char const *str, size_t size) const;
};
