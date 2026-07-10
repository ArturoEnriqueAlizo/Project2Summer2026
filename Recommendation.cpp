#include "Recommendation.h"
#include <iostream>
#include <cctype>
using namespace std;

bool genreMatches(Movie movie, vector<string> favoriteGenres)
{
    string genres = movie.genres;

    for (int i = 0; i < genres.size(); i++)
    {
        genres[i] = tolower(genres[i]);
    }

    for (int i = 0; i < favoriteGenres.size(); i++)
    {
        string genre = favoriteGenres[i];

        for (int j = 0; j < genre.size(); j++)
        {
            genre[j] = tolower(genre[j]);
        }

        if (genres.find(genre) != -1)
        {
            return true;
        }
    }

    return false;
}

int getExpirationBonus(Movie movie)
{
    if (movie.daysUntilExpiration <= 7)
    {
        return 30;
    }

    if (movie.daysUntilExpiration <= 14)
    {
        return 20;
    }

    if (movie.daysUntilExpiration <= 30)
    {
        return 10;
    }

    return 0;
}

double calculateScore(Movie movie, vector<string> favoriteGenres)
{
    double score = 0;

    score = movie.averageRating * 20;

    if (genreMatches(movie, favoriteGenres))
    {
        score = score + 25;
    }

    score = score + getExpirationBonus(movie);

    return score;
}

vector<Movie> getRecommendations(vector<Movie> movies, vector<string> favoriteGenres)
{
    vector<Movie> recommendations;

    for (int i = 0; i < movies.size(); i++)
    {
        movies[i].recommendationScore = calculateScore(movies[i], favoriteGenres);

        if (genreMatches(movies[i], favoriteGenres))
        {
            recommendations.push_back(movies[i]);
        }
    }

    return recommendations;
}

vector<Movie> getLeavingSoonMovies(vector<Movie> movies)
{
    vector<Movie> leavingSoon;

    for (int i = 0; i < movies.size(); i++)
    {
        if (movies[i].daysUntilExpiration <= 7)
        {
            leavingSoon.push_back(movies[i]);
        }
    }
    // chose to have the leaving soon function mergesort the results by reccomendation score, this way
    // highest reccomended films that are leaving soon appear to the user first.
    vector<Movie> leaveing = mergeSort(leavingSoon);

    return leaveing;
}

void heapify(vector<Movie>& movies, int size, int index)
{
    int largest = index;

    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < size)
    {
        if (movies[left].recommendationScore > movies[largest].recommendationScore)
        {
            largest = left;
        }
    }

    if (right < size)
    {
        if (movies[right].recommendationScore > movies[largest].recommendationScore)
        {
            largest = right;
        }
    }

    if (largest != index)
    {
        Movie temp = movies[index];
        movies[index] = movies[largest];
        movies[largest] = temp;

        heapify(movies, size, largest);
    }
}

void heapSort(vector<Movie>& movies)
{
    int size = movies.size();

    // Build the heap
    for (int i = size / 2 - 1; i >= 0; i--)
    {
        heapify(movies, size, i);
    }

    // Sort the heap
    for (int i = size - 1; i > 0; i--)
    {
        Movie temp = movies[0];
        movies[0] = movies[i];
        movies[i] = temp;

        heapify(movies, i, 0);
    }
}

// Function to reunite and properly sort the films. Also, obligatory ghostbusters reference.
vector<Movie> merginmakesmefeelgood(vector<Movie> movieleft, vector<Movie> movieright) {
    vector<Movie> sortedfilms;
    int counter = 0;
    int secondcounter = 0;
    while (movieleft.size() >counter && movieright.size() >secondcounter) {
        if (movieleft[counter].recommendationScore > movieright[secondcounter].recommendationScore) {
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
vector<Movie> mergeSort(vector<Movie>& movies) {
    if (movies.size() <= 1) {
        return movies;
    }
    // This should split the vector properly.
    int stuckinthemiddle = movies.size() / 2;
    vector<Movie> lefty(movies.begin(), movies.begin() + stuckinthemiddle);
    vector<Movie> righty(movies.begin() + stuckinthemiddle, movies.end());

    vector<Movie> leftsplit = mergeSort(lefty);
    vector<Movie> rightsplit = mergeSort(righty);

    // See merginmakesmefeelgood for more info.
    vector<Movie> finalsort = merginmakesmefeelgood(leftsplit, rightsplit);
    return finalsort;

}



Movie* searchinForAFilm(vector<Movie>& movies, string searchTitle) {
    if (movies.empty()) {
        return nullptr;
    }
    if (movies[0].title == searchTitle) {
        return &movies[0];
    }
    int merginsearch = movies.size()/2;
    vector <Movie> filmleft(movies.begin(), movies.begin() + merginsearch);
    vector <Movie> filmright(movies.begin() + merginsearch, movies.end());

    Movie* leftsearch = searchinForAFilm(filmleft, searchTitle);
    if (leftsearch!=nullptr) {
        return leftsearch;
    }
    Movie* rightsearch = searchinForAFilm(filmright, searchTitle);
    if (rightsearch!=nullptr) {
        return rightsearch;
    }
}