#pragma once

#include "cero/syntax/source.hpp"

namespace cero {

enum class AstNodeKind {
	root,
	function_def,
	function_parameter,
	function_output,
	block_statement,
	binding_statement,
	if_expr,
	while_loop,
	for_loop,
	identifier_expr
};

template<AstNodeKind kind>
struct AstHeader {
	const AstNodeKind node_kind : 8 = kind;
	const SourceSize offset : source_size_bits;

	AstHeader(SourceSize offset) :
		offset(offset) {
	}

	CodeLocation locate_in(const SourceView& source, uint8_t tab_size) const {
		return source.locate(offset, tab_size);
	}
};

struct AstRoot {
	AstHeader<AstNodeKind::root> header;
};

enum class AccessModifier : uint8_t {
	none,
	mod_private,
	mod_public,
};

struct AstFunctionDefinition {
	AstHeader<AstNodeKind::function_def> header;
	AccessModifier access_modifier;
};

} // namespace cero
