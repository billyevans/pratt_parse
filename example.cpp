#include <stdio.h>
#include <math.h>

#include "symbol_table.h"
#include "parse_state.h"


static int calc(symbol_base_t *root)
{
	int left = -500, right = -500;
	bool left_valid = false;
	bool right_valid = false;

	if (root->first) {
		left = calc(root->first);
		left_valid = true;
	}

	if (root->second) {
		right = calc(root->second);
		right_valid = true;
	}

	switch(*root->str) {
		default:
			return strtol(root->str, NULL, 10);
		case '+':
			if (left_valid && right_valid)
				return left + right;
			else if (left_valid)
				return left;
		case '-':
			if (left_valid && right_valid)
				return left - right;
			else if (left_valid)
				return -left;
		case '*':
			if (root->size == 1) {
				return left * right;
			} else if (root->size == 2 && root->str[1] == '*') { // '**'
				return pow(left, right);
			}
		case '/':
			return left / right;
		case '%':
			return left % right;
		case '~':
			return ~left;
		case '|':
			return left | right;
		case '&':
			return left & right;
		case '^':
			return left ^ right;
		case '<':
			if (root->size == 2 && root->str[1] == '<')
				return left << right;
		case '>':
			if (root->size == 2 && root->str[1] == '>')
				return left >> right;
	}
	return -1;
}

int main(int argc, char **argv)
{

	symbol_table_t st;

	// add symbols to st;

	PRE_IN_FIX(st, "+", 130, 110);
	PRE_IN_FIX(st, "-", 130, 110);

	INFIX(st, "*", 120);
	INFIX(st, "/", 120);
	INFIX(st, "%", 120);

	INFIX(st, "|", 70);
	INFIX(st, "^", 80);
	INFIX(st, "&", 90);

	INFIX(st, "<<", 100);
	INFIX(st, ">>", 100);

	PREFIX(st, "~", 130);

	INFIX_R(st, "**", 140);

//	SYMBOL(st, "(", 150);
	// usage
	{
		//char line[500];
		if (argc != 2)
			return 1;
		char *line = argv[1];
		//while (fgets(line, sizeof(line), stdin)) {
			parse_state_t ps(st);
			symbol_base_t *root = ps.parse(line, strlen(line));
			printf ("%d\n", calc(root));

			delete root;
		//}
	}

	return 0;
}