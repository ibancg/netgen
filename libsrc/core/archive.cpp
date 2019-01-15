
#include "archive.hpp"

#ifndef WIN32
#include <cxxabi.h>
#endif

namespace ngcore
{
  // clang-tidy should ignore this static object
  static std::map<std::string, VersionInfo> library_versions;  // NOLINT
  std::map<std::string, VersionInfo>& Archive :: GetLibraryVersions()
  {
    return library_versions;
  }
  const VersionInfo& GetLibraryVersion(const std::string& library)
  { return library_versions[library]; }

  void SetLibraryVersion(const std::string& library, const VersionInfo& version)
  { library_versions[library] = version; }

  // clang-tidy should ignore this static object
  static std::unique_ptr<std::map<std::string, detail::ClassArchiveInfo>> type_register;  // NOLINT
  const detail::ClassArchiveInfo& Archive :: GetArchiveRegister(const std::string& classname)
  {
    if(type_register == nullptr) type_register =
                                   std::make_unique<std::map<std::string, detail::ClassArchiveInfo>>();
    return (*type_register)[classname];
  }
  void Archive :: SetArchiveRegister(const std::string& classname, const detail::ClassArchiveInfo& info)
  {
    if(type_register == nullptr) type_register =
                                   std::make_unique<std::map<std::string, detail::ClassArchiveInfo>>();
    (*type_register)[classname] = info;
  }
  bool Archive :: IsRegistered(const std::string& classname)
  {
    if(type_register == nullptr) type_register =
                                   std::make_unique<std::map<std::string, detail::ClassArchiveInfo>>();
    return type_register->count(classname) != 0;
  }
} // namespace ngcore
