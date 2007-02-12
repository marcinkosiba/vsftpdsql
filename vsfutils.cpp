#include "vsfutils.h"
using namespace std;

void VSFUtils::tokenize(const string& str,
                      vector<string>& tokens,
                      const string& delimiters)
{
    // Skip delimiters at beginning.
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    string::size_type pos     = str.find_first_of(delimiters, lastPos);

    while (string::npos != pos || string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}

void VSFUtils::tokenize(const string& str,
                      vector<string>& tokens)
{tokenize(str,tokens,"/");}

std::string VSFUtils::trim(std::string& s)
{
 std::string r=s.erase(s.find_last_not_of(" ")+1);
 return r.erase(0,r.find_first_not_of(" "));
}
std::pair<std::string,std::string> VSFUtils::sanitizePath(const char * src_path_c,const char * src_name_c)
{
  std::string src_path;
  std::string src_name;
  src_path=src_path_c;
  src_name=src_name_c;
  return sanitizePath(src_path,src_name);
}
std::pair<std::string,std::string> VSFUtils::sanitizePath(std::string& src_path,std::string& src_name)
{
  std::pair <std::string,std::string> result;
  std::string temp;
  std::string res_path;
  std::string res_name;
  std::vector<string> tokens;
  std::vector<string> tokensBis;
  vector<string>::iterator tempIterator;
  int path_is_global;

 //trimming
  src_path=trim(src_path);
  src_name=trim(src_name);
  //linking
  temp=src_path+"/"+src_name;
  if(temp[0]=='/')
  {
    path_is_global=1;
  }
  else
  {
    path_is_global=0;
  }
  //tokenizing
  tokenize(temp,tokens);
  //removing unwanted tokens
  for(tempIterator=tokens.begin();tempIterator != tokens.end(); tempIterator++ )
  {
    //removing multiple slash (//)
    if ((*tempIterator).length()==0)
    {
      continue;
    }
    //removing selfpaths (/./)
    if ((*tempIterator)==".")
    {
        continue;
    }
    //removing updirs (/../)
    if ((*tempIterator)=="..")
    {
      if (!tokensBis.empty())
        tokensBis.pop_back();
      continue;
    }
    tokensBis.push_back(*tempIterator);
  }

  if (!tokensBis.empty())
  {
    res_name = tokensBis.back();
    tokensBis.pop_back();
  }
  else
  {
    res_name = ".";
  }
  if(path_is_global)
  {
    res_path="/";
  }
  for (tempIterator=tokensBis.begin();tempIterator != tokensBis.end(); tempIterator++)
  {
    res_path += *tempIterator + "/";
  }
  //reassembling
  result.first=res_path;
  result.second=res_name;
  return result;
}

C_COMPATIBLE_FUNCTION void sanitize_path_and_name(const char** path, const char** name) {
  string sPath(*path);
  string sName(*name);  
  pair<string, string> ret = VSFUtils::sanitizePath(sPath, sName);
  free(const_cast<char*>(*path));
  free(const_cast<char*>(*name));
  *path = strdup(ret.first.c_str());
  *name = strdup(ret.second.c_str());
}

C_COMPATIBLE_FUNCTION void sanitize_path(const char** pathAndName) {
  string sPath(*pathAndName);
  string sName("name");
  pair<string, string> ret = VSFUtils::sanitizePath(sPath, sName);  
  free(const_cast<char*>(*pathAndName));
  *pathAndName = strdup(ret.first.substr(0,ret.first.length()-1).c_str());
  //*pathAndName = strdup(ret.first.c_str());
}


/*
//main function... for testing purposes only!
int main()
{
  string p;
  string n;
  cout << "PATH?:";
  cin >> p;
  cout << "NAME?:";
  cin >> n;
  cout << "\nRESULT:\nPATH:" << VSFUtils::sanitizePath(p,n).first << endl << "NAME:" << VSFUtils::sanitizePath(p,n).second << endl;
}
//*/
