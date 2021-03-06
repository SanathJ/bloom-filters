// Microsoft Visual Studio

#if defined(_MSC_VER) && (_MSC_VER < 1600)

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef unsigned __int64 uint64_t;

// Other compilers

#else	// defined(_MSC_VER)

#include <stdint.h>

#endif // !defined(_MSC_VER)

#if defined(_MSC_VER)

#define FORCE_INLINE	__forceinline

#include <stdlib.h>

#define ROTL32(x,y)	_rotl(x,y)
#define ROTL64(x,y)	_rotl64(x,y)

#define BIG_CONSTANT(x) (x)

// Other compilers

#else	// defined(_MSC_VER)

#define	FORCE_INLINE inline __attribute__((always_inline))

inline uint32_t rotl32 ( uint32_t x, int8_t r )
{
  return (x << r) | (x >> (32 - r));
}

inline uint64_t rotl64 ( uint64_t x, int8_t r )
{
  return (x << r) | (x >> (64 - r));
}

#define	ROTL32(x,y)	rotl32(x,y)
#define ROTL64(x,y)	rotl64(x,y)

#define BIG_CONSTANT(x) (x##LLU)

#endif // !defined(_MSC_VER)


namespace sanath 
{
    class hasher
    {
    public:
        uint32_t seed = 89478583;

        // MurmurHash3 was written by Austin Appleby, and is placed in the public
        // domain. The author hereby disclaims copyright to this source code.
        uint32_t MurmurHash3 (void * key, int len)
        {
            const uint8_t *data = (const uint8_t *)key;
            const int nblocks = len / 4;

            uint32_t h1 = seed;

            const uint32_t c1 = 0xcc9e2d51;
            const uint32_t c2 = 0x1b873593;

            // body

            const uint32_t *blocks = (const uint32_t *)(data + nblocks * 4);

            for(int i = -nblocks; i; i++)
            {
                uint32_t k1 = getblock32(blocks, i);

                k1 *= c1;
                k1 = ROTL32(k1, 15);
                k1 *= c2;
                
                h1 ^= k1;
                h1 = ROTL32(h1, 13); 
                h1 = h1*5+0xe6546b64;
            }

            // tail

            const uint8_t *tail = (const uint8_t *)(data + nblocks * 4);

            uint32_t k1 = 0;

            switch(len & 3)
            {
            case 3: k1 ^= tail[2] << 16;
            case 2: k1 ^= tail[1] << 8;
            case 1: k1 ^= tail[0];
                    k1 *= c1; k1 = ROTL32(k1, 15); k1 *= c2; h1 ^= k1;
            };

            // finalization

            h1 ^= len;

            h1 = fmix32(h1);

            return h1;
        } 

        // djb2
        uint32_t djb2(void *key, int len) 
        {
            uint32_t hash = seed;

            char *ptr = reinterpret_cast<char *>(key);
            for(size_t i = 0; i < len; i++) 
            {
                hash = ((hash << 5) + hash) + ptr[i];
            }

            return hash;
        }

        // FNV-1a
        uint32_t fnv1a(void *key, int len) 
        {
            const uint32_t fnv_offset_basis = 2166136261, fnv_prime = 16777619; 

            uint32_t hash = fnv_offset_basis;

            char *ptr = reinterpret_cast<char *>(key);
            for(size_t i = 0; i < len; i++) 
            {
                hash ^= ptr[i];
                hash *= fnv_prime;
            }

            return hash;
        }

    private:
        // Block read - if your platform needs to do endian-swapping or can only
        // handle aligned reads, do the conversion here

        FORCE_INLINE uint32_t getblock32 ( const uint32_t * p, int i )
        {
            return p[i];
        }

        // Finalization mix - force all bits of a hash block to avalanche

        FORCE_INLINE uint32_t fmix32 ( uint32_t h )
        {
            h ^= h >> 16;
            h *= 0x85ebca6b;
            h ^= h >> 13;
            h *= 0xc2b2ae35;
            h ^= h >> 16;

            return h;
        }

    };
}