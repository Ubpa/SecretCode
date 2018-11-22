#ifndef _BASE_H_
#define _BASE_H_

#include <vector>
#include <string>

namespace CppUtility {
	namespace Other {
		namespace Base64 {
			typedef unsigned char byte;
			std::string Encode(const std::vector<byte> & code);
			const std::vector<unsigned char> Decode(const std::string & code);
		}
	}
}

#endif // !_BASE_H_
