#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#ifdef _WIN32
#include <Windows.h>

using TChar = wchar_t;
using TString = std::wstring;
#else
using TChar = char;
using TString = std::string;
#endif

static std::vector<TString> get_command_line() {
  std::vector<TString> res;
#ifdef _WIN32
  int argc;
  auto argvW = CommandLineToArgvW(GetCommandLineW(), &argc);
  if (argvW == nullptr)
    return {};
  res.reserve(argc);
  for (int i = 0; i != argc; ++i) {
    res.push_back(argvW[i]);
  }
  LocalFree(argvW);
#else
  std::ifstream file("/proc/self/cmdline", std::ios::in);
  if (file.fail())
    return {};
  std::string s;
  char c;
  while (file.get(c)) {
    if (c == '\0') {
      if (!s.empty()) {
        res.push_back(s);
        s.clear();
      }
    } else {
      s.push_back(c);
    }
  }
#endif
  return res;
}

int main() {
  auto args = get_command_line();
#ifdef _WIN32
  for (int i = 0; i < args.size(); ++i) {
    wprintf(L"%-4d %s\n", i, args[i].data());
  }
#else
  for (int i = 0; i < args.size(); ++i) {
    printf("%-4d %s\n", i, args[i].data());
  }
#endif
  return 0;
}
