#pragma once

#include "cero/report/reporter.hpp"
#include "cero/syntax/ast.hpp"
#include "cero/syntax/source.hpp"
#include "cero/syntax/token_list.hpp"

namespace cero {

Ast run_parser(const TokenList& token_list, const SourceView& source, Reporter& reporter, uint8_t tab_size);

Ast run_parser_on_source(const SourceView& source, Reporter& reporter, uint8_t tab_size);

} // namespace cero
