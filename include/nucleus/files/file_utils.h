
#ifndef NUCLEUS_FILES_FILE_UTILS_H_
#define NUCLEUS_FILES_FILE_UTILS_H_

#include <string>
#include <vector>

namespace nu {

bool readFileToVector(const std::string& path, std::vector<char>* buffer);
bool writeVectorToFile(const std::string& path, const std::vector<char>& data);

}  // namespace nu

#endif  // NUCLEUS_FILES_FILE_UTILS_H_
