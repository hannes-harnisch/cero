#include "AstCompare.hpp"
#include "common/ExhaustiveReporter.hpp"
#include "common/Test.hpp"

#include <cero/syntax/Parser.hpp>

namespace tests {

CERO_TEST(ParseWhileLoopFibonacci) {
	auto source = make_test_source(R"_____(
fibonacci(var uint32 n) -> uint32 {
    var uint32 result = 0;
    var uint32 next   = 1;

    while n-- != 0 {
        let temp = next;
        next = result;
        result += temp;
    }

    return result;
}
)_____");

	ExhaustiveReporter r;
	auto ast = cero::run_parser_on_source(source, r, TabSize);
	CHECK(!ast.has_errors());

	AstCompare c(ast);
	c.root();
	c.function_definition(cero::AccessSpecifier::None, "fibonacci", [](AstCompare& c) {
		c.function_parameter(cero::ParameterSpecifier::Var, "n", [](AstCompare& c) {
			c.name_expr("uint32");
		});
		c.function_output("", [](AstCompare& c) {
			c.name_expr("uint32");
		});
		c.binding_statement(cero::BindingSpecifier::Var, "result", [](AstCompare& c) {
			c.name_expr("uint32");
			c.numeric_literal_expr(cero::NumericLiteralKind::Decimal);
		});
		c.binding_statement(cero::BindingSpecifier::Var, "next", [](AstCompare& c) {
			c.name_expr("uint32");
			c.numeric_literal_expr(cero::NumericLiteralKind::Decimal);
		});
		c.while_loop([](AstCompare& c) {
			c.binary_expr(cero::BinaryOperator::NotEq, [](AstCompare& c) {
				c.unary_expr(cero::UnaryOperator::PostDec, [](AstCompare& c) {
					c.name_expr("n");
				});
				c.numeric_literal_expr(cero::NumericLiteralKind::Decimal);
			});
			c.binding_statement(cero::BindingSpecifier::Let, "temp", [](AstCompare& c) {
				c.name_expr("next");
			});
			c.binary_expr(cero::BinaryOperator::Assign, [](AstCompare& c) {
				c.name_expr("next");
				c.name_expr("result");
			});
			c.binary_expr(cero::BinaryOperator::AddAssign, [](AstCompare& c) {
				c.name_expr("result");
				c.name_expr("temp");
			});
		});
		c.return_expr([](AstCompare& c) {
			c.name_expr("result");
		});
	});
}

CERO_TEST(ParseIfStatement) {
	auto source = make_test_source(R"_____(
public divChecked(int32 a, int32 b) -> Opt<int32> {
	if b == 0 {
		return null;
	} else {
		return a / b;
	}
}
)_____");

	ExhaustiveReporter r;
	auto ast = cero::run_parser_on_source(source, r, TabSize);
	CHECK(!ast.has_errors());

	AstCompare c(ast);
	c.root();
	c.function_definition(cero::AccessSpecifier::Public, "divChecked", [](AstCompare& c) {
		c.function_parameter(cero::ParameterSpecifier::None, "a", [](AstCompare& c) {
			c.name_expr("int32");
		});
		c.function_parameter(cero::ParameterSpecifier::None, "b", [](AstCompare& c) {
			c.name_expr("int32");
		});
		c.function_output("", [](AstCompare& c) {
			c.generic_name_expr("Opt", [](AstCompare& c) {
				c.name_expr("int32");
			});
		});
		c.if_expr([](AstCompare& c) {
			c.binary_expr(cero::BinaryOperator::Eq, [](AstCompare& c) {
				c.name_expr("b");
				c.numeric_literal_expr(cero::NumericLiteralKind::Decimal);
			});
			c.return_expr([](AstCompare& c) {
				c.name_expr("null");
			});
			c.return_expr([](AstCompare& c) {
				c.binary_expr(cero::BinaryOperator::Div, [](AstCompare& c) {
					c.name_expr("a");
					c.name_expr("b");
				});
			});
		});
	});
}

} // namespace tests
