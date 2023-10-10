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
  auto f = fopen("/proc/self/cmdline", "rb");
  char ch;
  int argc = 0;
  if (f) {
    while ((ch = fgetc(f)) != EOF) {
      if (ch == '\0')
        argc++;
    }
    fseek(f, 0, SEEK_SET);
    char **argv = malloc((argc + 1) * sizeof(char *));
    char *arg = NULL;
    size_t size;
    for (int i = 0; i < argc; i++) {
      getline(&arg, &size, f); // Read until \0
      argv[i] = strdup(arg);   // Copy the argument
    }
    argv[argc] = NULL; // NULL-terminate the list

    // Use argc and argv as needed
    res.reserve(argc);
    for (int i = 0; i != argc; ++i) {
      res.push_back(argv[i]);
    }

    for (int i = 0; i < argc; i++)
      free(argv[i]);
    free(argv);
  } else {
    return {};
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
