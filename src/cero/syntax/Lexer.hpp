#pragma once

#include "cero/report/Reporter.hpp"
#include "cero/syntax/Source.hpp"
#include "cero/syntax/TokenStream.hpp"

namespace cero {

/// Performs lexical analysis on a single source input.
TokenStream run_lexer(const SourceGuard& source, Reporter& reporter, bool emit_comments, uint8_t tab_size);

} // namespace cero
