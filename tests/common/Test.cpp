#include "Test.hpp"

#include <cero/driver/BuildCommand.hpp>

namespace tests {

cero::SourceGuard make_test_source(std::string_view source_text) {
	std::string_view name = get_current_test_name();
	return cero::Source::from_string(name, source_text).lock().value();
}

void build_test_source(cero::Reporter& reporter, std::string_view source_text) {
	std::string_view name = get_current_test_name();
	auto source = cero::Source::from_string(name, source_text);

	cero::CompilerOptions options;
	cero::try_build_source(source, reporter, options);
}

} // namespace tests
