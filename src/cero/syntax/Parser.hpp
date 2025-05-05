#pragma once

#include "cero/report/Reporter.hpp"
#include "cero/syntax/Ast.hpp"
#include "cero/syntax/Source.hpp"
#include "cero/syntax/TokenStream.hpp"

namespace cero {

Ast run_parser(const TokenStream& token_stream, const SourceGuard& source, Reporter& reporter, uint8_t tab_size);

Ast run_parser_on_source(const SourceGuard& source, Reporter& reporter, uint8_t tab_size);

} // namespace cero
