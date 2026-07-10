#ifndef WATCHLIST_H
#define WATCHLIST_H

#include <string>
#include <vector>
#include "Movie.h"
using namespace std;

bool addToWatchlist(vector<Movie>& watchlist, const Movie& movie);
bool removeFromWatchlist(vector<Movie>& watchlist, const string& title);
bool isInWatchlist(const vector<Movie>& watchlist, int movieId);

#endif
