#ifndef FILEIO_H
#define FILEIO_H

#include <string>

class FileIO {
public:
    // Reads the entire file content into a string
    static std::string read(const std::string& filename);

    // Writes a string to a file (overwrites existing content)
    static void write(const std::string& filename, const std::string& content);
};

#endif // FILEIO_H
