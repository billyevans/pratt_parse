#include "symbol_table.h"

#include <stdexcept>

#include "parse_state.h"

void symbol_table_t::add_symbol(char const *str, size_t size, create_callback callback)
{
	str_t s(str, size);

	table.insert(std::make_pair(s, callback));
}

symbol_base_t *symbol_table_t::get_symbol(char const *str, size_t size) const
{
    std::map<str_t, create_callback>::const_iterator fnd = table.find(str_t(str, size));

    if (fnd != table.end()) {
        return fnd->second(str, size);
    } else {
        return new symbol_value_t(str, size);
    }
}

symbol_base_t *symbol_infix_t::nud(parse_state_t &)
{
    return this;
}

symbol_base_t *symbol_infix_t::led(parse_state_t &ps, symbol_base_t *left)
{
    first = left;
    second = ps.expression(lbp);
    return this;
}

symbol_base_t *symbol_prefix_t::nud(parse_state_t &ps)
{
    first = ps.expression(lbp);
    second = NULL;
	return this;
}

symbol_base_t *symbol_prefix_t::led(parse_state_t &, symbol_base_t *)
{
	return this;
}

symbol_base_t *symbol_value_t::nud(parse_state_t &)
{
    return this;
}

symbol_base_t *symbol_value_t::led(parse_state_t &, symbol_base_t *)
{
    return this;
}
