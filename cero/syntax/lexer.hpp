#pragma once

#include "cero/syntax/source.hpp"
#include "cero/syntax/token_list.hpp"

namespace cero {

/// Performs lexical analysis on a single source input.
TokenList run_lexer(const SourceView& source, bool emit_comments, uint8_t tab_size = default_tab_size);

} // namespace cero
