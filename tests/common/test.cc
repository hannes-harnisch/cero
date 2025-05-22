#include "test.hh"

namespace tests {

cero::SourceView make_test_source(const char* source_text) {
	const char* name = get_current_test_name();
	return cero::Source::from_string(source_text, name).lock().value();
}

} // namespace tests
