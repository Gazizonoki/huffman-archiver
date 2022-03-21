#pragma once

#include "writer.h"
#include "reader.h"

#include <iostream>
#include <vector>

void Compress(const std::string &filename, Reader &input, Writer &output, bool is_last);