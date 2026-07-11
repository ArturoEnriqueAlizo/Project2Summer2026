#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "Movie.h"
#include "DataLoader.h"
#include "Recommendation.h"
#include "Performance.h"
#include "Watchlist.h"
using namespace std;

vector<Movie> merginmakesmefeelgoods(vector<Movie> movieleft, vector<Movie> movieright) {

    vector<Movie> sortedfilms;

    int counter = 0;

    int secondcounter = 0;

    while (movieleft.size() >counter && movieright.size() >secondcounter) {

        if (movieleft[counter].averageRating > movieright[secondcounter].averageRating) {

            sortedfilms.push_back(movieleft[counter]);
            counter++;

        }
        else {

            sortedfilms.push_back(movieright[secondcounter]);
            secondcounter++;

        }
    }

    vector<Movie> leftscraps(movieleft.begin()+counter, movieleft.end());
    vector<Movie> rightscraps(movieright.begin()+secondcounter, movieright.end());

    for (auto& scrap:leftscraps) {
        sortedfilms.push_back(scrap);
    }

    for (auto& scrappy:rightscraps) {
        sortedfilms.push_back(scrappy);
    }

    return sortedfilms;
}
vector<Movie> mergeSortByRatingNotReccomendation(vector<Movie>& movies) {

    if (movies.size() <= 1) {
        return movies;
    }

    int stuckinthemiddle = movies.size() / 2;


    vector<Movie> lefty(movies.begin(), movies.begin() + stuckinthemiddle);

    vector<Movie> righty(movies.begin() + stuckinthemiddle, movies.end());


    vector<Movie> leftsplit = mergeSort(lefty);

    vector<Movie> rightsplit = mergeSort(righty);

    // See merginmakesmefeelgood for more info.
    vector<Movie> finalsort = merginmakesmefeelgoods(leftsplit, rightsplit);
    return finalsort;

}

// search the unsorted dataset directly without copying temporary vectors
Movie searchForMovie(const vector<Movie>& movies, const string& searchTitle) {

    for (const Movie& movie : movies) {
        if (movie.title == searchTitle) {
            return movie;
        }
    }

    Movie temp;
    temp.title = "FAIL";
    return temp;
}

vector<Movie> searchByGenre(const vector<Movie>& movies, string searchGenre) {

    vector<Movie> genres;
    int counter = 0;

    for (int j = 0; j < searchGenre.size(); j++)
    {
        searchGenre[j] = tolower(searchGenre[j]);
    }

    for (const Movie& movie : movies) {

        string genre = movie.genres;

        for (int i = 0; i < genre.size(); i++)
        {

            genre[i] = tolower(genre[i]);

        }
        if (genre.find(searchGenre) != -1)
        {

            genres.push_back(movie);
            counter++;

        }
        if (counter==10) {
            return genres;
        }

    }
    return genres;
}
vector<Movie> searchByRating(const vector<Movie>& movies, double searchRating) {

    double lowend = searchRating-1;

    int counter = 0;

    vector<Movie> ratings;

    for (const Movie& movie : movies) {

        if (searchRating>= movie.averageRating && movie.averageRating> lowend) {

            ratings.push_back(movie);
            counter++;
        }
        if (counter==10) {

            return ratings;
        }
    }

    return ratings;
}