#pragma once

#include "cero/report/reporter.hpp"
#include "cero/syntax/source.hpp"
#include "cero/syntax/token_list.hpp"
#include "cero/util/flags.hpp"

namespace cero {

enum class LexerFlags {
	none = 0,
	emit_comments = 1 << 0,
};

template<>
struct is_flag_enum<LexerFlags> : std::true_type {};

/// Performs lexical analysis on a single source input.
TokenList run_lexer(const SourceView& source, Reporter& reporter, LexerFlags flags, uint8_t tab_size = default_tab_size);

} // namespace cero
