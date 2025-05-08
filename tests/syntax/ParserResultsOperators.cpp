#include "AstCompare.hpp"
#include "common/ExhaustiveReporter.hpp"
#include "common/Test.hpp"

#include <cero/syntax/Parser.hpp>

namespace tests {

CERO_TEST(ParseAdditiveAndMultiplicativeOperators) {
	auto source = make_test_source(R"_____(
foo(int32 a, int32 b) -> int32 {
	let c = a + b;
	let d = a + b * c;
	let e = (d - a) / c;
	return e**2 * b;
}
)_____");

	ExhaustiveReporter r;
	auto ast = cero::run_parser_on_source(source, r, TabSize);
	CHECK(!ast.has_errors());

	AstCompare c(ast);
	c.root();
	c.function_definition(cero::AccessSpecifier::None, "foo", [](AstCompare& c) {
		c.function_parameter(cero::ParameterSpecifier::None, "a", [](AstCompare& c) {
			c.name_expr("int32");
		});
		c.function_parameter(cero::ParameterSpecifier::None, "b", [](AstCompare& c) {
			c.name_expr("int32");
		});
		c.function_output("", [](AstCompare& c) {
			c.name_expr("int32");
		});
		c.binding_statement(cero::BindingSpecifier::Let, "c", [](AstCompare& c) {
			c.binary_expr(cero::BinaryOperator::Add, [](AstCompare& c) {
				c.name_expr("a");
				c.name_expr("b");
			});
		});
		c.binding_statement(cero::BindingSpecifier::Let, "d", [](AstCompare& c) {
			c.binary_expr(cero::BinaryOperator::Add, [](AstCompare& c) {
				c.name_expr("a");
				c.binary_expr(cero::BinaryOperator::Mul, [](AstCompare& c) {
					c.name_expr("b");
					c.name_expr("c");
				});
			});
		});
		c.binding_statement(cero::BindingSpecifier::Let, "e", [](AstCompare& c) {
			c.binary_expr(cero::BinaryOperator::Div, [](AstCompare& c) {
				c.group_expr([](AstCompare& c) {
					c.binary_expr(cero::BinaryOperator::Sub, [](AstCompare& c) {
						c.name_expr("d");
						c.name_expr("a");
					});
				});
				c.name_expr("c");
			});
		});
		c.return_expr([](AstCompare& c) {
			c.binary_expr(cero::BinaryOperator::Mul, [](AstCompare& c) {
				c.binary_expr(cero::BinaryOperator::Pow, [](AstCompare& c) {
					c.name_expr("e");
					c.numeric_literal_expr(cero::NumericLiteralKind::Decimal);
				});
				c.name_expr("b");
			});
		});
	});
}

CERO_TEST(ParseAdditiveAndComparisonOperators) {
	auto source = make_test_source(R"_____(
bar(int32 a, int32 b, int32 c) -> bool {
	let u = a - b == b + c;
	let v = b * a != c / a;
	let w = c + b > b * a;
	let x = b / a < c - b;
	let y = a * c <= b - a;
	let z = b + c >= a / c;
	return u || v || w || x || y || z;
}
)_____");

	ExhaustiveReporter r;
	auto ast = cero::run_parser_on_source(source, r, TabSize);
	CHECK(!ast.has_errors());

	AstCompare c(ast);
	c.root();
	c.function_definition(cero::AccessSpecifier::None, "bar", [](AstCompare& c) {
		c.function_parameter(cero::ParameterSpecifier::None, "a", [](AstCompare& c) {
			c.name_expr("int32");
		});
		c.function_parameter(cero::ParameterSpecifier::None, "b", [](AstCompare& c) {
			c.name_expr("int32");
		});
		c.function_parameter(cero::ParameterSpecifier::None, "c", [](AstCompare& c) {
			c.name_expr("int32");
		});
		c.function_output("", [](AstCompare& c) {
			c.name_expr("bool");
		});
		c.binding_statement(cero::BindingSpecifier::Let, "u", [](AstCompare& c) {
			c.binary_expr(cero::BinaryOperator::Eq, [](AstCompare& c) {
				c.binary_expr(cero::BinaryOperator::Sub, [](AstCompare& c) {
					c.name_expr("a");
					c.name_expr("b");
				});
				c.binary_expr(cero::BinaryOperator::Add, [](AstCompare& c) {
					c.name_expr("b");
					c.name_expr("c");
				});
			});
		});
		c.binding_statement(cero::BindingSpecifier::Let, "v", [](AstCompare& c) {
			c.binary_expr(cero::BinaryOperator::NotEq, [](AstCompare& c) {
				c.binary_expr(cero::BinaryOperator::Mul, [](AstCompare& c) {
					c.name_expr("b");
					c.name_expr("a");
				});
				c.binary_expr(cero::BinaryOperator::Div, [](AstCompare& c) {
					c.name_expr("c");
					c.name_expr("a");
				});
			});
		});
		c.binding_statement(cero::BindingSpecifier::Let, "w", [](AstCompare& c) {
			c.binary_expr(cero::BinaryOperator::Greater, [](AstCompare& c) {
				c.binary_expr(cero::BinaryOperator::Add, [](AstCompare& c) {
					c.name_expr("c");
					c.name_expr("b");
				});
				c.binary_expr(cero::BinaryOperator::Mul, [](AstCompare& c) {
					c.name_expr("b");
					c.name_expr("a");
				});
			});
		});
		c.binding_statement(cero::BindingSpecifier::Let, "x", [](AstCompare& c) {
			c.binary_expr(cero::BinaryOperator::Less, [](AstCompare& c) {
				c.binary_expr(cero::BinaryOperator::Div, [](AstCompare& c) {
					c.name_expr("b");
					c.name_expr("a");
				});
				c.binary_expr(cero::BinaryOperator::Sub, [](AstCompare& c) {
					c.name_expr("c");
					c.name_expr("b");
				});
			});
		});
		c.binding_statement(cero::BindingSpecifier::Let, "y", [](AstCompare& c) {
			c.binary_expr(cero::BinaryOperator::LessEq, [](AstCompare& c) {
				c.binary_expr(cero::BinaryOperator::Mul, [](AstCompare& c) {
					c.name_expr("a");
					c.name_expr("c");
				});
				c.binary_expr(cero::BinaryOperator::Sub, [](AstCompare& c) {
					c.name_expr("b");
					c.name_expr("a");
				});
			});
		});
		c.binding_statement(cero::BindingSpecifier::Let, "z", [](AstCompare& c) {
			c.binary_expr(cero::BinaryOperator::GreaterEq, [](AstCompare& c) {
				c.binary_expr(cero::BinaryOperator::Add, [](AstCompare& c) {
					c.name_expr("b");
					c.name_expr("c");
				});
				c.binary_expr(cero::BinaryOperator::Div, [](AstCompare& c) {
					c.name_expr("a");
					c.name_expr("c");
				});
			});
		});
		c.return_expr([](AstCompare& c) {
			c.binary_expr(cero::BinaryOperator::LogicOr, [](AstCompare& c) {
				c.binary_expr(cero::BinaryOperator::LogicOr, [](AstCompare& c) {
					c.binary_expr(cero::BinaryOperator::LogicOr, [](AstCompare& c) {
						c.binary_expr(cero::BinaryOperator::LogicOr, [](AstCompare& c) {
							c.binary_expr(cero::BinaryOperator::LogicOr, [](AstCompare& c) {
								c.name_expr("u");
								c.name_expr("v");
							});
							c.name_expr("w");
						});
						c.name_expr("x");
					});
					c.name_expr("y");
				});
				c.name_expr("z");
			});
		});
	});
}

CERO_TEST(ParseComparisonAndLogicalOperators) {
	auto source = make_test_source(R"_____(
baz(int32 a, int32 b, int32 c, int32 d) -> bool {
	return a + b == b + c && b + c != c + d && a < c && a > d;
}
)_____");

	ExhaustiveReporter r;
	auto ast = cero::run_parser_on_source(source, r, TabSize);
	CHECK(!ast.has_errors());

	AstCompare c(ast);
	c.root();
	c.function_definition(cero::AccessSpecifier::None, "baz", [](AstCompare& c) {
		c.function_parameter(cero::ParameterSpecifier::None, "a", [](AstCompare& c) {
			c.name_expr("int32");
		});
		c.function_parameter(cero::ParameterSpecifier::None, "b", [](AstCompare& c) {
			c.name_expr("int32");
		});
		c.function_parameter(cero::ParameterSpecifier::None, "c", [](AstCompare& c) {
			c.name_expr("int32");
		});
		c.function_parameter(cero::ParameterSpecifier::None, "d", [](AstCompare& c) {
			c.name_expr("int32");
		});
		c.function_output("", [](AstCompare& c) {
			c.name_expr("bool");
		});
		c.return_expr([](AstCompare& c) {
			c.binary_expr(cero::BinaryOperator::LogicAnd, [](AstCompare& c) {
				c.binary_expr(cero::BinaryOperator::LogicAnd, [](AstCompare& c) {
					c.binary_expr(cero::BinaryOperator::LogicAnd, [](AstCompare& c) {
						c.binary_expr(cero::BinaryOperator::Eq, [](AstCompare& c) {
							c.binary_expr(cero::BinaryOperator::Add, [](AstCompare& c) {
								c.name_expr("a");
								c.name_expr("b");
							});
							c.binary_expr(cero::BinaryOperator::Add, [](AstCompare& c) {
								c.name_expr("b");
								c.name_expr("c");
							});
						});
						c.binary_expr(cero::BinaryOperator::NotEq, [](AstCompare& c) {
							c.binary_expr(cero::BinaryOperator::Add, [](AstCompare& c) {
								c.name_expr("b");
								c.name_expr("c");
							});
							c.binary_expr(cero::BinaryOperator::Add, [](AstCompare& c) {
								c.name_expr("c");
								c.name_expr("d");
							});
						});
					});
					c.binary_expr(cero::BinaryOperator::Less, [](AstCompare& c) {
						c.name_expr("a");
						c.name_expr("c");
					});
				});
				c.binary_expr(cero::BinaryOperator::Greater, [](AstCompare& c) {
					c.name_expr("a");
					c.name_expr("d");
				});
			});
		});
	});
}

} // namespace tests
