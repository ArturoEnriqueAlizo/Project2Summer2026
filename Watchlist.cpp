#include "Watchlist.h"
using namespace std;

bool isInWatchlist(const vector<Movie>& watchlist, int movieId)
{
    for (const Movie& movie : watchlist)
    {
        if (movie.movieId == movieId)
        {
            return true;
        }
    }

    return false;
}

bool addToWatchlist(vector<Movie>& watchlist, const Movie& movie)
{
    if (isInWatchlist(watchlist, movie.movieId))
    {
        return false;
    }

    watchlist.push_back(movie);
    return true;
}

bool removeFromWatchlist(vector<Movie>& watchlist, const string& title)
{
    for (int i = 0; i < watchlist.size(); i++)
    {
        if (watchlist[i].title == title)
        {
            watchlist.erase(watchlist.begin() + i);
            return true;
        }
    }

    return false;
}
