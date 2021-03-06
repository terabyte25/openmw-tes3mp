#include "Utils.hpp"

#include <cstdio>
#include <cstring>
#include <ctime>
#include <cmath>
#include <memory>
#include <iostream>
#include <sstream>
#include <boost/crc.hpp>
#include <boost/filesystem/fstream.hpp>
#include <iomanip>

using namespace std;

#ifdef _WIN32
int setenv(const char *name, const char *value, int overwrite)
{
    printf("%s: %s\n", name, value);
    return _putenv_s(name, value);
}
#endif


std::string Utils::convertPath(std::string str)
{
#if defined(_WIN32)
#define _SEP_ '\\'
#elif defined(__APPLE__)
#define _SEP_ ':'
#endif

#if defined(_WIN32) || defined(__APPLE__)
    replace(str.begin(), str.end(), '/', _SEP_);
#endif //defined(_WIN32) || defined(__APPLE__)
    return str;

#undef _SEP_
}

bool Utils::doesFileHaveChecksum(std::string filePath, unsigned int requiredChecksum)
{
    unsigned int fileChecksum = crc32Checksum(filePath);

    if (fileChecksum == requiredChecksum)
        return true;

    return false;
}

void Utils::timestamp()
{
    time_t ltime;
    ltime = time(0);
    char t[32];
    snprintf(t, sizeof(t), "[%s", asctime(localtime(&ltime)));
    char* newline = strchr(t, '\n');
    *newline = ']';
    strcat(t, " ");
    printf("%s", t);
}

// Based on http://stackoverflow.com/questions/1637587/c-libcurl-console-progress-bar
int Utils::progressFunc(double TotalToDownload, double NowDownloaded)
{
    // how wide you want the progress meter to be
    int totaldotz=40;
    double fractiondownloaded = NowDownloaded / TotalToDownload;
    // part of the progressmeter that's already "full"
    int dotz = round(fractiondownloaded * totaldotz);

    // create the "meter"
    int ii=0;
    printf("%3.0f%% [",fractiondownloaded*100);
    // part  that's full already
    for ( ; ii < dotz;ii++) {
        printf("=");
    }
    // remaining part (spaces)
    for ( ; ii < totaldotz;ii++) {
        printf(" ");
    }
    // and back to line begin - do not forget the fflush to avoid output buffering problems!
    printf("]\r");
    fflush(stdout);
    return 1;
}

bool Utils::compareDoubles(double a, double b, double epsilon)
{
    return fabs(a - b) < epsilon;
}

bool Utils::compareFloats(float a, float b, float epsilon)
{
    return fabs(a - b) < epsilon;
}

// Based on https://stackoverflow.com/a/1489873
unsigned int Utils::getNumberOfDigits(int integer)
{
    int digits = 0;
    if (integer < 0) digits = 1;
    while (integer) {
        integer /= 10;
        digits++;
    }
    return digits;
}



std::string Utils::toString(int num)
{
    std::ostringstream stream;
    stream << num;
    return stream.str();
}

std::string Utils::replaceString(const string& source, const char* find, const char* replace)
{
    unsigned int find_len = strlen(find);
    unsigned int replace_len = strlen(replace);
    size_t pos = 0;

    string dest = source;

    while ((pos = dest.find(find, pos)) != string::npos)
    {
        dest.replace(pos, find_len, replace);
        pos += replace_len;
    }

    return dest;
}

string& Utils::removeExtension(string& file)
{
    size_t pos = file.find_last_of('.');

    if (pos)
        file = file.substr(0, pos);

    return file;
}

long int Utils::getFileLength(const char* file)
{
    FILE* _file = fopen(file, "rb");

    if (!_file)
        return 0;

    fseek(_file, 0, SEEK_END);
    long int size = ftell(_file);
    fclose(_file);

    return size;
}

unsigned int ::Utils::crc32Checksum(const std::string &file)
{
    boost::crc_32_type  crc32;
    boost::filesystem::ifstream  ifs(file, std::ios_base::binary);
    if (ifs)
    {
        do
        {
            char buffer[1024];

            ifs.read(buffer, 1024);
            crc32.process_bytes( buffer, ifs.gcount() );
        } while (ifs);
    }
    return crc32.checksum();
}

std::string Utils::getOperatingSystemType()
{
#if defined(_WIN32)
    return "Windows";
#elif defined(__linux)
    return "Linux";
#elif defined(__APPLE__)
    return "OS X";
#else
    return "Unknown OS";
#endif
}

std::string Utils::getArchitectureType()
{
#if defined(__x86_64__) || defined(_M_X64)
    return "64-bit";
#elif defined(__i386__) || defined(_M_I86)
    return "32-bit";
#elif defined(__ARM_ARCH)
    return "ARMv" + __ARM_ARCH;
#ifdef __aarch64__
    return "64-bit";
#else
    return "32-bit";
#endif
#else
    return "Unknown architecture";
#endif
}

void Utils::printVersion(std::string appName, std::string version, std::string commitHash, int protocol)
{
    cout << appName << " " << version;
    cout << " (" << getOperatingSystemType() << " " << getArchitectureType() << ")" << endl;
    cout << "Protocol version: " << protocol << endl;
    cout << "Oldest compatible commit hash: " << commitHash.substr(0, 10) << endl;

    cout << "------------------------------------------------------------" << endl;
}

void Utils::printWithWidth(ostringstream &sstr, string str, size_t width)
{
    sstr << left << setw(width) << setfill(' ') << str;
}

string Utils::intToHexStr(unsigned val)
{
    ostringstream sstr;
    sstr << "0x" << setfill('0') << setw(8) << uppercase << hex << val;
    return sstr.str();
}

unsigned int Utils::hexStrToInt(std::string hexString)
{
    unsigned int intValue;
    sscanf(hexString.c_str(), "%x", &intValue);
    return intValue;
}
