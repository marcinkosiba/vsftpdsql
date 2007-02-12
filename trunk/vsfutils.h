#ifndef VSFUTILS_H
#define VSFUTILS_H
#include <iostream>
#include <string>
#include <vector>
#include "cpp_to_c_helpers.h"
#include "vsfutils_c.h"

class VSFUtils {
private:
  static void tokenize(const std::string& str, std::vector<std::string>& tokens, const std::string& delimiters);
  static void tokenize(const std::string& str, std::vector<std::string>& tokens);
  static std::string trim(std::string& s);
public:
  static std::pair<std::string,std::string> sanitizePath(std::string&,std::string& );
  static std::pair<std::string,std::string> sanitizePath(const char * ,const char * );
};

#endif
