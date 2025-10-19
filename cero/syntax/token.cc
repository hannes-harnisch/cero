#include "token.hh"

#include "cero/util/assert.hh"

namespace cero {

CodeLocation Token::locate_in(const SourceView& source, uint8_t tab_size) const {
	return source.locate(offset, tab_size);
}

std::string_view Token::get_text(const SourceView& source) const {
	if (variable_length > 0) {
		return source.get_text().substr(offset, variable_length);
	}
	else {
		using enum TokenKind;
		switch (kind) {
		case kw_break:         return "break";
		case kw_catch:         return "catch";
		case kw_const:         return "const";
		case kw_continue:      return "continue";
		case kw_do:            return "do";
		case kw_else:          return "else";
		case kw_enum:          return "enum";
		case kw_for:           return "for";
		case kw_if:            return "if";
		case kw_in:            return "in";
		case kw_interface:     return "interface";
		case kw_let:           return "let";
		case kw_mut:           return "mut";
		case kw_private:       return "private";
		case kw_public:        return "public";
		case kw_raise:         return "raise";
		case kw_return:        return "return";
		case kw_static:        return "static";
		case kw_struct:        return "struct";
		case kw_switch:        return "switch";
		case kw_try:           return "try";
		case kw_unchecked:     return "unchecked";
		case kw_while:         return "while";
		case dot:              return ".";
		case comma:            return ",";
		case colon:            return ":";
		case semicolon:        return ";";
		case l_brace:          return "{";
		case r_brace:          return "}";
		case l_paren:          return "(";
		case r_paren:          return ")";
		case l_bracket:        return "[";
		case r_bracket:        return "]";
		case l_angle:          return "<";
		case r_angle:          return ">";
		case eq:               return "=";
		case plus:             return "+";
		case minus:            return "-";
		case star:             return "*";
		case slash:            return "/";
		case percent:          return "%";
		case ampersand:        return "&";
		case pipe:             return "|";
		case tilde:            return "~";
		case caret:            return "^";
		case bang:             return "!";
		case thin_arrow:       return "->";
		case thick_arrow:      return "=>";
		case colon_colon:      return "::";
		case plus_plus:        return "++";
		case minus_minus:      return "--";
		case l_angle_angle:    return "<<";
		case and_and:          return "&&";
		case pipe_pipe:        return "||";
		case eq_eq:            return "==";
		case bang_eq:          return "!=";
		case l_angle_eq:       return "<=";
		case r_angle_eq:       return ">=";
		case plus_eq:          return "+=";
		case minus_eq:         return "-=";
		case star_eq:          return "*=";
		case slash_eq:         return "/=";
		case percent_eq:       return "%=";
		case ampersand_eq:     return "&=";
		case pipe_eq:          return "|=";
		case tilde_eq:         return "~=";
		case l_angle_angle_eq: return "<<=";
		case r_angle_angle_eq: return ">>=";
		case and_and_eq:       return "&&=";
		case pipe_pipe_eq:     return "||=";
		case end_of_file:      return "";
		default:               util::assert_unreachable();
		}
	}
}

std::string_view get_token_message_format(TokenKind kind) {
	using enum TokenKind;
	switch (kind) {
	case identifier:      return "name `{}`";
	case line_comment:
	case block_comment:   return "comment";
	case dec_int_literal:
	case hex_int_literal:
	case bin_int_literal:
	case oct_int_literal: return "integer literal `{}`";
	case float_literal:   return "floating-point literal `{}`";
	case char_literal:    return "character literal {}";
	case string_literal:  return "string literal {}";
	case end_of_file:     return "end of file";
	default:              return "`{}`";
	}
}

} // namespace cero
