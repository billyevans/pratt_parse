#include "parse_state.h"

#include <stdexcept>

/*
struct token *parse_state::get_token(char const *str, size_t size, enum char_type char_type)
{
	struct token *t = &token;
	t->str = str;
	t->size = size;
	t->lbp = 0;
	t->nud = NULL;
	t->led = NULL;
	t->node = NULL;
	
	if (str && size) {
		if (char_type <= CHAR_TYPE_NAME) {
			struct token_raw node = { .str = str, .size = size };
			struct token_raw *fnd = yabs_hash_find(&ps->table->symbol_table, &node);
			if (!fnd)
				throw std::runtime_error("unknown symbol!");
			
			t->node = parse_tree_init(fnd->stype, fnd->svalue);

			t->str = fnd->str;
			t->size = fnd->size;
			t->lbp = fnd->lbp;
			t->led = fnd->led;
			t->nud = fnd->nud;
		} else { // value
			raw_symbol_value val;
			val.val = str;
			val.val_size = size;
			
			t->nud = fnud;
			t->node = parse_tree_init(SYMBOL_TYPE_VALUE, (raw_symbol_value)val);

		}
		t->node->link = ps->first;
		ps->first = t->node;
	}
	return t;
}*/

symbol_base_t *parse_state_t::get_next_token()
{
	char const *p;
	char const *word = NULL;
	char const *word_end = NULL;
	enum char_type start_type, cur_type;
	symbol_base_t *prev_token;

	for (p = pos; p && (*p) && !word_end; ++p) {
		cur_type = get_char_type(*p);
		if (cur_type != CHAR_TYPE_SPACE) {
			// inside word
			if (word) {
				if (start_type != cur_type)
					word_end = p;
			} else { // this is start of word, detect type of the word
				word = p;
				start_type = cur_type;
			}
		} else {
			if (word)
				word_end = p;
		}
	}

	if (!p)
		throw std::runtime_error("wrong string");

	if (word && !word_end)
		word_end = p;
	
	pos = word_end;

	prev_token = this->token;
	this->token = st.get_symbol(word, word_end - word);
	return prev_token;
	//get_token(word, word_end - word, start_type);
}

symbol_base_t *parse_state_t::expression(int rbp)
{
	symbol_base_t *t = get_next_token();
	symbol_base_t *left = t->nud(*this);

	while (rbp < token->lbp) {

		t = get_next_token();

		left = t->led(*this, left);
	}
	return left;
}

symbol_base_t *parse_state_t::parse(char const *str)
{
	pos = str;
	//first = NULL;

	get_next_token();
	return expression();
}