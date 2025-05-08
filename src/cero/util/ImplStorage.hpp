#pragma once

namespace cero {

/**
 * Helper for hiding implementation details without dynamic allocation.
 */
template<typename T, size_t Size = 16, size_t Align = alignof(void*)>
class ImplStorage {
public:
	ImplStorage() { // NOLINT(*-pro-type-member-init)
		static_assert_size_align();
		new (storage_) T();
	}

	explicit(false) ImplStorage(T&& t) { // NOLINT(*-pro-type-member-init)
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
	alignas(Align) unsigned char storage_[Size];

	static void static_assert_size_align() {
		static_assert(Size >= sizeof(T) && Align >= alignof(T), "Specified size and alignment must be greater or equal to the "
																"size and alignment of T.");
	}
};

} // namespace cero
