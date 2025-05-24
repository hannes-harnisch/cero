#include "CodeLocation.hpp"

namespace cero {

std::string CodeLocation::to_string() const {
	return fmt::format("{}:{}:{}", source_path, line, column);
}

std::string CodeLocation::to_short_string() const {
	return fmt::format("[{}:{}]", line, column);
}

} // namespace cero
