#pragma once

namespace cero {

enum class TypeId : uint64_t;

class Type {};

class TypeContext {
public:

private:
	std::unordered_map<TypeId, Type> type_map_;
};

} // namespace cero
