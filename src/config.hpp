#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <filesystem>
#include <stdexcept>
#include <string>

namespace fs = std::filesystem;

class Config
{
public:
  static std::string get_project_root()
  {
    if (!s_project_root.empty())
      return s_project_root;

    fs::path current_dir = fs::current_path();

    while (true) {
      if (fs::exists(current_dir / "assets")) {
        s_project_root = current_dir.string();
        return s_project_root;
      }

      if (!current_dir.has_parent_path())
        break;

      current_dir = current_dir.parent_path();
    }

    throw std::runtime_error("Unable to find the root directory of the "
                             "project. The 'data' folder is missing.");
  }

private:
  inline static std::string s_project_root;
};

#endif // CONFIG_HPP