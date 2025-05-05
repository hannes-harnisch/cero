#include "AstCompare.hpp"
#include "common/ExhaustiveReporter.hpp"
#include "common/Test.hpp"

#include <cero/syntax/Parser.hpp>

namespace tests {

CERO_TEST(ParseGenericReturnType) {
	auto source = make_test_source(R"_____(
a() -> List<List<int32> > {
	return ();
}

b() -> List<List<int32>> {
	return ();
}

c() -> List<List<List<int32> > > {
	return ();
}

d() -> List<List<List<int32> >> {
	return ();
}

e() -> List<List<List<int32>> > {
	return ();
}

f() -> List<List<List<int32>>> {
	return ();
}
)_____");

	ExhaustiveReporter r;
	auto ast = cero::run_parser_on_source(source, r, TabSize);
	CHECK(!ast.has_errors());

	AstCompare c(ast);
	c.root();
	c.function_definition(cero::AccessSpecifier::None, "a", [](AstCompare& c) {
		c.function_output("", [](AstCompare& c) {
			c.generic_name_expr("List", [](AstCompare& c) {
				c.generic_name_expr("List", [](AstCompare& c) {
					c.name_expr("int32");
				});
			});
		});
		c.return_expr([](AstCompare& c) {
			c.group_expr([](AstCompare&) {});
		});
	});
	c.function_definition(cero::AccessSpecifier::None, "b", [](AstCompare& c) {
		c.function_output("", [](AstCompare& c) {
			c.generic_name_expr("List", [](AstCompare& c) {
				c.generic_name_expr("List", [](AstCompare& c) {
					c.name_expr("int32");
				});
			});
		});
		c.return_expr([](AstCompare& c) {
			c.group_expr([](AstCompare&) {});
		});
	});
	c.function_definition(cero::AccessSpecifier::None, "c", [](AstCompare& c) {
		c.function_output("", [](AstCompare& c) {
			c.generic_name_expr("List", [](AstCompare& c) {
				c.generic_name_expr("List", [](AstCompare& c) {
					c.generic_name_expr("List", [](AstCompare& c) {
						c.name_expr("int32");
					});
				});
			});
		});
		c.return_expr([](AstCompare& c) {
			c.group_expr([](AstCompare&) {});
		});
	});
	c.function_definition(cero::AccessSpecifier::None, "d", [](AstCompare& c) {
		c.function_output("", [](AstCompare& c) {
			c.generic_name_expr("List", [](AstCompare& c) {
				c.generic_name_expr("List", [](AstCompare& c) {
					c.generic_name_expr("List", [](AstCompare& c) {
						c.name_expr("int32");
					});
				});
			});
		});
		c.return_expr([](AstCompare& c) {
			c.group_expr([](AstCompare&) {});
		});
	});
	c.function_definition(cero::AccessSpecifier::None, "e", [](AstCompare& c) {
		c.function_output("", [](AstCompare& c) {
			c.generic_name_expr("List", [](AstCompare& c) {
				c.generic_name_expr("List", [](AstCompare& c) {
					c.generic_name_expr("List", [](AstCompare& c) {
						c.name_expr("int32");
					});
				});
			});
		});
		c.return_expr([](AstCompare& c) {
			c.group_expr([](AstCompare&) {});
		});
	});
	c.function_definition(cero::AccessSpecifier::None, "f", [](AstCompare& c) {
		c.function_output("", [](AstCompare& c) {
			c.generic_name_expr("List", [](AstCompare& c) {
				c.generic_name_expr("List", [](AstCompare& c) {
					c.generic_name_expr("List", [](AstCompare& c) {
						c.name_expr("int32");
					});
				});
			});
		});
		c.return_expr([](AstCompare& c) {
			c.group_expr([](AstCompare&) {});
		});
	});

	c.compare();
}

CERO_TEST(ParseLessAndRightShift) {
	auto source = make_test_source(R"_____(
oof(int32 a, int32 b) -> bool {
	return a < b >> (16 - 4);
}

oog(int32 a, int32 b, int32 c) -> bool {
	return a < b >> c;
}
)_____");

	ExhaustiveReporter r;
	auto ast = cero::run_parser_on_source(source, r, TabSize);
	CHECK(!ast.has_errors());

	AstCompare c(ast);
	c.root();
	c.function_definition(cero::AccessSpecifier::None, "oof", [](AstCompare& c) {
		c.function_parameter(cero::ParameterSpecifier::None, "a", [](AstCompare& c) {
			c.name_expr("int32");
		});
		c.function_parameter(cero::ParameterSpecifier::None, "b", [](AstCompare& c) {
			c.name_expr("int32");
		});
		c.function_output("", [](AstCompare& c) {
			c.name_expr("bool");
		});
		c.return_expr([](AstCompare& c) {
			c.binary_expr(cero::BinaryOperator::Less, [](AstCompare& c) {
				c.name_expr("a");
				c.binary_expr(cero::BinaryOperator::Shr, [](AstCompare& c) {
					c.name_expr("b");
					c.group_expr([](AstCompare& c) {
						c.binary_expr(cero::BinaryOperator::Sub, [](AstCompare& c) {
							c.numeric_literal_expr(cero::NumericLiteralKind::Decimal);
							c.numeric_literal_expr(cero::NumericLiteralKind::Decimal);
						});
					});
				});
			});
		});
	});
	c.function_definition(cero::AccessSpecifier::None, "oog", [](AstCompare& c) {
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
		c.return_expr([](AstCompare& c) {
			c.binary_expr(cero::BinaryOperator::Less, [](AstCompare& c) {
				c.name_expr("a");
				c.binary_expr(cero::BinaryOperator::Shr, [](AstCompare& c) {
					c.name_expr("b");
					c.name_expr("c");
				});
			});
		});
	});

	c.compare();
}

CERO_TEST(ParseAmbiguousGenericCallVsComparisonArguments) {
	auto source = make_test_source(R"_____(
ouch(float32 e) -> float64 {
	return a(b<c, d>(e));
}
)_____");

	ExhaustiveReporter r;
	auto ast = cero::run_parser_on_source(source, r, TabSize);
	CHECK(!ast.has_errors());

	AstCompare c(ast);
	c.root();
	c.function_definition(cero::AccessSpecifier::None, "ouch", [](AstCompare& c) {
		c.function_parameter(cero::ParameterSpecifier::None, "e", [](AstCompare& c) {
			c.name_expr("float32");
		});
		c.function_output("", [](AstCompare& c) {
			c.name_expr("float64");
		});
		c.return_expr([](AstCompare& c) {
			c.call_expr([](AstCompare& c) {
				c.name_expr("a");
				c.call_expr([](AstCompare& c) {
					c.generic_name_expr("b", [](AstCompare& c) {
						c.name_expr("c");
						c.name_expr("d");
					});
					c.name_expr("e");
				});
			});
		});
	});

	c.compare();
}

CERO_TEST(ParseComparisonArgumentsVsGenericPattern) {
	auto source = make_test_source(R"_____(
ouch(float32 e) -> float64 {
	return a(b < c, d > e);
}
)_____");

	ExhaustiveReporter r;
	auto ast = cero::run_parser_on_source(source, r, TabSize);
	CHECK(!ast.has_errors());

	AstCompare c(ast);
	c.root();
	c.function_definition(cero::AccessSpecifier::None, "ouch", [](AstCompare& c) {
		c.function_parameter(cero::ParameterSpecifier::None, "e", [](AstCompare& c) {
			c.name_expr("float32");
		});
		c.function_output("", [](AstCompare& c) {
			c.name_expr("float64");
		});
		c.return_expr([](AstCompare& c) {
			c.call_expr([](AstCompare& c) {
				c.name_expr("a");
				c.binary_expr(cero::BinaryOperator::Less, [](AstCompare& c) {
					c.name_expr("b");
					c.name_expr("c");
				});
				c.binary_expr(cero::BinaryOperator::Greater, [](AstCompare& c) {
					c.name_expr("d");
					c.name_expr("e");
				});
			});
		});
	});

	c.compare();
}

CERO_TEST(ParseComparisonAndRightShiftAsGenericArgument) {
	auto source = make_test_source(R"_____(
woof() -> A<(B > C)> {
	return ();
}

meow() -> A<(D >> E)> {
	return ();
}
)_____");

	ExhaustiveReporter r;
	auto ast = cero::run_parser_on_source(source, r, TabSize);
	CHECK(!ast.has_errors());

	AstCompare c(ast);
	c.root();
	c.function_definition(cero::AccessSpecifier::None, "woof", [](AstCompare& c) {
		c.function_output("", [](AstCompare& c) {
			c.generic_name_expr("A", [](AstCompare& c) {
				c.group_expr([](AstCompare& c) {
					c.binary_expr(cero::BinaryOperator::Greater, [](AstCompare& c) {
						c.name_expr("B");
						c.name_expr("C");
					});
				});
			});
		});
		c.return_expr([](AstCompare& c) {
			c.group_expr([](AstCompare&) {});
		});
	});
	c.function_definition(cero::AccessSpecifier::None, "meow", [](AstCompare& c) {
		c.function_output("", [](AstCompare& c) {
			c.generic_name_expr("A", [](AstCompare& c) {
				c.group_expr([](AstCompare& c) {
					c.binary_expr(cero::BinaryOperator::Shr, [](AstCompare& c) {
						c.name_expr("D");
						c.name_expr("E");
					});
				});
			});
		});
		c.return_expr([](AstCompare& c) {
			c.group_expr([](AstCompare&) {});
		});
	});

	c.compare();
}

CERO_TEST(ParseGenericParameters) {
	auto source = make_test_source(R"_____(
moo(List<int32> _a,
	List<List<int32>> _b,
	List<List<int32> > _c,
	List<List<List<int32> > > _d,
	List<List<List<int32> >> _e,
	List<List<List<int32>> > _f,
	List<List<List<int32>>> _g) {
}
)_____");

	ExhaustiveReporter r;
	auto ast = cero::run_parser_on_source(source, r, TabSize);
	CHECK(!ast.has_errors());

	AstCompare c(ast);
	c.root();
	c.function_definition(cero::AccessSpecifier::None, "moo", [](AstCompare& c) {
		c.function_parameter(cero::ParameterSpecifier::None, "_a", [](AstCompare& c) {
			c.generic_name_expr("List", [](AstCompare& c) {
				c.name_expr("int32");
			});
		});
		c.function_parameter(cero::ParameterSpecifier::None, "_b", [](AstCompare& c) {
			c.generic_name_expr("List", [](AstCompare& c) {
				c.generic_name_expr("List", [](AstCompare& c) {
					c.name_expr("int32");
				});
			});
		});
		c.function_parameter(cero::ParameterSpecifier::None, "_c", [](AstCompare& c) {
			c.generic_name_expr("List", [](AstCompare& c) {
				c.generic_name_expr("List", [](AstCompare& c) {
					c.name_expr("int32");
				});
			});
		});
		c.function_parameter(cero::ParameterSpecifier::None, "_d", [](AstCompare& c) {
			c.generic_name_expr("List", [](AstCompare& c) {
				c.generic_name_expr("List", [](AstCompare& c) {
					c.generic_name_expr("List", [](AstCompare& c) {
						c.name_expr("int32");
					});
				});
			});
		});
		c.function_parameter(cero::ParameterSpecifier::None, "_e", [](AstCompare& c) {
			c.generic_name_expr("List", [](AstCompare& c) {
				c.generic_name_expr("List", [](AstCompare& c) {
					c.generic_name_expr("List", [](AstCompare& c) {
						c.name_expr("int32");
					});
				});
			});
		});
		c.function_parameter(cero::ParameterSpecifier::None, "_f", [](AstCompare& c) {
			c.generic_name_expr("List", [](AstCompare& c) {
				c.generic_name_expr("List", [](AstCompare& c) {
					c.generic_name_expr("List", [](AstCompare& c) {
						c.name_expr("int32");
					});
				});
			});
		});
		c.function_parameter(cero::ParameterSpecifier::None, "_g", [](AstCompare& c) {
			c.generic_name_expr("List", [](AstCompare& c) {
				c.generic_name_expr("List", [](AstCompare& c) {
					c.generic_name_expr("List", [](AstCompare& c) {
						c.name_expr("int32");
					});
				});
			});
		});
	});

	c.compare();
}

CERO_TEST(ParseVariableWithGenericType) {
	auto source = make_test_source(R"_____(
bark() {
	List<int32> _a = ();
	List<List<int32>> _b = ();
	List<List<int32> > _c = ();
	List<List<List<int32> > > _d = ();
	List<List<List<int32> >> _e = ();
	List<List<List<int32>> > _f = ();
	List<List<List<int32>>> _g = ();
}
)_____");

	ExhaustiveReporter r;
	auto ast = cero::run_parser_on_source(source, r, TabSize);
	CHECK(!ast.has_errors());

	AstCompare c(ast);
	c.root();
	c.function_definition(cero::AccessSpecifier::None, "bark", [](AstCompare& c) {
		c.binding_statement(cero::BindingSpecifier::Let, "_a", [](AstCompare& c) {
			c.generic_name_expr("List", [](AstCompare& c) {
				c.name_expr("int32");
			});
			c.group_expr([](AstCompare&) {});
		});
		c.binding_statement(cero::BindingSpecifier::Let, "_b", [](AstCompare& c) {
			c.generic_name_expr("List", [](AstCompare& c) {
				c.generic_name_expr("List", [](AstCompare& c) {
					c.name_expr("int32");
				});
			});
			c.group_expr([](AstCompare&) {});
		});
		c.binding_statement(cero::BindingSpecifier::Let, "_c", [](AstCompare& c) {
			c.generic_name_expr("List", [](AstCompare& c) {
				c.generic_name_expr("List", [](AstCompare& c) {
					c.name_expr("int32");
				});
			});
			c.group_expr([](AstCompare&) {});
		});
		c.binding_statement(cero::BindingSpecifier::Let, "_d", [](AstCompare& c) {
			c.generic_name_expr("List", [](AstCompare& c) {
				c.generic_name_expr("List", [](AstCompare& c) {
					c.generic_name_expr("List", [](AstCompare& c) {
						c.name_expr("int32");
					});
				});
			});
			c.group_expr([](AstCompare&) {});
		});
		c.binding_statement(cero::BindingSpecifier::Let, "_e", [](AstCompare& c) {
			c.generic_name_expr("List", [](AstCompare& c) {
				c.generic_name_expr("List", [](AstCompare& c) {
					c.generic_name_expr("List", [](AstCompare& c) {
						c.name_expr("int32");
					});
				});
			});
			c.group_expr([](AstCompare&) {});
		});
		c.binding_statement(cero::BindingSpecifier::Let, "_f", [](AstCompare& c) {
			c.generic_name_expr("List", [](AstCompare& c) {
				c.generic_name_expr("List", [](AstCompare& c) {
					c.generic_name_expr("List", [](AstCompare& c) {
						c.name_expr("int32");
					});
				});
			});
			c.group_expr([](AstCompare&) {});
		});
		c.binding_statement(cero::BindingSpecifier::Let, "_g", [](AstCompare& c) {
			c.generic_name_expr("List", [](AstCompare& c) {
				c.generic_name_expr("List", [](AstCompare& c) {
					c.generic_name_expr("List", [](AstCompare& c) {
						c.name_expr("int32");
					});
				});
			});
			c.group_expr([](AstCompare&) {});
		});
	});

	c.compare();
}

} // namespace tests
