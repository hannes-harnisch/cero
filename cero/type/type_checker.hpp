#pragma once

#include "cero/report/reporter.hpp"
#include "cero/syntax/ast.hpp"
#include "cero/type/symbol_tree.hpp"

namespace cero {

/// Performs type checking on the given AST.
SymbolTree run_type_checker(const Ast& ast, const SourceView& source, Reporter& reporter, uint8_t tab_size = default_tab_size);

} // namespace cero
