#pragma once

#include "cero/report/reporter.hh"
#include "cero/sema/tir_tree.hh"
#include "cero/syntax/ast.hh"

namespace cero {

/// Performs type checking on the given AST.
TirTree run_type_checker(const Ast& ast, const SourceView& source, Reporter& reporter, uint8_t tab_size = default_tab_size);

} // namespace cero
