#include <cstdarg>
#include <string>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#elif defined(__APPLE__)
#include <crt_externs.h>
#else
#include <fstream>
#endif

#ifdef _WIN32
std::string wideToUtf8(const std::wstring &utf16Str) {
  if (utf16Str.empty()) {
    return {};
  }
  const auto utf16Length = static_cast<int>(utf16Str.size());
  if (utf16Length >= (std::numeric_limits<int>::max)()) {
    return {};
  }
  const int utf8Length =
      ::WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, utf16Str.data(),
                            utf16Length, nullptr, 0, nullptr, nullptr);
  if (utf8Length <= 0) {
    return {};
  }
  std::string utf8Str;
  utf8Str.resize(utf8Length);
  ::WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, utf16Str.data(),
                        utf16Length, &utf8Str[0], utf8Length, nullptr, nullptr);
  return utf8Str;
}
#endif

std::vector<std::string> commandLineArguments() {
  std::vector<std::string> res;
#ifdef _WIN32
  int argc;
  auto argvW = ::CommandLineToArgvW(::GetCommandLineW(), &argc);
  if (argvW == nullptr)
    return {};
  res.reserve(argc);
  for (int i = 0; i != argc; ++i) {
    res.push_back(wideToUtf8(argvW[i]));
  }
  ::LocalFree(argvW);
#elif defined(__APPLE__)
  auto argv = *(_NSGetArgv());
  for (int i = 0; argv[i] != nullptr; ++i) {
    res.push_back(argv[i]);
  }
#else
  std::ifstream file("/proc/self/cmdline", std::ios::in);
  if (file.fail())
    return {};
  std::string s;
  while (std::getline(file, s, '\0')) {
    res.push_back(s);
  }
  file.close();
#endif
  return res;
}

int u8printf(const char *fmt, ...) {
#ifdef _WIN32
  auto cp = ::GetConsoleOutputCP();
  ::SetConsoleOutputCP(CP_UTF8);
#endif

  va_list args;
  va_start(args, fmt);
  int res = vprintf(fmt, args);
  va_end(args);

#ifdef _WIN32
  ::SetConsoleOutputCP(cp);
#endif
  return res;
}

int main(int /* argc */, char * /* argv */[]) {
  auto args = commandLineArguments();
  for (int i = 0; i < args.size(); ++i) {
    u8printf("%-4d %s\n", i, args[i].data());
  }
  return 0;
}
