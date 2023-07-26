#include <iostream>
#include <cstdio>
#include <string>
#include <cstring>
#include <ctime>

using namespace std;

namespace Rui_Ethereal {
    namespace file_io {
        namespace exio {
            const auto IO_CACHE = 1 << 16;
            inline auto IsDigit        (const char &_charact) -> bool { return '0' <= _charact and _charact <= '9'; }
            inline auto IsLowerLetter  (const char &_charact) -> bool { return 'a' <= _charact and _charact <= 'z'; }
            inline auto IsCapitalLetter(const char &_charact) -> bool { return 'A' <= _charact and _charact <= 'Z'; }
            inline auto IsLetter       (const char &_charact) -> bool { return IsLowerLetter(_charact) or IsCapitalLetter(_charact); }
            inline auto IsValidCharact (const char &_charact) -> bool { return IsDigit(_charact) or IsLetter(_charact); }
        }
        class Input {
            char buffer[exio::IO_CACHE],*in_ptr,*end_ptr;
            inline auto GetChar(void) -> char {
                if(in_ptr == end_ptr) end_ptr = (in_ptr = buffer) + fread(buffer,1,exio::IO_CACHE,stdin);
                return in_ptr == end_ptr ? EOF : *in_ptr++;
            }
            inline auto LeachSpace(char &charact) -> void { do charact = GetChar();while(!exio::IsValidCharact(charact)); }
        public:
            Input(void) : in_ptr(buffer),end_ptr(buffer) {}
            template <typename _Tp> Input& operator >> (_Tp &_data) {
                bool _neg = false;
                char _charact;
                do {
                    _charact = GetChar();
                    _neg ^= _charact == '-';
                } while(!exio::IsDigit(_charact));
                _data = 0;
                while(exio::IsDigit(_charact)) {
                    _data = (_data << 1) + (_data << 3) + (_charact ^ '0');
                    _charact = GetChar();
                }
                if(_neg) _data = ~_data + 1;
                --in_ptr;
                return *this;
            }
            Input& operator >> (char &_data) {
                LeachSpace(_data);
                return *this;
            }
            Input& operator >> (char *_data) {
                LeachSpace(*_data);
                while(exio::IsValidCharact(*_data)) *++_data = GetChar();
                *_data = '\0';
                --in_ptr;
                return *this;
            }
            Input& operator >> (std::string &_data) {
                char _charact;
                LeachSpace(_charact);
                _data.clear();
                while(exio::IsValidCharact(_charact)) {
                    _data.push_back(_charact);
                    _charact = GetChar();
                }
                --in_ptr;
                return *this;
            }
        };
        class Output {
            char buffer[exio::IO_CACHE],*out_ptr;
            inline auto Flush(void) -> void {
                fwrite(buffer,1,out_ptr - buffer,stdout);
                out_ptr = buffer;
            }
            inline auto PutChar(const char &_charact) -> void {
                if(out_ptr - buffer == exio::IO_CACHE) Flush();
                *out_ptr++ = _charact;
            }
        public:
            Output(void) : out_ptr(buffer) {}
            ~Output(void) { Flush(); }
            template <typename _Tp> Output& operator << (_Tp _data) {
                if(_data < 0) {
                    PutChar('-');
                    _data = ~_data + 1;
                }
                static char _bit[44];
                short _top = 0;
                do _bit[_top++] = static_cast <char> (_data % 10 + '0');while(_data /= 10);
                while(_top) PutChar(_bit[--_top]);
                return *this;
            }
            Output& operator << (const char &_data) {
                PutChar(_data);
                return *this;
            }
            Output& operator << (const char *_data) {
                size_t cur = 0;
                while(_data[cur]) PutChar(_data[cur++]);
                return *this;
            }
            Output& operator << (const std::string &_data) {
                size_t cur = 0;
                const size_t end_addr = _data.length();
                while(cur < end_addr) PutChar(_data[cur++]);
                return *this;
            }
        };
    }
}
