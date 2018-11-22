#ifndef _SECRECT_CODE_H_
#define _SECRECT_CODE_H_

#include <Other/Base64.h>
#include <Other/DES.h>

namespace CppUtility {
	namespace Other {
		class SecretCode {
		public:
			SecretCode(const char key[8]);

			const std::string Encrypt(const std::string & plainText) const;
			const std::string Decrypt(const std::string & cipherText) const;

			enum TextState {
				NOT_CIPHER,//�����Ǹ�ʽ���������
				CIPHER_KEY_RIGHT,
				CIPHER_KEY_WRONG
			};
			TextState Detect(const std::string & text) const;
		private:
			//������Ҫ�޸�linePlainText�Ŀ��������Բ�ʹ�� const &
			static const std::string EncryptLine(const DES & des, std::string linePlainText);
			static const std::string DecryptLine(const DES & des, const std::string & lineCipherText);

			DES des;
			static const DES defaultHeaderDES;
		};
	}
}

#endif // !_SECRECT_CODE_H_
