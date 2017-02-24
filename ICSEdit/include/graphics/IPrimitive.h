#pragma once
#include "graphics/DrawEnv.h"

namespace ICSE{
namespace graphics{

	class IPrimitive {
	public:
		virtual void draw(DrawEnv *env) = 0;
		virtual ~IPrimitive(void){};
		IPrimitive(void) {};
	protected:
	};

}
}
