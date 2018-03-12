#ifndef ZWUtils_BoundedAllocator_H_
#define ZWUtils_BoundedAllocator_H_

class BoundedOneshotAllocator {
	protected:
		size_t _allocated;
	public:
		size_t const Total;

		BoundedOneshotAllocator(size_t total)
			: Total(total), _allocated(0)
			{}

		BoundedOneshotAllocator(BoundedOneshotAllocator &&alloc)
			: Total(alloc.Total), _allocated(alloc._allocated)
			{ *const_cast<size_t*>(&alloc.Total) = alloc._allocated = 0; }

		size_t available() { return Total - _allocated; }

		void* allocate(size_t size) {
			void* Ret = nullptr;
			if (_allocated+size <= Total) {
				if (Ret = malloc(size)) {
					_allocated+= size;
				}
			}
			return Ret;
		}

		void deallocate(void* pointer) {
			free(pointer);
		}
};

extern "C" {
extern size_t msize(void *ptr);
}

class BoundedRecycleAllocator {
	protected:
		size_t _allocated;
	public:
		size_t const Total;

		BoundedRecycleAllocator(size_t total)
			: Total(total), _allocated(0)
			{}

		BoundedRecycleAllocator(BoundedRecycleAllocator &&alloc)
			: Total(alloc.Total), _allocated(alloc._allocated)
			{ *const_cast<size_t*>(&alloc.Total) = alloc._allocated = 0; }

		size_t available() { return Total - _allocated; }

		void* allocate(size_t size) {
			void* Ret = nullptr;
			if (_allocated+size <= Total) {
				if (Ret = malloc(size)) {
					_allocated+= msize(Ret);
				}
			}
			return Ret;
		}

		void deallocate(void* pointer) {
			_allocated -= msize(pointer);
			free(pointer);
		}
};

#endif //ZWUtils_BoundedAllocator_H_