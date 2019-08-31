#pragma once

namespace spring {

	// For warning C4316
	template <size_t Sz>
	class IAlignedAllocationPolicy {

	public:
		static void* operator new(size_t size) {
			return _aligned_malloc(size, Sz);
		}

		static void operator delete(void* memory) {
			_aligned_free(memory);
		}

	};

}