#pragma once

namespace util {

/// Utility for hiding implementation details without dynamic allocation, by providing correctly aligned storage with a given
/// size. The size and alignment requirements of the implementation type are checked statically when it is actually constructed.
template<typename T, size_t size, size_t align = alignof(void*)>
class ImplStorage {
public:
	ImplStorage() { // NOLINT(*-pro-type-member-init)
		static_assert_size_align();
		new (storage_) T();
	}

	explicit ImplStorage(T&& t) { // NOLINT(*-pro-type-member-init)
		static_assert_size_align();
		new (storage_) T(std::move(t));
	}

	ImplStorage(ImplStorage&& other) noexcept { // NOLINT(*-pro-type-member-init)
		static_assert_size_align();
		new (storage_) T(std::move(*other));
	}

	~ImplStorage() {
		(*this)->~T();
	}

	ImplStorage& operator=(ImplStorage&& other) noexcept {
		**this = std::move(*other);
		return *this;
	}

	T& operator*() & noexcept {
		return *std::launder(reinterpret_cast<T*>(storage_));
	}

	const T& operator*() const& noexcept {
		return *std::launder(reinterpret_cast<const T*>(storage_));
	}

	T&& operator*() && noexcept {
		return std::move(*std::launder(reinterpret_cast<T*>(storage_)));
	}

	T* operator->() noexcept {
		return std::launder(reinterpret_cast<T*>(storage_));
	}

	const T* operator->() const noexcept {
		return std::launder(reinterpret_cast<const T*>(storage_));
	}

private:
	alignas(align) unsigned char storage_[size];

	static void static_assert_size_align() {
		static_assert(size >= sizeof(T) && align >= alignof(T),
		    "Specified size and alignment must be greater or equal to the "
		    "size and alignment of T.");
	}
};

} // namespace util
