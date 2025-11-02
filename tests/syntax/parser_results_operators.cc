#include "common/test.hh"
#include "syntax/ast_compare.hh"

namespace tests {

CERO_TEST(parser_comparison_and_logical_operators) {
	const char* source = R"_____(
baz(int32 a, int32 b, int32 c, int32 d) -> bool {
	return a + b == b + c && b + c != c + d && a < c && a > d;
}
)_____";

	AstCompare c(source);
	c.function_definition(cero::AccessModifier::none, "baz", [&] {
		c.function_parameter("a", [&] {
			c.ident_expr("int32");
		});
		c.function_parameter("b", [&] {
			c.ident_expr("int32");
		});
		c.function_parameter("c", [&] {
			c.ident_expr("int32");
		});
		c.function_parameter("d", [&] {
			c.ident_expr("int32");
		});
		c.function_output("", [&] {
			c.ident_expr("bool");
		});
		c.return_expr([&] {
			c.binary_expr(cero::BinaryOperator::logic_and, [&] {
				c.binary_expr(cero::BinaryOperator::logic_and, [&] {
					c.binary_expr(cero::BinaryOperator::logic_and, [&] {
						c.binary_expr(cero::BinaryOperator::eq, [&] {
							c.binary_expr(cero::BinaryOperator::add, [&] {
								c.ident_expr("a");
								c.ident_expr("b");
							});
							c.binary_expr(cero::BinaryOperator::add, [&] {
								c.ident_expr("b");
								c.ident_expr("c");
							});
						});
						c.binary_expr(cero::BinaryOperator::neq, [&] {
							c.binary_expr(cero::BinaryOperator::add, [&] {
								c.ident_expr("b");
								c.ident_expr("c");
							});
							c.binary_expr(cero::BinaryOperator::add, [&] {
								c.ident_expr("c");
								c.ident_expr("d");
							});
						});
					});
					c.binary_expr(cero::BinaryOperator::less, [&] {
						c.ident_expr("a");
						c.ident_expr("c");
					});
				});
				c.binary_expr(cero::BinaryOperator::greater, [&] {
					c.ident_expr("a");
					c.ident_expr("d");
				});
			});
		});
	});
}

} // namespace tests
