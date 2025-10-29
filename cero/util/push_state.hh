#pragma once

namespace util {

/// Takes an object and assigns it a new value after remembering its old value. The object's old value is restored when this
/// goes out of scope.
template<typename T>
class PushState {
public:
	template<typename U>
	PushState(T& object, U&& new_value) :
	    object_(object),
	    old_value_(std::exchange(object, std::forward<U>(new_value))) {
	}

	~PushState() {
		object_ = std::move(old_value_);
	}

	PushState(PushState&&) = delete;

private:
	T& object_;
	T old_value_;
};

} // namespace util
