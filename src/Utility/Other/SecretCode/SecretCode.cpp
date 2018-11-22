#include <Other/SecretCode.h>
#include <iostream>

using namespace CppUtility::Other;
using namespace std;

const string DefualtHeader = "Ubpa_SecretCode_Defualt_Header\n";
const string UserHeader = "Ubpa_SecretCode_User_Header\n";
const DES SecretCode::defaultHeaderDES("password");

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

const std::string SecretCode::EncryptLine(const DES & des, std::string linePlainText) {
	if (linePlainText.size() == 0)
		return "\n";

	while (linePlainText.size() % 8 != 0)
		linePlainText += '\n';

	vector<Base64::byte> lineBytes;
	for (size_t j = 0; j < linePlainText.size() / 8; j++) {
		bitset<64> desCode = des.Encrypt(linePlainText.substr(j * 8, 8).c_str());
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
	if (base64Code.size() == 0)
		return "";

	return base64Code + "\n";
}

const string SecretCode::DecryptLine(const DES & des, const string & lineCipherText) {
	vector<Base64::byte> desCodes = Base64::Decode(lineCipherText);
	if (desCodes.size() % 8 != 0)
		return "";

	string linePlainTex;
	for (size_t j = 0; j < desCodes.size() / 8; j++) {
		bitset<64> desCode = 0;
		for (size_t k = 0; k < 8; k++) {
			desCode <<= 8;
			desCode |= desCodes[8 * j + k];
		}
		linePlainTex += des.Decrypt(desCode);
	}
	size_t idx = linePlainTex.find('\n');
	if (idx != string::npos)
		linePlainTex.erase(idx, linePlainTex.size() - idx);
	linePlainTex.push_back('\n');

	return linePlainTex;
}

const string SecretCode::Encrypt(const string & plainText) const {
	vector<string> lines = Split(plainText, '\n');

	string cipherText = EncryptLine(defaultHeaderDES, DefualtHeader) + EncryptLine(des, UserHeader);
	for (size_t i = 0; i < lines.size(); i++) {
		string cipherLine = EncryptLine(des, lines[i]);
		if (cipherLine.size() == 0) {
			cout << "ERROR: Encrypt wrong.\n";
			return "";
		}

		cipherText += cipherLine;
	}

	return cipherText;
}

const string SecretCode::Decrypt(const string & cipherText) const{
	if (Detect(cipherText) != TextState::CIPHER_KEY_RIGHT) {
		cout << "ERROR: cipherText or key is wrong.\n";
		return "";
	}

	vector<string> lines = Split(cipherText, '\n');
	string plainText;
	//Ìø¹ýÁ½ÐÐ
	for (size_t i = 2; i < lines.size(); i++) {
		string linePlainTex = DecryptLine(des, lines[i]);
		if (linePlainTex.size() == 0) {
			cout << "ERROR: Decode wrong\n";
			return "";
		}

		plainText += linePlainTex;
	}

	return plainText;
}

SecretCode::TextState SecretCode::Detect(const std::string & text) const{
	vector<string> lines = Split(text, '\n');
	if (lines.size() < 2)
		return TextState::NOT_CIPHER;
	
	string firstLinePlainText = DecryptLine(defaultHeaderDES, lines[0]);
	if (firstLinePlainText != DefualtHeader)
		return TextState::NOT_CIPHER;

	string secondLinePlainText = DecryptLine(des, lines[1]);
	if (secondLinePlainText != UserHeader)
		return TextState::CIPHER_KEY_WRONG;

	return CIPHER_KEY_RIGHT;
}