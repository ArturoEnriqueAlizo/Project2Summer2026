#ifndef PERFORMANCE_H
#define PERFORMANCE_H

#include <vector>
#include "Movie.h"
using namespace std;

struct PerformanceResult
{
    int dataSize;
    double heapAverageMicroseconds;
    double mergeAverageMicroseconds;
    bool outputsMatch;
};

PerformanceResult compareSorts(const vector<Movie>& movies, int trials);
void runPerformanceTests(const vector<Movie>& movies, int trials);
bool verifySortResults(const vector<Movie>& heapResult,
                       const vector<Movie>& mergeResult);

#endif
