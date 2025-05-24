#pragma once

#include "cero/report/reporter.hh"
#include "cero/syntax/source.hh"
#include "cero/syntax/token_list.hh"

namespace cero {

/// Performs lexical analysis on a single source input.
TokenList run_lexer(const SourceView& source, Reporter& reporter, bool emit_comments, uint8_t tab_size = default_tab_size);

} // namespace cero
