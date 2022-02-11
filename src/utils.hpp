#pragma once

#include <string>

#define sptr(type_, len_) (void*)(sizeof(type_) * len_)

std::string assets_dir();
