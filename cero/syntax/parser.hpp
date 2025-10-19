#pragma once

#include "cero/report/reporter.hpp"
#include "cero/syntax/ast.hpp"
#include "cero/syntax/source.hpp"
#include "cero/syntax/token_list.hpp"

namespace cero {

/// Performs syntactic analysis on the given token list.
Ast run_parser(const TokenList& token_list, const SourceView& source, Reporter& reporter, uint8_t tab_size = default_tab_size);

/// Performs lexical and syntactic analysis on a single source input.
Ast run_parser_on_source(const SourceView& source, Reporter& reporter, uint8_t tab_size = default_tab_size);

} // namespace cero
