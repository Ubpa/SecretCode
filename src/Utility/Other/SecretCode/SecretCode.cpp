#include <Other/SecretCode.h>
#include <regex>
#include <iostream>
#include <bitset>

using namespace CppUtility::Other;
using namespace std;

SecretCode::SecretCode(const char key[8])
	: des(key) { }

vector<string> Split(const string str, const char c) {
	vector<string> rst;
	size_t start = 0;
	while (start < str.length()) {
		size_t end = str.find(c, start);
		if (end == string::npos) {
			rst.push_back(str.substr(start, str.length()-start));
			break;
		}

		rst.push_back(str.substr(start, end-start));
		start = end + 1;
	}
	return rst;
}

const string SecretCode::Encrypt(const string & plainText) {
	vector<string> lines = Split(plainText, '\n');

	string cipherText;
	for (size_t i = 0; i < lines.size(); i++) {
		string line = lines[i];
		if (line.size() == 0) {
			cipherText += "\n";
			continue;
		}

		while (line.size() % 8 != 0)
			line += '\n';
		vector<Base64::byte> lineBytes;
		for (size_t j = 0; j < line.size()/8; j++) {
			bitset<64> desCode = des.Encrypt(line.substr(j * 8, 8).c_str());
			Base64::byte val = 0;
			for (size_t k = 0; k < 64; k++) {
				val <<= 1;
				val += desCode[63 - k];
				if ((k + 1) % 8 == 0) {
					lineBytes.push_back(val);
					val = 0;
				}
			}
		}
		string base64Code = Base64::Encode(lineBytes);
		if (base64Code.size() == 0) {
			cout << "ERROR: base64 encode error\n";
			return "";
		}
		cipherText += base64Code + "\n";
	}

	return cipherText;
}

const string SecretCode::Decode(const string & cipherText) {
	vector<string> lines = Split(cipherText, '\n');

	string plainText;
	for (size_t i = 0; i < lines.size(); i++) {
		string base64Code = lines[i];
		vector<Base64::byte> desCodes = Base64::Decode(base64Code);
		if (desCodes.size() % 8 != 0) {
			cout << "ERROR: length of desCodes is not 8*n\n";
			return "";
		}

		string linePlainTex;
		for (size_t j = 0; j < desCodes.size() / 8; j++) {
			bitset<64> desCode = 0;
			for (size_t k = 0; k < 8; k++) {
				desCode <<= 8;
				desCode |= desCodes[8 * j + k];
			}
			string partLinePlainText = des.Decrypt(desCode);
			linePlainTex += partLinePlainText;
		}
		size_t idx = linePlainTex.find('\n');
		if(idx != string::npos)
			linePlainTex.erase(idx, linePlainTex.size() - idx);
		linePlainTex.push_back('\n');
		plainText += linePlainTex;
	}

	return plainText;
}