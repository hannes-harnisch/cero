#include "semantic_analyzer.hh"

#include "cero/syntax/ast_visitor.hh"

namespace cero {

struct SemanticAnalyzer final : AstVisitor<void> {
	IrGraph graph_;

	void run() {
	}
};

IrGraph run_semantic_analyzer(const Ast& ast, const SourceView& source, Reporter& reporter, uint8_t tab_size) {
	SemanticAnalyzer analyzer;
	analyzer.run();
	return std::move(analyzer.graph_);
}

} // namespace cero
