#include <emscripten.h>
#include <iomanip>

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

  AsmStream& operator<<(AsmStream &outStream, const float data) {
    EM_ASM({
      writeToConsole($0);
    }, data);
    return outStream;
  }

  AsmStream& operator<<(AsmStream &outStream, const double data) {
    EM_ASM({
      writeToConsole($0);
    }, data);
    return outStream;
  }
  AsmStream& operator<<(AsmStream &outStream, const __iom_t4<char> data) {
    EM_ASM({
      writeToConsole(UTF8ToString($0));
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

  AsmStream& operator>>(AsmStream& inStream, float& data) {
    int gotInput = 0;
    while(!gotInput) {
      data = (float) EM_ASM_DOUBLE({
        console.log('Checking', $0);
        if(consoleInput === null) {
          return -20968.0;
        } else {
          var temp = consoleInput;
          flushConsoleInput();
          return temp;
        }
      }, data);
      if(data != -20968.0) {
        gotInput = 1;
        break;
      }
      emscripten_sleep(200);
    }
    return inStream;
  }

  AsmStream& operator>>(AsmStream& inStream, double& data) {
    int gotInput = 0;
    while(!gotInput) {
      data = EM_ASM_DOUBLE({
        console.log('Checking', $0);
        if(consoleInput === null) {
          return -20968.0;
        } else {
          var temp = consoleInput;
          flushConsoleInput();
          return temp;
        }
      }, data);
      if(data != -20968.0) {
        gotInput = 1;
        break;
      }
      emscripten_sleep(200);
    }
    return inStream;
  }
}
