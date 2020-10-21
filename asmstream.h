#include <emscripten.h>

namespace std {
  struct AsmStream {
    int id;
  };

  AsmStream cin, cout, cerr;

  AsmStream& operator<<(AsmStream &outStream, const int data) {
    EM_ASM({
      writeToConsole($0);
    }, data);
    return outStream;
  }

  AsmStream& operator<<(AsmStream& outStream, const char data[]) {
    EM_ASM({
      writeToConsole(UTF8ToString($0));
    }, data);
    return outStream;
  }

  AsmStream& operator>>(AsmStream& inStream, int& data) {
    int gotInput = 0;
    while(!gotInput) {
      data = EM_ASM_INT({
        console.log('Checking', $0);
        if(consoleInput === null) {
          return -20968;
        } else {
          var temp = consoleInput;
          flushConsoleInput();
          return temp;
        }
      }, data);
      if(data != -20968) {
        gotInput = 1;
        break;
      }
      emscripten_sleep(200);
    }
    return inStream;
  }
}
