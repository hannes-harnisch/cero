#include "common/test.hpp"
#include "syntax/ast_compare.hpp"

namespace tests {

CERO_TEST(parser_empty_source) {
	const char* source = "";

	AstCompare c(source);
}

CERO_TEST(parser_empty_function) {
	const char* source = "main() {}";

	AstCompare c(source);

	c.function_definition(cero::AccessModifier::none, "main", [] {});
}

} // namespace tests
