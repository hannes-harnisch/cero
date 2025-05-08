#include "AstCompare.hpp"

#include "common/ExhaustiveReporter.hpp"
#include "common/Test.hpp"

#include <cero/syntax/Parser.hpp>

namespace tests {

CERO_TEST(ParseEmptyFunction) {
	auto source = make_test_source(R"_____(
main() {
}
)_____");

	ExhaustiveReporter r;
	auto ast = cero::run_parser_on_source(source, r, TabSize);
	CHECK(!ast.has_errors());

	AstCompare c(ast);
	c.root();
	c.function_definition(cero::AccessSpecifier::None, "main", [](AstCompare&) {});
}

} // namespace tests
