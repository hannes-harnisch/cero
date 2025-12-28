#pragma once

namespace util {

/// Allocator that linearly allocates within a large memory block allocated dynamically upfront. Allocates a new block, larger
/// via geometric size progression, whenever the current block becomes full and joins them together in a linked list. Memory can
/// only be deallocated all at once, either explicitly or when the arena is destroyed.
class MemoryArena {
public:
	/// Creates an empty memory arena.
	MemoryArena();

	/// Creates an empty memory arena where the first will have the given initial size when allocate is first called.
	explicit MemoryArena(size_t initial_size);

	/// Allocates size bytes with the given alignment. May return the same pointer if given size 0. Alignment must be a power of
	/// two, otherwise the behavior is undefined.
	void* allocate(size_t size, size_t alignment);

	/// Deallocates all memory blocks. Non-trivial objects must be destroyed manually.
	void release();

	/// Returns the	total size of all memory blocks in the arena, not counting overhead from allocator bookkeeping.
	size_t get_total_size() const;

	/// Creates an object in the arena and returns a pointer to it. The object must be destroyed manually if it is not trivial.
	template<typename T, typename... Args>
	T* create(Args&&... args) {
		void* storage = allocate(sizeof(T), alignof(T));
		return new (storage) T(std::forward<Args>(args)...);
	}

	~MemoryArena();
	MemoryArena(MemoryArena&& other) noexcept;
	MemoryArena& operator=(MemoryArena&& other) noexcept;

private:
	static constexpr size_t initial_block_size = 1024;

	struct Block;

	void* next_ptr_ = nullptr;
	size_t remaining_size_ = 0;
	size_t next_block_size_ = initial_block_size;
	Block* head_block_ = nullptr;
	size_t total_size_ = 0;

	void create_block(size_t size, size_t alignment);
};

/// A lightweight dynamic array that will allocate trivial objects contiguously within a memory arena.
template<typename T>
requires std::is_trivially_copyable_v<T>
class ArenaArray {
public:
	/// Appends an element to the array. If the array is full, it grows geometrically.
	void put(const T& value, MemoryArena& arena) {
		if (count_ == capacity_) {
			size_t new_cap = std::max((size_t) capacity_ + capacity_ / 2, initial_capacity);
			void* storage = arena.allocate(new_cap * sizeof(T), alignof(T));

			::memcpy(storage, data_, count_ * sizeof(T));
			data_ = static_cast<T*>(storage);
			capacity_ = (uint32_t) new_cap;
		}

		new (data_ + count_) T(value);
		++count_;
	}

	const T* begin() const {
		return data_;
	}

	const T* end() const {
		return data_ + count_;
	}

private:
	static constexpr size_t initial_capacity = 4;

	T* data_ = nullptr;
	uint32_t count_ = 0;
	uint32_t capacity_ = 0;
};

} // namespace util
