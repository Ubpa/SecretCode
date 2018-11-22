#include <Other/Base64.h>

using namespace CppUtility::Other;
using namespace std;

/*这是Base64编码使用的标准字典*/
const string Base64Table = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
const char Base64Pad = '=';

const char DecodeTable[256] =
{//空白符 \n \r \t SPACE 为 -1,, 有效>0, 等号0, 其余-2
	-2, -2, -2, -2, -2, -2, -2, -2, -2, -1, -1, -2, -2, -1, -2, -2,
	-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
	-1, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, 62, -2, -2, -2, 63,
	52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -2, -2, -2,  0, -2, -2,
	-2,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
	15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -2, -2, -2, -2, -2,
	-2, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
	41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -2, -2, -2, -2, -2,
	-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
	-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
	-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
	-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
	-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
	-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
	-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
	-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2
};

string Base64::Encode(const vector<byte> & code) {
	string rst;

	size_t len = code.size();
	for (size_t i = 0; i + 2 < len; i += 3) {
		rst += Base64Table[code[i] >> 2];
		rst += Base64Table[((code[i] & 0x03) << 4) + (code[i + 1] >> 4)];
		rst += Base64Table[((code[i + 1] & 0x0f) << 2) + (code[i + 2] >> 6)];
		rst += Base64Table[code[i + 2] & 0x3f];
	}

	if (len % 3 == 1) {
		rst += Base64Table[code[len - 1] >> 2];
		rst += Base64Table[(code[len - 1] & 0x03) << 4];
		rst += Base64Pad;
		rst += Base64Pad;
	}
	if (len % 3 == 2) {
		rst += Base64Table[code[len - 2] >> 2];
		rst += Base64Table[((code[len - 2] & 0x03) << 4) + (code[len-1] >> 4)];
		rst += Base64Table[(code[len - 1] & 0x0f) << 2];
		rst += Base64Pad;
	}

	return rst;
}

const vector<Base64::byte> Base64::Decode(const string & code) {
	vector<byte> rst;
	size_t len = code.size();

	unsigned int binVal = 0;
	size_t num = 0;
	size_t pad = 0;
	for (size_t i = 0; i < len; i++) {
		char val = DecodeTable[code[i]];
		// 空白符
		if (val == -1)
			continue;

		if (val == -2
			|| (pad == 1 && code[i] != Base64Pad)
			|| pad == 2)
			return vector<byte>();

		if (code[i] == Base64Pad)
			pad++;

		binVal = binVal << 6 | val;
		if (++num != 4)
			continue;

		rst.push_back(binVal >> 16);
		rst.push_back((binVal >> 8) & 0xff);
		rst.push_back(binVal & 0xff);
		binVal = 0;
		num = 0;
	}
	
	if(num != 0)
		return vector<byte>();

	while (pad-- > 0)
		rst.pop_back();

	return rst;
}