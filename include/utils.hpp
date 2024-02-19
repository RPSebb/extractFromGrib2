#ifndef UTILS
#define UTILS

#include <iostream>
#include <string>
#include <filesystem>
#include <cmath>
#include <vector>

void createDirectory(std::string path);
float toMultipleOf(float value, float multiple);
void getFilesRecursive(std::string path, std::vector<std::string>& files);

#endif