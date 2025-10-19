#pragma once

#include "cero/report/reporter.hh"
#include "cero/syntax/ast.hh"
#include "cero/syntax/source.hh"
#include "cero/syntax/token_list.hh"

namespace cero {

/// Performs syntactic analysis on the given token list.
Ast run_parser(const TokenList& token_list, const SourceView& source, Reporter& reporter, uint8_t tab_size = default_tab_size);

} // namespace cero
