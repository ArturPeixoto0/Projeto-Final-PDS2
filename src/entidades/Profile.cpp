#include "Profile.hpp"
#include <string>

// Definição da constante declarada no .hpp (necessária se ela for usada por referência).
const int Profile::MAX_DESCRIPTION_LENGTH;

Profile::~Profile() {}

std::string Profile::getDescription() const {
    return this->Description;
}

std::string Profile::getPhotoPath() const {
    return this->PhotoPath;
}

std::string Profile::getHeaderPath() const {
    return this->HeaderPath;
}

bool Profile::setDescription(const std::string& description) {
    if ((int)description.size() > MAX_DESCRIPTION_LENGTH) {
        return false;
    }
    this->Description = description;
    return true;
}

void Profile::setPhotoPath(const std::string& path) {
    this->PhotoPath = path;
}

void Profile::setHeaderPath(const std::string& path) {
    this->HeaderPath = path;
}
