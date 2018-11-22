#ifndef _DES_H_
#define _DES_H_

#include <bitset>
#include <vector>
#include <string>

namespace CppUtility {
	namespace Other {
		class DES {
		public:
			DES(const char key[8]);
			const std::bitset<64> Encrypt(const char plainText[8]) const;
			const std::string Decrypt(const std::bitset<64> & cipher) const;
		private:
			//std::bitset<64> key;
			std::vector<std::bitset<48>> subKeys;
		};
	}
}

#endif // !_DES_H_