#ifndef TERRAIN_HEADER
#define TERRAIN_HEADER

namespace dawn { namespace terrain { 

		class Terrain
		{
		public:
			// no copying
			Terrain(const Terrain&) = delete;
			const Terrain& operator=(const Terrain&) = delete;

			Terrain(void) noexcept;
			~Terrain(void) = default;
		};

	} 
}

#endif