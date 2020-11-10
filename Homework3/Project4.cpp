#include <fstream>
#include <string>
#include <iostream>

// How to install Crypto++: https://youtu.be/fDvbE7LQDqE
#include "..\cryptopp\sha.h"
#include "..\cryptopp\hex.h"

const int BLOCK_SIZE = 1024;
const int SHA_OUTPUT_SIZE = 32;
const int SHA_INPUT_SIZE = BLOCK_SIZE + SHA_OUTPUT_SIZE;

int main()
{
	// Ask user for video URL
	std::string file_name;
	std::cout << "Nhap dia chi file: ";
	std::cin >> file_name;

	// Read the video in binary mode
	std::ifstream file_input(file_name, std::ifstream::in | std::ifstream::binary | std::ifstream::ate);
	if (!file_input.is_open())
	{
		printf("Cannot open file!");
		return 1;
	}

	// Calculate file size
	int file_size = file_input.tellg();

	// Number of blocks, Last block size
	int num_of_blocks, curr_block_size;
	unsigned char curr_block[SHA_INPUT_SIZE], sha_block[SHA_OUTPUT_SIZE];

	// If file size is a multiple of BLOCK_SIZE
	if (file_size % BLOCK_SIZE == 0)
	{
		num_of_blocks = file_size / BLOCK_SIZE;
		curr_block_size = BLOCK_SIZE;
	}
	else
	{
		num_of_blocks = file_size / BLOCK_SIZE + 1;
		curr_block_size = file_size - BLOCK_SIZE * (num_of_blocks - 1);
	}

	// Iterating from the last block
	for (int i = num_of_blocks - 1; i >= 0; i--)
	{
		// Set the position to the beginning of the block
		file_input.seekg(i * BLOCK_SIZE, std::ifstream::beg);

		// Read the block from file and write to curr_block
		file_input.read((char*)curr_block, curr_block_size);

		// Last block
		if (i == num_of_blocks - 1)
		{
			CryptoPP::SHA256().CalculateDigest(sha_block, curr_block, curr_block_size);
			curr_block_size = BLOCK_SIZE;
		}
		else
		{
			for (int j = 0; j < SHA_OUTPUT_SIZE; j++)
			{
				curr_block[BLOCK_SIZE + j] = sha_block[j];
			}
			CryptoPP::SHA256().CalculateDigest(sha_block, curr_block, SHA_INPUT_SIZE);
		}
	}

	// Convert result to hex
	CryptoPP::HexEncoder encoder;
	std::string result;
	encoder.Attach(new CryptoPP::StringSink(result));
	encoder.Put(sha_block, SHA_OUTPUT_SIZE);
	encoder.MessageEnd();
	std::cout << "Ma hexa h0 can tim: " << result << std::endl;
	system("pause");
}