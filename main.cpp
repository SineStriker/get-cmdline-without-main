#include <iostream>
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
#ifdef _WIN32
  int argc;
  auto argvW = CommandLineToArgvW(GetCommandLineW(), &argc);
  if (argvW == nullptr)
    return {};
  std::vector<TString> res;
  res.reserve(argc);
  for (int i = 0; i != argc; ++i) {
    res.push_back(argvW[i]);
  }
  LocalFree(argvW);
  return res;
#else

#endif
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
