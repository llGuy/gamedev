#ifndef _UDATA_H_
#define _UDATA_H_

namespace mulgame {

	enum udata_t
	{
		VEC3,

		VEC2,

		MAT4,
		
		F1,

		INV
	};

	struct UDataLoc
	{
		UDataLoc(void) = default;
		UDataLoc(udata_t t, const char* n)
			: type(t), name(n)
		{
		}
		udata_t type;
		const char* name;
		uint32_t location;
	};

}

#endif /* _UDATA_H_ */
