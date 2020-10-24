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
    int underflow() override {
      int gotInput = 0, data;
      while(!gotInput) {
        gotInput = EM_ASM_INT({
          console.log('Checking');
          if(consoleInput === null) {
            return 0;
          } else {
            return 1;
          }
        }, 0);
        if(!gotInput) emscripten_sleep(200);
      }
      return EM_ASM_INT({
        if(consoleInput.trim() === "") {
          flushConsoleInput();
          return $0;
        }
        var value = consoleInput.charCodeAt(0);
        console.log(value, 'underflow');
        return value;
      }, EOF);
    }

    int uflow() override {
      int gotInput = 0, data;
      while(!gotInput) {
        gotInput = EM_ASM_INT({{
          console.log('Checking');
          if(consoleInput === null) {
            return 0;
          } else {
            return 1;
          }
        }}, 0);
        if(!gotInput) emscripten_sleep(200);
      }
      return EM_ASM_INT({(
        if(consoleInput.trim() === "") {
          flushConsoleInput();
          return $0;
        }
        var value = consoleInput.charCodeAt(0);
        consoleInput = consoleInput.slice(1);
        console.log(value, 'uflow');
        return value;
      )}, EOF);
    }
  };

  struct AsmStream {
    int id;
  };

  AsmInStream _asmcin;
  AsmOutStream _asmcout, _asmcerr;
  ostream asmcout(&_asmcout);
  ostream asmcerr(&_asmcerr);

  istream asmcin(&_asmcin);
}
