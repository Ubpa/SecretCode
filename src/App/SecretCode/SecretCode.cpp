#include <Other/SecretCode.h>
#include <Other/File.h>
#include <Other/ArgManager.h>
#include <conio.h>
#include <ROOT_PATH.h>

using namespace CppUtility::Other;
using namespace std;

void CheckInput(const string & inFName, const string & outFName, string & key);

int main(int argc, char ** argv) {
	ArgManager argManager(argc, argv);

	string inFName = argManager.Get("s", 0);
	string outFName = argManager.Get("t", 0, inFName);
	string key = argManager.Get("k", 0);
	CheckInput(inFName, outFName, key);

	SecretCode secretCode(key.c_str());

	string srcText;
	{//这里使用括号是为了释放 inFile
		File inFile(inFName, File::READ);
		if (!inFile.IsValid()) {
			printf("ERROR: in file open fail\n");
			return 1;
		}
		srcText = inFile.ReadAll();
	}
	SecretCode::TextState state = secretCode.Detect(srcText);

	string outText;
	switch (state)
	{
	case CppUtility::Other::SecretCode::NOT_CIPHER:
		outText = secretCode.Encrypt(srcText);
		if (outText.size() == 0) {
			printf("ERROR: Encrypt %s error.\n", inFName.c_str());
			return 1;
		}
		printf("INFO: Encrypt success\n");
		break;
	case CppUtility::Other::SecretCode::CIPHER_KEY_RIGHT:
		outText = secretCode.Decrypt(srcText);
		if (outText.size() == 0) {
			printf("ERROR: Decrypt %s Error.\n", inFName.c_str());
			return 1;
		}
		printf("INFO: Decrypt success\n");
		break;
	case CppUtility::Other::SecretCode::CIPHER_KEY_WRONG:
		printf("ERROR: Key is not correct.\n");
		return 1;
		break;
	default:
		printf("ERROR: text state is not known.\n");
		return 1;
		break;
	}
	
	File outFile(outFName, File::WRITE);
	if (!outFile.IsValid()) {
		printf("ERROR: out file open fail\n");
		return 1;
	}
	outFile.Printf("%s", outText.c_str());

	return 0;
}

void CheckInput(const string & inFName, const string & outFName, string & key) {
	if (inFName == "" || outFName == "") {
		printf("ERROR: arg error\n"
			"INFO: [ USAGE ]\n"
			"      App_SecretCode -s [in file name] (-t [out file name])? -k [\\w{8}]\n"
			"INFO: eg. App_SecretCode -s hello.txt -k password\n"
			"INFO: eg. App_SecretCode -s hello.txt -t out.txt -k password\n");
		exit(1);
	}
	else {
		if (key.size() == 0) {
			printf("Input Key(8 character): ");
			while (1) {
				char ch = getch();
				if (ch == 0)
					continue;
				if (ch == '\r')
					break;

				if (ch == 8) {
					if (key.size() > 0) {
						key.pop_back();
						putchar('\b');
						putchar(' ');
						putchar('\b');
					}
				}
				else {
					key += ch;
					putchar('*');
				}
			}
		}
		printf("\n");
		if (key.size() != 8) {
			printf("ERROR: key should be 8 character. eg. 12345678\n");
			exit(1);
		}
	}
}