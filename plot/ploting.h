#ifndef PLOTING_H_INCLUDED
#define PLOTING_H_INCLUDED
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include<vector>
#include "svgfile.h"

// Tested on:
// 1. Visual Studio 2012 on Windows
// 2. Mingw gcc 4.7.1 on Windows
// 3. gcc 4.6.3 on GNU/Linux

// Note that gnuplot binary must be on the path
// and on Windows we need to use the piped version of gnuplot
#ifdef WIN32
    #define GNUPLOT_NAME "pgnuplot -persist"
#else
    #define GNUPLOT_NAME "gnuplot"
#endif

extern "C" FILE *popen(const char *command, const char *mode);
extern "C" void *pclose(FILE*);

void plotPareto3D(std::vector<std::vector<float>> frontier,std::vector<std::vector<float>> rest);
void printPareto3D(std::vector<std::vector<float>> frontier,std::vector<std::vector<float>> rest, Svgfile& svgout);
void printPareto2D(std::vector<std::vector<float>> frontier,std::vector<std::vector<float>> rest, Svgfile& svgout);
void plotPareto2D(std::vector<std::vector<float>> frontier,std::vector<std::vector<float>> rest);
void plotPareto3Dtest();
void plotPareto2Dtest();

#endif // PLOTING_H_INCLUDED
