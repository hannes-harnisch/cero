#include "memory_arena.hh"

namespace util {

struct MemoryArena::Block {
	Block* next;
};

MemoryArena::MemoryArena() = default;

MemoryArena::MemoryArena(size_t initial_size) :
    next_block_size_(initial_size) {
}

void* MemoryArena::allocate(size_t size, size_t alignment) {
	void* ptr = std::align(alignment, size, next_ptr_, remaining_size_);

	if (ptr == nullptr) [[unlikely]] {
		create_block(size, alignment);
		ptr = next_ptr_;
	}

	next_ptr_ = static_cast<char*>(next_ptr_) + size;
	remaining_size_ -= size;
	return ptr;
}

void MemoryArena::create_block(size_t size, size_t alignment) {
	size_t block_size = std::max(next_block_size_, size + sizeof(Block));
	void* block = ::operator new(block_size);

	Block* new_head = new (block) Block {.next = head_block_};
	head_block_ = new_head;

	block = static_cast<char*>(block) + sizeof(Block);
	block_size -= sizeof(Block);

	next_ptr_ = std::align(alignment, size, block, block_size);
	remaining_size_ = block_size;
	next_block_size_ += next_block_size_ / 2;
}

void MemoryArena::release() {
	Block* block = head_block_;
	head_block_ = nullptr;

	while (block != nullptr) {
		Block* next = block->next;
		::operator delete(block);
		block = next;
	}
}

MemoryArena::~MemoryArena() {
	release();
}

MemoryArena::MemoryArena(MemoryArena&& other) noexcept :
    next_ptr_(std::exchange(other.next_ptr_, nullptr)),
    remaining_size_(std::exchange(other.remaining_size_, 0)),
    next_block_size_(std::exchange(other.next_block_size_, initial_block_size)),
    head_block_(std::exchange(other.head_block_, nullptr)) {
}

MemoryArena& MemoryArena::operator=(MemoryArena&& other) noexcept {
	if (this != &other) {
		release();

		next_ptr_ = std::exchange(other.next_ptr_, nullptr);
		remaining_size_ = std::exchange(other.remaining_size_, 0);
		next_block_size_ = std::exchange(other.next_block_size_, initial_block_size);
		head_block_ = std::exchange(other.head_block_, nullptr);
	}
	return *this;
}

} // namespace util
