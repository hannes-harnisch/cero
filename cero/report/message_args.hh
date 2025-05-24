#pragma once

#include "cero/report/message.hh"

namespace cero {

/// Stores formatting arguments for diagnostic messages.
struct MessageArgs {
	fmt::dynamic_format_arg_store<fmt::format_context> store;

	explicit MessageArgs(auto&&... args) {
		(store.push_back(args), ...);
	}

	/// Checks if the number of arguments matches the number expected for the given message, since fmt::vformat will not error
	/// if too many arguments are supplied.
	bool valid_for_message(Message message) const;
};

} // namespace cero
