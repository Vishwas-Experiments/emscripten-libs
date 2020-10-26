#include <emscripten.h>
#include <iostream>

namespace std {
  struct AsmOutStream : public streambuf {
  protected:
    streamsize xsputn(const char* s, streamsize n) override {
      EM_ASM({
        writeToConsole(UTF8ToString($0));
      }, s);
      return n;
    };

    int overflow(int ch) override {
      EM_ASM({
        writeToConsole(UTF8ToString($0));
      }, ch);
      return ch;
    }
  };

  struct AsmInStream : public streambuf {
  protected:
    // streambuf::int_type underflow() override {
    //   int gotInput = 0, data;
    //   while(!gotInput) {
    //     gotInput = checkForInput();
    //     if(!gotInput) emscripten_sleep(200);
    //   }
    //   return getInput(0);
    // }
    //
    // streambuf::int_type uflow() override {
    //   bufferSize = 10;
    //   bufferIndex = 0;
    //   buffer = (char*)realloc(buffer, sizeof(char)*bufferSize);
    //   memset(buffer, 0, bufferSize);
    //
    //   int gotInput = 0, data;
    //   while(!gotInput) {
    //     gotInput = checkForInput();
    //     if(!gotInput) emscripten_sleep(200);
    //   }
    //
    //   while(!isEmpty()) {
    //     streambuf::int_type input = getInput(1);
    //
    //     if(bufferSize - bufferIndex < 3) {
    //       bufferSize += 10;
    //       buffer = (char*)realloc(buffer, sizeof(char)*bufferSize);
    //     }
    //
    //     buffer[bufferIndex++] = traits_type::to_char_type(input);
    //   }
    //   setg(buffer, buffer, buffer + bufferIndex);
    //
    //   EM_ASM_INT({
    //     console.log(UTF8ToString($0));
    //   }, buffer);
    //   return traits_type::to_int_type(*gptr());
    // }
    streamsize xsgetn(char* s, streamsize n) {
      int i = 0;

      int gotInput = 0, data;
      while(!gotInput) {
        gotInput = checkForInput();
        if(!gotInput) emscripten_sleep(200);
      }

      while(!isEmpty() && i < n) {
        streambuf::int_type input = getInput(1);
        s[i++] = traits_type::to_char_type(input);
      }
      EM_ASM_INT({
        console.log(UTF8ToString($0));
      }, s);
      return i;
    }
  private:
    char* buffer;
    int bufferSize, bufferIndex;

    int isEmpty() {
      return EM_ASM_INT({(
        if(consoleInput.trim() === "" || consoleInput == null) {
          flushConsoleInput();
          return 1;
        } else {
          return 0;
        }
      )});
    }

    streambuf::int_type getInput(int advanceToNext) {
      return EM_ASM_INT({(
        console.log('Getting ', $0);
        var value = consoleInput.charCodeAt(0);
        if($0) {consoleInput = consoleInput.slice(1);}
        return value;
      )}, advanceToNext);
    }

    int checkForInput() {
      return EM_ASM_INT({{
        console.log('Checking');
        if(consoleInput === null) {
          return 0;
        } else {
          return 1;
        }
      }}, 0);
    }

  public:
    AsmInStream() {
      bufferSize = 10;
      bufferIndex = 0;
      buffer = new char[bufferSize];
    }

    ~AsmInStream() {
      delete[] buffer;
    }
  };

  AsmInStream _asmcin;
  AsmOutStream _asmcout, _asmcerr;
  ostream asmcout(&_asmcout);
  ostream asmcerr(&_asmcerr);

  istream asmcin(&_asmcin);
}
