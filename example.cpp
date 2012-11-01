#include <stdio.h>
#include "symbol_table.h"
#include "parse_state.h"


static int calc(symbol_base_t *root)
{
	int left = -500, right = -500;

	if (root->first) {
		left = calc(root->first);
	}

	if (root->second) {
		right = calc(root->second);
	}

	switch(*root->str) {
		default:
			return strtol(root->str, NULL, 10);
		case '+':
			return left + right;
		case '-':
			return left - right;
		case '*':
			return left * right;
		case '/':
			return left / right;
		case '%':
			return left % right;
	}
	return -1;
}

int main()
{

	symbol_table_t st;

	// add symbols to st;

	INFIX(st, "+", 110);
	INFIX(st, "-", 110);

	INFIX(st, "*", 120);
	INFIX(st, "/", 120);
	INFIX(st, "%", 120);

//	PREFIX(st, "-", 130);
//	PREFIX(st, "+", 130);

//	INFIX_R(st, "**", 140);

//	SYMBOL(st, "(", 150);
	//usage
	{
		parse_state_t ps(st);

		symbol_base_t *root = ps.parse("1 + 2*13 - 8", sizeof("1 + 2*13 - 8")-1);
		printf ("%d\n", calc(root));
		//root->print();
		delete root;
	}

	return 0;
}