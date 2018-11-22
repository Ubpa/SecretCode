#include <ROOT_PATH.h>
#include <Other/Base64.h>
#include <Other/Config.h>
#include <cstdio>

using namespace CppUtility::Other;
using namespace std;

int main() {
	Config config(string(ROOT_PATH) + "/config/config.out");
	unsigned int code;
	config.GetVal("Example_01_Code", code, 0xFEDCBA98);

	vector<Base64::byte> bytes;
	for (size_t i = 0; i < 4; i++)
		bytes.push_back((code >> ((3 - i) * 8)) & 0xFF);
	for (size_t i = 0; i < bytes.size(); i++)
		printf("%ud\n", bytes[i]);

	string encode = Base64::Encode(bytes);
	printf("%s\n", encode.c_str());

	vector<Base64::byte> origin = Base64::Decode(encode);
	for (size_t i = 0; i < origin.size(); i++)
		printf("%ud\n", origin[i]);

	if (origin == bytes)
		printf("same\n");
	else
		printf("not same\n");

	return 0;
}