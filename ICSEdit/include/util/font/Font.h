#pragma once
#include "util/file/BinaryFile.h"

namespace ICSE {
namespace font {

	class Font {		
	 public:
		Font(void);

		static Font LoadFromFile(ICSE::file::BinaryFile &file);

		Font(Font &&font);
		Font &operator=(Font &&font);
		
		float descent(void);
		float ascent(void);
	 private:
		
	};

}
}
