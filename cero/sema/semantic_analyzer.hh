#pragma once

#include "cero/report/reporter.hh"
#include "cero/sema/ir_graph.hh"
#include "cero/syntax/ast.hh"

namespace cero {

IrGraph run_semantic_analyzer(const Ast& ast, const SourceView& source, Reporter& reporter, uint8_t tab_size = default_tab_size);

} // namespace cero
