#include "test.hh"

#include <cero/compile/build.hh>

namespace tests {

cero::SourceView make_test_source(const char* source_text) {
	const char* name = get_current_test_name();
	return cero::Source::from_string(source_text, name).lock().value();
}

void build_test_source(cero::Reporter& reporter, const char* source_text) {
	const char* name = get_current_test_name();
	cero::Source source = cero::Source::from_string(source_text, name);

	cero::build_source(source, reporter);
}

} // namespace tests
