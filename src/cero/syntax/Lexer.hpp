#pragma once

#include "cero/report/Reporter.hpp"
#include "cero/syntax/Source.hpp"
#include "cero/syntax/TokenStream.hpp"

namespace cero {

TokenStream run_lexer(const SourceGuard& source, Reporter& reporter, bool emit_comments, uint8_t tab_size);

}
