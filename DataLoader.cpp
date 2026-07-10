#include "DataLoader.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
using namespace std;

vector<Movie> loadMovies(string moviesFile, string ratingsFile)
{
    vector<Movie> movies;
    unordered_map<int, int> movieIndexById;

    ifstream file;
    file.open(moviesFile);

    if (!file.is_open())
    {
        cout << "Could not open movies file." << endl;
        return movies;
    }

    string line;

    getline(file, line);

    while (getline(file, line))
    {
        stringstream ss(line);

        string movieIdText;
        string rest;

        getline(ss, movieIdText, ',');
        getline(ss, rest);

        Movie movie;

        movie.movieId = stoi(movieIdText);

        int lastComma = rest.find_last_of(',');

        if (lastComma == -1)
        {
            continue;
        }

        movie.title = rest.substr(0, lastComma);
        movie.genres = rest.substr(lastComma + 1);

        if (movie.title[0] == '"')
        {
            movie.title.erase(0, 1);
        }

        if (movie.title[movie.title.size() - 1] == '"')
        {
            movie.title.erase(movie.title.size() - 1, 1);
        }
        movie.averageRating = 0;
        movie.daysUntilExpiration = 0;
        movie.recommendationScore = 0;

        movies.push_back(movie);
        movieIndexById[movie.movieId] = static_cast<int>(movies.size()) - 1;
    }

    file.close();

    ifstream ratings;
    ratings.open(ratingsFile);

    if (!ratings.is_open())
    {
        cout << "Could not open ratings file." << endl;
        return movies;
    }

    vector<double> totalRatings;
    vector<int> ratingCounts;

    for (int i = 0; i < movies.size(); i++)
    {
        totalRatings.push_back(0);
        ratingCounts.push_back(0);
    }

    getline(ratings, line);

    while (getline(ratings, line))
    {
        stringstream ss(line);

        string userIdText;
        string movieIdText;
        string ratingText;
        string timestampText;

        getline(ss, userIdText, ',');
        getline(ss, movieIdText, ',');
        getline(ss, ratingText, ',');
        getline(ss, timestampText, ',');

        int movieId = stoi(movieIdText);
        double rating = stod(ratingText);

        // use the movie id map so each rating is processed in constant average time
        auto movieIndex = movieIndexById.find(movieId);
        if (movieIndex != movieIndexById.end())
        {
            int index = movieIndex->second;
            totalRatings[index] = totalRatings[index] + rating;
            ratingCounts[index] = ratingCounts[index] + 1;
        }
    }

    ratings.close();

    for (int i = 0; i < movies.size(); i++)
    {
        if (ratingCounts[i] > 0)
        {
            movies[i].averageRating = totalRatings[i] / ratingCounts[i];
        }

        // movielens has no expiration data so generate 1 to 60 days for demonstration
        movies[i].daysUntilExpiration = (i % 60) + 1;
    }

    cout << "Movies loaded: " << movies.size() << endl;
    cout << "Ratings loaded." << endl;
    cout << "Expiration days added." << endl;

    return movies;
}
