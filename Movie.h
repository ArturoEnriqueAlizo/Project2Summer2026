#ifndef MOVIE_H
#define MOVIE_H

#include <string>
using namespace std;

struct Movie {
    int movieId;
    string title;
    string genres;
    double averageRating;
    int daysUntilExpiration;
    double recommendationScore;
};

#endif