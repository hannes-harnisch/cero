#pragma once

#include <cero/syntax/token.hpp>

namespace tests {

void check_tokens_equal(const char* source_text, std::span<const cero::Token> expected_tokens);

}
