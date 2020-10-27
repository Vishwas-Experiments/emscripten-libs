#include <emscripten.h>
#include <iostream>

namespace std {
  struct AsmOutStream : public streambuf {
  protected:
    int overflow(int ch) override {
      EM_ASM({
        writeToConsole(UTF8ToString($0));
      }, ch);
      return ch;
    }

    streamsize xsputn(const char* s, streamsize n) override {
      EM_ASM({
        writeToConsole(UTF8ToString($0));
      }, s);
      return n;
    };
  };

  struct AsmInStream {
    template <typename T>
    void getInputAsDouble(T& data) {
      data = (T) EM_ASM_DOUBLE({(
        console.log('Getting ');
        var value = consoleInput;
        flushConsoleInput();
        return value;
      )}, 0);
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

    void waitForInput() {
      int gotInput = 0;
      while(!gotInput) {
        gotInput = checkForInput();
        if(!gotInput) emscripten_sleep(200);
      }
    }
  };

  AsmInStream& operator>>(AsmInStream& inStream, float& data) {
    inStream.waitForInput();
    inStream.getInputAsDouble<float>(data);
    return inStream;
  }

  AsmInStream& operator>>(AsmInStream& inStream, int& data) {
    inStream.waitForInput();
    inStream.getInputAsDouble<int>(data);
    return inStream;
  }

  AsmInStream& operator>>(AsmInStream& inStream, unsigned int& data) {
    inStream.waitForInput();
    inStream.getInputAsDouble<unsigned int>(data);
    return inStream;
  }

  AsmInStream& operator>>(AsmInStream& inStream, double& data) {
    inStream.waitForInput();
    inStream.getInputAsDouble<double>(data);
    return inStream;
  }

  AsmInStream& operator>>(AsmInStream& inStream, char& data) {
    inStream.waitForInput();
    inStream.getInputAsDouble<char>(data);
    return inStream;
  }

  AsmInStream asmcin;
  AsmOutStream _asmcout, _asmcerr;
  ostream asmcout(&_asmcout);
  ostream asmcerr(&_asmcerr);
}
