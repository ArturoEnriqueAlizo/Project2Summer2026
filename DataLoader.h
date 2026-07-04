#ifndef DATALOADER_H
#define DATALOADER_H
#include <vector>
#include <string>
#include "Movie.h"
using namespace std;

vector<Movie> loadMovies(string moviesFile, string ratingsFile);

#endif