#include <cstring>

extern "C" void *_memcpy(void *dest, const void *src, size_t len);

namespace std
{
	void *memcpy(void *dest, const void *src, size_t len) __attribute__((weak, alias("_memcpy")));
}

void *_memcpy(void *dest, const void *src, size_t len)
{
	auto *_dest{static_cast<char *>(dest)};
	const auto *const _src{static_cast<const char *>(src)};
	for (size_t i{0}; i < len; ++i)
		_dest[i] = _src[i];
	return dest;
}
