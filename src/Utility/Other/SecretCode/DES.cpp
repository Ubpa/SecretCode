#include <Other/DES.h>
#include "DesImpl.hpp"

using namespace CppUtility::Other;
using namespace std;

DES::DES(const char key[8])
	: //key(charToBitset(key)),
	//subKeys(generateKeys(this->key)) { }
	subKeys(generateKeys(charToBitset(key))) { }

const bitset<64> DES::Encrypt(const char plainText[8]) {
	bitset<64> cipher = encrypt(charToBitset(plainText), subKeys);
	return cipher;
}

const string DES::Decrypt(const bitset<64> & cipher) {
	bitset<64> plain = decrypt(cipher, subKeys);
	return bitsetToStr(plain);
}