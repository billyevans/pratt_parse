#include "symbol_table.h"
#include "parse_state.h"

int main()
{

	symbol_table_t st;

	// add symbols to st;

	//SYMBOL(st, "(literal)", 0);
	INFIX(st, "+", 10);
	INFIX(st, "-", 10);
	INFIX(st, "*", 20);
	INFIX(st, "/", 20);
	//SYMBOL(st, "**", 30);
	//SYMBOL(st, "(end)", 0);

	//usage
	{
		parse_state_t ps(st);

		symbol_base_t *root = ps.parse("1 + 2");

		root->print();
		delete root;
	}

	{
//		parse_state ps(st);

//		ps.parse("(1 + 2)*3");
	}

	return 0;
}