#pragma once

#include <cero/syntax/token_list.hpp>

namespace tests {

void check_tokens_equal(const cero::TokenList& tokens, std::span<const cero::Token> expected_tokens);

}
