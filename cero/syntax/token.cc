#include "token.hh"

namespace cero {

CodeLocation Token::locate_in(const SourceView& source, uint8_t tab_size) const {
	return source.locate(offset, tab_size);
}

} // namespace cero
