#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>
#include <map>

void frequencyAnalysis(std::ifstream& ciphertext);
void replace(std::ifstream& ciphertext, std::ifstream& key);

int main()
{
    std::ifstream ciphertext ("BaiTap1_NguyenVietThai\\ciphertext.txt");
    std::ifstream key ("BaiTap1_NguyenVietThai\\key.txt");

    // Check if the text file can be opened
    if (!ciphertext.is_open())
    {
        printf("Error1\n");
        return 1;
    }
    if (!key.is_open())
    {
        printf("Error2\n");
    }

    //frequencyAnalysis(ciphertext);
    replace(ciphertext, key);
    key.close();
    ciphertext.close();
}

void frequencyAnalysis(std::ifstream& ciphertext)
{
    // Construct frequency map and reverse frequency multimap
    std::map<char,int> freqMap;
    std::multimap<int,char> revFreqMap;
    for (int i = 0; i < 26; ++i)
    {
        freqMap[char(i + 97)] = 0;
    }

    // Find the frequency of all the letters
    char ch;
    while (ciphertext.get(ch))
    {
        if (tolower(ch) >= 'a' && tolower(ch) <= 'z')
        {
            freqMap[ch]++;
        }
    }

    // Swap values and keys in freqMap to revFreqMap
    for (auto elem : freqMap)
    {
        revFreqMap.insert(std::pair<int,char>(elem.second, elem.first));
    }

    // Write to text file
    std::ofstream key ("BaiTap1_NguyenVietThai\\key.txt");
    for (auto rit = revFreqMap.rbegin(); rit != revFreqMap.rend(); ++rit)
    {
        key << rit->second << std::endl;
    }
    key.close();
}

void replace(std::ifstream& ciphertext, std::ifstream& key)
{
    // Check if the text file can be opened
    if (!key.is_open()) {return;}
    if (!ciphertext.is_open()) {return;}

    // Construct key vector
    std::string line;
    std::vector<char> keyVector (26);
    while (std::getline(key, line))
    {
        keyVector[(int)line[0] - 97] = line[1];
    }

    // Iterate over characters in ciphertext, replace it with key and write to original
    std::ofstream original ("BaiTap1_NguyenVietThai\\original.txt");
    char ch;
    while (ciphertext.get(ch))
    {
        if (tolower(ch) >= 'a' && tolower(ch) <= 'z')
        {
            original << keyVector[ch - 97];
        }
        else
        {
            original << ch;
        }
    }
    original.close();
}
