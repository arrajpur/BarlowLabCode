#pragma once
#ifdef __MACH__
#define __UNIX__
#endif

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <map>
#include <list>
#include <string>
#include <vector>
#include <set>
using namespace std;

#include "sfl.h"

#ifdef WIN32
#define DIR_SEP '\\'
#define NEWLINE_LEN 1
#else
#define DIR_SEP '/'
#define NEWLINE_LEN 1
#endif
