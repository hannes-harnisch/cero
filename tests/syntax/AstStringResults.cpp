#include "common/ExhaustiveReporter.hpp"
#include "common/Test.hpp"

#include <cero/syntax/Parser.hpp>

namespace tests {

CERO_TEST(AstStringForEmptyFunction) {
	auto source = make_test_source(R"_____(

main() {
}

)_____");

	ExhaustiveReporter r;
	auto ast = cero::run_parser_on_source(source, r, TabSize);
	CHECK(!ast.has_errors());

	auto str = ast.to_string(source, TabSize);
	auto expected = R"_____(AST for AstStringForEmptyFunction (2 nodes)
└── function `main` [3:1]
    ├── parameters
    ├── outputs
    └── statements
)_____";
	CHECK_EQ(str, expected);
}

CERO_TEST(AstStringForSimpleFunctionWithParametersAndReturn) {
	auto source = make_test_source(R"_____(

a(int32 x, bool _a, bool _b = x) -> float32 {
}

)_____");

	ExhaustiveReporter r;
	auto ast = cero::run_parser_on_source(source, r, TabSize);
	CHECK(!ast.has_errors());

	auto str = ast.to_string(source, TabSize);
	auto expected = R"_____(AST for AstStringForSimpleFunctionWithParametersAndReturn (11 nodes)
└── function `a` [3:1]
    ├── parameters
    │   ├── value parameter `x` [3:3]
    │   │   └── name `int32` [3:3]
    │   ├── value parameter `_a` [3:12]
    │   │   └── name `bool` [3:12]
    │   └── value parameter `_b` [3:21]
    │       ├── name `bool` [3:21]
    │       └── name `x` [3:31]
    ├── outputs
    │   └── output [3:37]
    │       └── name `float32` [3:37]
    └── statements
)_____";
	CHECK_EQ(str, expected);
}

CERO_TEST(AstStringForCall) {
	auto source = make_test_source(R"_____(

a(int32 _a, float64 _f, int64 _b) {
}

b(int32 i, float64 f) {
	a(i, f, i * i);
}

)_____");

	ExhaustiveReporter r;
	auto ast = cero::run_parser_on_source(source, r, TabSize);
	CHECK(!ast.has_errors());

	auto str = ast.to_string(source, TabSize);
	auto expected = R"_____(AST for AstStringForCall (20 nodes)
├── function `a` [3:1]
│   ├── parameters
│   │   ├── value parameter `_a` [3:3]
│   │   │   └── name `int32` [3:3]
│   │   ├── value parameter `_f` [3:13]
│   │   │   └── name `float64` [3:13]
│   │   └── value parameter `_b` [3:25]
│   │       └── name `int64` [3:25]
│   ├── outputs
│   └── statements
└── function `b` [6:1]
    ├── parameters
    │   ├── value parameter `i` [6:3]
    │   │   └── name `int32` [6:3]
    │   └── value parameter `f` [6:12]
    │       └── name `float64` [6:12]
    ├── outputs
    └── statements
        └── call expression [7:5]
            ├── name `a` [7:5]
            └── arguments
                ├── name `i` [7:7]
                ├── name `f` [7:10]
                └── `*` [7:13]
                    ├── name `i` [7:13]
                    └── name `i` [7:17]
)_____";
	CHECK_EQ(str, expected);
}

CERO_TEST(AstStringForFibonacci) {
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

	auto str = ast.to_string(source, TabSize);
	auto expected = R"_____(AST for AstStringForFibonacci (27 nodes)
└── function `fibonacci` [2:1]
    ├── parameters
    │   └── var parameter `n` [2:11]
    │       └── name `uint32` [2:15]
    ├── outputs
    │   └── output [2:28]
    │       └── name `uint32` [2:28]
    └── statements
        ├── var binding `result` [3:5]
        │   ├── type
        │   │   └── name `uint32` [3:9]
        │   └── initializer
        │       └── decimal literal ` ---TODO--- ` [3:25]
        ├── var binding `next` [4:5]
        │   ├── type
        │   │   └── name `uint32` [4:9]
        │   └── initializer
        │       └── decimal literal ` ---TODO--- ` [4:25]
        ├── while [6:5]
        │   ├── `!=` [6:11]
        │   │   ├── `postfix --` [6:11]
        │   │   │   └── name `n` [6:11]
        │   │   └── decimal literal ` ---TODO--- ` [6:18]
        │   └── statements
        │       ├── let binding `temp` [7:9]
        │       │   └── initializer
        │       │       └── name `next` [7:20]
        │       ├── `=` [8:9]
        │       │   ├── name `next` [8:9]
        │       │   └── name `result` [8:16]
        │       └── `+=` [9:9]
        │           ├── name `result` [9:9]
        │           └── name `temp` [9:19]
        └── return [12:5]
            └── name `result` [12:12]
)_____";
	CHECK_EQ(str, expected);
}

} // namespace tests
