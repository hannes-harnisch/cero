#pragma once

#include "cero/report/reporter.hh"
#include "cero/syntax/source.hh"
#include "cero/syntax/token_list.hh"
#include "cero/util/flags.hh"

namespace cero {

enum class LexerFlags {
	none = 0,
	emit_comments = 1 << 0,
};

/// Performs lexical analysis on a single source input.
TokenList run_lexer(const SourceView& source, Reporter& reporter, LexerFlags flags, uint8_t tab_size = default_tab_size);

} // namespace cero

template<>
struct util::is_flag_enum<cero::LexerFlags> : std::true_type {};
