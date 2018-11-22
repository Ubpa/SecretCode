#include <Other/SecretCode.h>
#include <Other/File.h>
#include <Other/ArgManager.h>
#include <ROOT_PATH.h>

using namespace CppUtility::Other;
using namespace std;

int main(int argc, char ** argv) {
	ArgManager argManager(argc, argv);

	string inFName = argManager.Get("s", 0);
	string outFName = argManager.Get("t", 0, inFName);
	string key = argManager.Get("k", 0);

	if ( inFName == ""
		|| outFName == ""
		|| key.size() != 8) {
		printf("ERROR: arg error\n"
			"INFO: [ USAGE ]\n"
			"      App_SecretCode -s [in file name] (-t [out file name])? -k [\\w{8}]\n"
			"INFO: eg. App_SecretCode -s hello.txt -k password\n"
			"INFO: eg. App_SecretCode -s hello.txt -t out.txt -k password\n");
		return 1;
	}

	SecretCode secretCode(key.c_str());

	string srcText;
	{//����ʹ��������Ϊ���ͷ� inFile
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
		break;
	case CppUtility::Other::SecretCode::CIPHER_KEY_RIGHT:
		outText = secretCode.Decrypt(srcText);
		if (outText.size() == 0) {
			printf("ERROR: Decrypt %s Error.\n", inFName.c_str());
			return 1;
		}
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