#ifndef _SECRECT_CODE_H_
#define _SECRECT_CODE_H_

#include <Other/Base64.h>
#include <Other/DES.h>

namespace CppUtility {
	namespace Other {
		class SecretCode {
		public:
			SecretCode(const char key[8]);

			const std::string Encrypt(const std::string & plainText);
			const std::string Decode(const std::string & cipherText);
		private:
			DES des;
		};
	}
}

#endif // !_SECRECT_CODE_H_
