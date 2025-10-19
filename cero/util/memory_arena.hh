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

	void create_block(size_t size, size_t alignment);
};

/// A lightweight dynamic array that will allocate trivial objects contiguously within a memory arena.
template<typename T>
requires std::is_trivially_copyable_v<T>
class ArenaArray {
public:
	/// Appends an element to the array. If the array is full, it grows geometrically.
	void put(const T& value, MemoryArena& arena) {
		const uint32_t count = alloc_->count;
		const size_t capacity = alloc_->capacity;

		if (count == capacity) {
			size_t new_cap = std::max(capacity + capacity / 2, initial_capacity);
			void* storage = arena.allocate(sizeof(Allocation) + new_cap * sizeof(T), alignof(Allocation));

			Allocation* new_alloc = new (storage) Allocation;
			new_alloc->count = count;
			new_alloc->capacity = (uint32_t) new_cap;

			::memcpy(new_alloc->array, alloc_->array, count * sizeof(T));
			alloc_ = new_alloc;
		}

		new (alloc_->array + count) T(value);
		++alloc_->count;
	}

	const T* begin() const {
		return alloc_->array;
	}

	const T* end() const {
		return alloc_->array + alloc_->count;
	}

private:
	struct Allocation {
		uint32_t count = 0;
		uint32_t capacity = 0;
		T array[];
	};

	static constexpr size_t initial_capacity = 4;
	static inline const Allocation empty = {};

	Allocation* alloc_ = const_cast<Allocation*>(&empty);
};

} // namespace util
