#include <Other/SecretCode.h>
#include <Other/File.h>
#include <Other/ArgManager.h>
#include <ROOT_PATH.h>

using namespace CppUtility::Other;
using namespace std;

int main(int argc, char ** argv) {
	ArgManager argManager(argc, argv);

	string mode = argManager.Get("m", 0);
	string inFName = argManager.Get("s", 0);
	string outFName = argManager.Get("t", 0);
	string key = argManager.Get("k", 0);

	if ((mode != "e" && mode != "d")
		|| inFName == ""
		|| outFName == ""
		|| key.size() != 8) {
		printf("ERROR: arg error\n"
			"INFO: [ USAGE ]\n"
			"      App_SecretCode -m [e/d] -s [in file name] -t [out file name] -k [\\w{8}]\n"
			"INFO: eg. App_SecretCode -m e -s hello.txt -t out.txt -k password\n");
		return 1;
	}

	SecretCode secretCode(key.c_str());

	File inFile(inFName, File::READ);
	if (!inFile.IsValid()) {
		printf("ERROR: in file open fail\n");
		return 1;
	}
	string srcText = inFile.ReadAll();

	string outText;
	if (mode == "e")
		outText = secretCode.Encrypt(srcText);
	else
		outText = secretCode.Decode(srcText);

	File outFile(outFName, File::WRITE);
	if (!outFile.IsValid()) {
		printf("ERROR: out file open fail\n");
		return 1;
	}
	outFile.Printf("%s", outText.c_str());

	return 0;
}