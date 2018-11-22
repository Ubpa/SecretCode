#include <ROOT_PATH.h>

#include <Other/SecretCode.h>

#include <Other/Config.h>

#include <iostream>

using namespace CppUtility::Other;
using namespace std;

int main() {
	string text =
		"int main() {\n"
		"	string text = \"hello\\n\";\n"
		"	string key = \"12345678\";\n"
		"	SecretCode secretCode(key.c_str());\n"
		"	string cipher = secretCode.Encrypt(text);\n"
		"	cout << \"cipher:\\n\" << cipher << endl;\n"
		"	string plain = secretCode.Decode(cipher);\n"
		"	cout << \"plain:\\n\" << plain << endl;\n"
		"	return 0;\n"
		"}\n";

	string key = "87654321";
	SecretCode secretCode(key.c_str());
	string cipher = secretCode.Encrypt(text);
	cout << "cipher:\n" << cipher << endl;
	string plain = secretCode.Decrypt(cipher);
	cout << "plain:\n" << plain << endl;
	return 0;
}