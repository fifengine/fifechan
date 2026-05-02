#include "fifechan/fontloader.hpp"

#include <algorithm>
#include <filesystem>
#include <string>
#include <vector>

#include "fifechan/exception.hpp"

#ifdef _WIN32
    #include <windows.h>
#else
    #include <limits.h>
    #include <unistd.h>
#endif

namespace fcn
{
    namespace font
    {

        namespace
        {

            std::filesystem::path getExecutablePath()
            {
#ifdef _WIN32
                wchar_t path[MAX_PATH];
                GetModuleFileNameW(nullptr, path, MAX_PATH);
                return std::filesystem::path(path);
#else
                char path[PATH_MAX];
                ssize_t len = readlink("/proc/self/exe", path, sizeof(path) - 1);
                if (len != -1) {
                    path[len] = '\0';
                    return std::filesystem::path(path);
                }
                return {};
#endif
            }

            std::vector<std::filesystem::path> getSystemFontPaths()
            {
                std::vector<std::filesystem::path> paths;
#ifdef _WIN32
                // Windows system font directories
                wchar_t const * winDir = _wgetenv(L"WINDIR");
                if (winDir) {
                    paths.emplace_back(std::filesystem::path(winDir) / L"Fonts");
                } else {
                    paths.emplace_back(L"C:\\Windows\\Fonts");
                }
#elif defined(__APPLE__)
                // macOS system font directories
                paths.emplace_back("/System/Library/Fonts");
                paths.emplace_back("/Library/Fonts");
                paths.emplace_back(std::filesystem::path(getenv("HOME")) / "Library/Fonts");
#else
                // Linux/Unix system font directories
                paths.emplace_back("/usr/share/fonts");
                paths.emplace_back("/usr/local/share/fonts");
                paths.emplace_back(std::filesystem::path(getenv("HOME")) / ".local/share/fonts");
                paths.emplace_back(std::filesystem::path(getenv("HOME")) / ".fonts");
#endif
                return paths;
            }

            std::vector<std::string> getFontExtensions()
            {
                return {".ttf", ".otf", ".TTF", ".OTF", ".woff", ".WOFF"};
            }

        } // anonymous namespace

        std::vector<std::filesystem::path> FontLoader::getDefaultSearchPaths()
        {
            std::vector<std::filesystem::path> paths;

            // Current working directory
            paths.emplace_back(std::filesystem::current_path());

            // Executable directory
            auto exePath = getExecutablePath();
            if (!exePath.empty()) {
                paths.push_back(exePath.parent_path());
            }

            // System font directories
            auto systemPaths = getSystemFontPaths();
            paths.insert(paths.end(), systemPaths.begin(), systemPaths.end());

            // Common project-relative paths (for development/testing)
            paths.emplace_back("tests/resources");
            paths.emplace_back("../tests/resources");
            paths.emplace_back("./tests/resources");

            // Remove duplicates while preserving order
            std::vector<std::filesystem::path> uniquePaths;
            for (auto const & p : paths) {
                if (std::find(uniquePaths.begin(), uniquePaths.end(), p) == uniquePaths.end()) {
                    uniquePaths.push_back(p);
                }
            }

            return uniquePaths;
        }

        std::filesystem::path FontLoader::findFontFile(
            std::string const & fontName, std::vector<std::filesystem::path> const & searchPaths)
        {
            // Use provided paths or defaults
            auto paths = searchPaths.empty() ? FontLoader::getDefaultSearchPaths() : searchPaths;

            // Check if fontName already has an extension
            std::filesystem::path fontPath(fontName);
            bool hasExtension = fontPath.has_extension();

            auto extensions = getFontExtensions();

            for (auto const & dir : paths) {
                if (hasExtension) {
                    // Font name includes extension - try direct match
                    std::filesystem::path fullPath = dir / fontName;
                    if (std::filesystem::exists(fullPath)) {
                        return fullPath;
                    }
                } else {
                    // Try each extension
                    for (auto const & ext : extensions) {
                        std::filesystem::path fullPath = dir / (fontName + ext);
                        if (std::filesystem::exists(fullPath)) {
                            return fullPath;
                        }
                    }
                }
            }

            return {}; // Not found
        }

        std::shared_ptr<Font> FontLoader::loadFont(
            Graphics& graphics,
            std::string const & fontName,
            int size,
            std::vector<std::filesystem::path> const & searchPaths)
        {
            auto fontFile = FontLoader::findFontFile(fontName, searchPaths);

            if (fontFile.empty()) {
                throw fcn::Exception("loadFont: Could not find font '" + fontName + "'");
            }

            auto font = graphics.createFont(fontFile.string(), size);
            if (!font) {
                throw fcn::Exception("loadFont: Failed to load font from '" + fontFile.string() + "'");
            }

            return font;
        }

    } // namespace font
} // namespace fcn
