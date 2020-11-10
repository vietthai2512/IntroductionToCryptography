#include <iostream>
#include <string>
#include <bitset>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <fstream>

// Constant: max bitset size
#define MAXSIZE 1500

// Create new names for std::bitset<N> types 
typedef std::bitset<4> bit4_type;
typedef std::bitset<8> bit8_type;
typedef std::bitset<MAXSIZE> bit_max_type;

// Declare functions prototypes
bit_max_type hexToBit(const std::string& hex_text);
std::string bitToHex(const std::string& bit);

bit_max_type textToBit(const std::string& text);
std::string bitToText(const std::string& bit);

int hexValue(const unsigned char& hex_char);
std::string hexToText(const std::string& input);
std::string hexToTextStar(const std::string& input);
std::string textToHex(const std::string& input);

bit_max_type bitXOR(const bit_max_type& first, const bit_max_type& second);
std::string hexXOR(const std::string& hex1, const std::string& hex2);
std::string hexXORHeadToHead(const std::string& hex1, const std::string& hex2);

int main()
{
    // Open ciphertexts.txt and check if the txt file can be opened
    std::ifstream ciphertexts("One-time pad\\ciphertexts.txt");
    if (!ciphertexts.is_open())
    {
        printf("Error ciphertexts\n");
        return 1;
    }

    // Get ciphertexts line by line from txt and write to vector
    std::string line;
    std::vector<std::string> cipher_vector;
    while (std::getline(ciphertexts, line))
    {
        cipher_vector.push_back(line);
    }

    // Pick up one ciphertext per time and XOR with the target (ciphertexts[10])
    // Then write to cipherXORtarget file. 
    std::ofstream ciphersXORtarget("One-time pad\\cipherXORtarget.txt");
    for (int i = 0; i < 10; ++i)
    {
        std::string cipher_i_XOR_target = hexXORHeadToHead(cipher_vector[i], cipher_vector[10]);
        ciphersXORtarget << "[" << i << "]:" << hexToTextStar(cipher_i_XOR_target) << std::endl;
    }
    
    /* --------FIND KEYS AND PLAINTEXTS-------
    // Find key from plaintext10_v1
    std::string plaintext10_v1 = "The secret message is  When using a stream cipher  never use the key more than once";
    std::string hex_plaintext10_v1 = textToHex(plaintext10_v1);
    std::string key_v1 = hexXORHeadToHead(hex_plaintext10_v1, cipher_vector[10]);
    std::cout << key_v1 << std::endl;

    // XOR key_v1 with all the ciphertexts
    std::ofstream all_plaintexts_v1("One-time pad\\all_plaintexts_v1.txt");
    for (int i = 0; i < 11; ++i)
    {
        std::string cipher_i_XOR_key_v1 = hexXORHeadToHead(cipher_vector[i], key_v1);
        all_plaintexts_v1 << "[" << i << "]: " << hexToText(cipher_i_XOR_key_v1) << std::endl;
    }

    // Find key from plaintext1_v1
    std::string plaintext1_v1 =  "Euler would probably enjoy that now his theorem becomes a corner stone of crypto - ";
    std::string hex_plaintext1_v1 = textToHex(plaintext1_v1);
    std::string key_v2 = hexXORHeadToHead(hex_plaintext1_v1, cipher_vector[1]);
    std::cout << key_v2 << std::endl;

    // XOR key_v2 with all the ciphertexts
    std::ofstream all_plaintexts_v2("One-time pad\\all_plaintexts_v2.txt");
    for (int i = 0; i < 11; ++i)
    {
        std::string cipher_i_XOR_key_v2 = hexXORHeadToHead(cipher_vector[i], key_v2);
        all_plaintexts_v2 << "[" << i << "]: " << hexToText(cipher_i_XOR_key_v2) << std::endl;
    }

    all_plaintexts_v1.close();
    all_plaintexts_v2.close();
    */
    ciphersXORtarget.close();
    ciphertexts.close();  
}

// hexToBit: convert hex string to bitset.
bit_max_type hexToBit(const std::string &hex_text)
{
    // Insert space between every hex char
    // "5a6b8c" -> "5 a 6 b 8 c"
    std::string hex(hex_text);
    for (int i = 1; i < hex.size(); i+=2)
    {
        hex.insert(i, " ");
    }

    // Insert hex string to stringstream
    std::stringstream ss;
    ss << std::hex << hex;

    // Convert each hex char to 4-bit and copy to output stream
    std::ostringstream bit_stream;
    std::copy((std::istream_iterator<unsigned int>(ss)), std::istream_iterator<unsigned int>(), std::ostream_iterator<bit4_type>(bit_stream));

    // bit_stream to bitset and return it
    bit_max_type hex_bitset(bit_stream.str());
    return hex_bitset;
}

// textToBit: convert ASCII string to bitset
bit_max_type textToBit(const std::string &text)
{
    // Convert each char to 8-bit and copy to output stream
    std::ostringstream bit_stream;
    std::copy(text.begin(), text.end(), std::ostream_iterator<bit8_type> (bit_stream));

    // bit_stream to bitset and return it
    bit_max_type text_bitset(bit_stream.str());
    return text_bitset;
}

// bitXOR: compute XOR between 2 bitsets
bit_max_type bitXOR(const bit_max_type &first, const bit_max_type &second)
{
    return (first ^ second);
}

// hexXOR: compute XOR between 2 hex strings
//      xxxxxxxxxxxxx
// XOR          xxxxx
// =    xxxxxxxxxxxxx
std::string hexXOR(const std::string& hex1, const std::string& hex2)
{
    // Convert hex strings to bitsets
    bit_max_type bit1 = hexToBit(hex1), bit2 = hexToBit(hex2);

    // XOR 2 bitsets
    bit_max_type bit1XORbit2 = bitXOR(bit1, bit2);

    // Convert XORed bitset to hex string
    std::string output = bitToHex(bit1XORbit2.to_string());

    // Remove '0' 
    // output: "0000000xxxxxxxx" -> "xxxxxxxx"
    int hex1_length = hex1.length(), hex2_length = hex2.length(), output_length = output.length();
    int diff = hex1_length - hex2_length;
    if (diff >= 0)
    {
        output.erase(output.begin(), output.begin() + output_length - hex1_length);
    }
    else
    {
        output.erase(output.begin(), output.begin() + output_length - hex2_length);
    }
    return output;
}

// hexXORHeadToHead: compute XOR between 2 hex strings
//      xxxxxxxxxxxxx
// XOR  xxxxx
// =    xxxxxxxxxxxxx
std::string hexXORHeadToHead(const std::string& hex1, const std::string& hex2)
{
    int hex1_length = hex1.length(), hex2_length = hex2.length();
    int diff = hex1_length - hex2_length;

    // If hex1 is longer than hex2 or equal to hex2
    if (diff >= 0)
    {
        // Convert hex strings to bitsets
        bit_max_type bit1 = hexToBit(hex1), bit2 = hexToBit(hex2);

        // If hex1 is longer than hex2
        //   bit1: xxxxxxxxxxxxxxxxxxxx
        //   bit2:            xxxxxxxxx
        // ->bit2: xxxxxxxxx00000000000
        // Else don't change anything
        if (diff > 0)
        {
            bit2 <<= ((diff) * 4);
        }
        
        // XOR 2 bitsets
        bit_max_type bit1XORbit2 = bitXOR(bit1, bit2);

        // Remove '0' 
        // output: "0000000xxxxxxxx" -> "xxxxxxxx"
        std::string output = bitToHex(bit1XORbit2.to_string());
        int output_length = output.length();
        output.erase(output.begin(), output.begin() + output_length - hex1_length);
        return output;
    }
    else
    {
        return hexXORHeadToHead(hex2, hex1);
    }
}

// textToHex: convert ASCII string to hex string
std::string textToHex(const std::string &input)
{
    static const char hex_digits[] = "0123456789ABCDEF";

    std::string output;
    output.reserve(input.length() * 2);
    for (unsigned char c : input)
    {
        output.push_back(hex_digits[c >> 4]);
        output.push_back(hex_digits[c & 15]);
    }
    return output;
}

// hexValue: convert hex char to hex digit
// Example: 'A' -> 10, 'F' -> 15
int hexValue(const unsigned char &hex_char)
{
    if ('0' <= hex_char && hex_char <= '9')     {return hex_char - '0';}
    else if ('a' <= hex_char && hex_char <= 'f')    {return hex_char - 'a' + 10;}
    else if ('A' <= hex_char && hex_char <= 'F')
    {
        return hex_char - 'A' + 10;
    }
    else throw std::invalid_argument("Invalid hex digit");
}

// hexToText: convert hex string to ASCII string
std::string hexToText(const std::string &input)
{
    const auto len = input.length();

    // If length is odd throw error
    if (len & 1) throw std::invalid_argument("Odd length");

    std::string output;
    output.reserve(len / 2);
    for (std::string::const_iterator p = input.begin(); p != input.end(); p++)
    {
        int c = hexValue(*p);
        p++;
        c = (c << 4) + hexValue(*p);
        output.push_back(c);
    }
    return output;
}

// hexToTextStar: convert hex string to ASCII string
// Replace non-alphabet char to '*'
std::string hexToTextStar(const std::string& input)
{
    const auto len = input.length();
    if (len & 1) throw std::invalid_argument("Odd length");

    std::string output;
    output.reserve(len / 2);
    for (std::string::const_iterator p = input.begin(); p != input.end(); p++)
    {
        int c = hexValue(*p);
        p++;
        c = (c << 4) + hexValue(*p);
        output.push_back(' ');
        if ((65 <= c && c <= 90) || (97 <= c && c <= 122))
        {
            output.push_back(c);
        }
        else
        {
            output.push_back('*');
        }    
    }
    return output;
}

// bitToText: convert binary string to ASCII string
std::string bitToText(const std::string &bit)
{
    std::istringstream iss(bit);
    std::string output;

    // Insert every 8-bit to input stringstream
    while (iss.good())
    {
        bit8_type bits;
        iss >> bits;
        unsigned char c = (unsigned char)bits.to_ullong();
        output += c;
    }
    return output;
}

// bitToHex: convert binary string to hex string
std::string bitToHex(const std::string &bit)
{
    std::istringstream iss(bit);
    std::ostringstream oss;

    // Insert every 4-bit to input stringstream
    while (iss.good())
    {
        bit4_type bits;
        iss >> bits;
        oss << std::hex << bits.to_ullong();
    }
    std::string output = oss.str();

    // Remove last char '0'
    output.pop_back();
    return output;
}