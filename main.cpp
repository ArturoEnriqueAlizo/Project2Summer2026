#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "Movie.h"
#include "DataLoader.h"
#include "Recommendation.h"
#include "Performance.h"
#include "Watchlist.h"
#include "variousSearches.h"
using namespace std;


// reuse the same validated genre input for recommendations and benchmarks
vector<string> getFavoriteGenres()
{
    string genre;

    do
    {
        cout << "Enter your favorite genre(s) (use space to separate genres): ";
        getline(cin, genre);

        if (genre.find_first_not_of(' ') == string::npos)
        {
            cout << "Please enter at least one genre." << endl;
            genre.clear();
        }
    }
    while (genre.empty());

    vector<string> favoriteGenres;
    stringstream genreStream(genre);
    string selectedGenre;

    while (getline(genreStream, selectedGenre, ' '))
    {
        if (!selectedGenre.empty())
        {
            favoriteGenres.push_back(selectedGenre);
        }
    }

    return favoriteGenres;
}

int main()
{
    vector<Movie> movies;

    movies = loadMovies("Data/movies.csv",
                        "Data/ratings.csv");

    if (movies.size() == 0)
    {
        cout << "No movies loaded." << endl;
        return 0;
    }

    cout << "Movies loaded successfully!" << endl;
    cout << "Total movies: " << movies.size() << endl;
    cout << endl;

    vector<Movie> watchlist;
    string menuChoice = "1";

    while (menuChoice != "7")
    {
        if (menuChoice == "1")
        {
            vector<string> favoriteGenres = getFavoriteGenres();

            vector<Movie> recommendations;

            recommendations = getRecommendations(movies, favoriteGenres);

            if (recommendations.size() == 0)
            {
                cout << "No recommendations found! Maybe try a different genre?" << endl;
            }
            else
            {
                // give both algorithms identical input so one sort does not affect the other
                vector<Movie> heapRecommendations = recommendations;
                vector<Movie> mergeRecommendations = recommendations;
                heapSort(heapRecommendations);
                mergeRecommendations = mergeSort(mergeRecommendations);

                // heap sort ranks recommendations while merge sort ranks leaving soon movies
                cout << endl;
                cout << "Top Recommended Movies" << endl;
                cout << "----------------------" << endl;
                cout << endl;

                int count = 0;

                for (int i = heapRecommendations.size() - 1; i >= 0; i--)
                {
                    cout << heapRecommendations[i].title << endl;
                    cout << "Genres: " << heapRecommendations[i].genres << endl;
                    cout << "Rating: " << heapRecommendations[i].averageRating << endl;
                    cout << "Days Until Expiration: " << heapRecommendations[i].daysUntilExpiration << endl;
                    cout << "Recommendation Score: " << heapRecommendations[i].recommendationScore << endl;
                    cout << endl;

                    count++;

                    if (count == 10)
                    {
                        break;
                    }
                }

                vector<Movie> leavingSoon;

                leavingSoon = getLeavingSoonMovies(mergeRecommendations);

                cout << endl;
                cout << "Movies Leaving Soon" << endl;
                cout << "-------------------" << endl;
                cout << endl;

                count = 0;

                for (int i = 0; i < leavingSoon.size(); i++)
                {
                    cout << leavingSoon[i].title << endl;
                    cout << "Days Until Expiration: " << leavingSoon[i].daysUntilExpiration << endl;
                    cout << "Genres: " << leavingSoon[i].genres << endl;
                    cout << "Recommendation Score: " << leavingSoon[i].recommendationScore << endl;
                    cout << endl;

                    count++;

                    if (count == 10)
                    {
                        break;
                    }
                }
            }
        }
        else if (menuChoice == "2")
        {
            string searchOption;
            cout<< "Select a search option:" << endl;
            cout<< "-----------------------"<<endl;
            cout<< "A. Search by Title"<<endl;
            cout<< "B. Search by Genre"<<endl;
            cout<< "C. Search by Year"<<endl;
            cout<< "D. Search by Rating"<<endl;
            cout<< "E. Cancel Search"<<endl;
            cout<<endl;
            cout<<"Search option: ";
            getline(cin,searchOption);
            vector<Movie> ratingsort = mergeSortByRatingNotReccomendation(movies);
            if (searchOption == "A") {
                string movieTitle;

                cout << "Enter a movie title to search." << endl;
                cout << "Please use the format: Title (year)" << endl;
                cout << "Movie title: ";

                getline(cin, movieTitle);

                Movie finder = searchForMovie(movies, movieTitle);

                if (finder.title == "FAIL")
                {
                    cout << "Sorry! We couldn't find that movie! :(" << endl;
                }
                else
                {
                    cout << endl;
                    cout << finder.title << endl;
                    cout << "Genres: " << finder.genres << endl;
                    cout << "Rating: " << finder.averageRating << endl;
                    cout << "Days Until Expiration: " << finder.daysUntilExpiration << endl;
                    cout << endl;
                }
            }
            if (searchOption == "B") {
                string searchGenre;

                cout << "Enter a single genre to search." << endl;

                cout << "Genre: ";
                getline(cin, searchGenre);


                vector<Movie> matches = searchByGenre(ratingsort, searchGenre);
                if (matches.size() == 0) {
                    cout<<"It looks like we couldn't find any matches. :("<<endl;
                }
                else {
                    cout<<"Here are the top 10 genre matches"<<endl;
                    cout<<"---------------------------------"<<endl;
                    for (int i = 0; i < matches.size(); i++) {
                        cout << endl;
                        cout << matches[i].title << endl;
                        cout << "Genres: " << matches[i].genres << endl;
                        cout << "Rating: " << matches[i].averageRating << endl;
                        cout << "Days Until Expiration: " << matches[i].daysUntilExpiration << endl;
                        cout << endl;
                    }
                }

            }
            if (searchOption == "C") {
                string searchYear;

                cout << "Enter a year to search." << endl;
                cout<< "Please use the format: (year)" << endl;

                cout << "Year: ";
                getline(cin, searchYear);



                vector<Movie> matches = searchByYear(ratingsort, searchYear);
                if (matches.size() == 0) {
                    cout<<"It looks like we couldn't find any matches. :("<<endl;
                }
                else {
                    vector<Movie> matchess = mergeSortByRatingNotReccomendation(matches);
                    cout<<"Here are the top 10 matches by audience rating"<<endl;
                    cout<<"----------------------------------------------"<<endl;
                    for (int i = 0; i < matchess.size(); i++) {
                        cout << endl;
                        cout << matchess[i].title << endl;
                        cout << "Genres: " << matchess[i].genres << endl;
                        cout << "Rating: " << matchess[i].averageRating << endl;
                        cout << "Days Until Expiration: " << matchess[i].daysUntilExpiration << endl;
                        cout << endl;
                    }
                }
            }
            if (searchOption == "D") {
                string searchRating;

                cout << "Enter a rating to search." << endl;

                cout << "Rating: ";
                getline(cin, searchRating);
                double searchRatings = stod(searchRating);


                vector<Movie> matches = searchByRating(ratingsort, searchRatings);
                if (matches.size() == 0) {
                    cout<<"It looks like we couldn't find any matches. :("<<endl;
                }
                else {
                    vector<Movie> matchess = mergeSortByRatingNotReccomendation(matches);
                    cout<<"Here are the top 10 matches by audience rating"<<endl;
                    cout<<"----------------------------------------------"<<endl;
                    for (int i = 0; i < matchess.size(); i++) {
                        cout << endl;
                        cout << matchess[i].title << endl;
                        cout << "Genres: " << matchess[i].genres << endl;
                        cout << "Rating: " << matchess[i].averageRating << endl;
                        cout << "Days Until Expiration: " << matchess[i].daysUntilExpiration << endl;
                        cout << endl;
                    }
                }
            }
            else {
                menuChoice = "8";
            }

        }
        else if (menuChoice == "3")
        {
            vector<string> favoriteGenres = getFavoriteGenres();

            // score the full dataset so the comparison can test every requested size
            vector<Movie> benchmarkMovies = movies;
            for (Movie& movie : benchmarkMovies)
            {
                movie.recommendationScore = calculateScore(movie, favoriteGenres);
            }

            runPerformanceTests(benchmarkMovies, 10);
        }
        else if (menuChoice == "4")
        {
            string movieTitle;
            cout << "Enter the exact movie title to add: ";
            getline(cin, movieTitle);

            Movie movie = searchForMovie(movies, movieTitle);
            if (movie.title == "FAIL")
            {
                cout << "Movie not found." << endl;
            }
            else if (addToWatchlist(watchlist, movie))
            {
                cout << movie.title << " was added to your watchlist." << endl;
            }
            else
            {
                cout << movie.title << " is already in your watchlist." << endl;
            }
        }
        else if (menuChoice == "5")
        {
            cout << endl << "Your Watchlist" << endl;
            cout << "--------------" << endl;

            if (watchlist.empty())
            {
                cout << "Your watchlist is empty." << endl;
            }
            else
            {
                for (const Movie& movie : watchlist)
                {
                    cout << movie.title << " | " << movie.genres
                         << " | Rating: " << movie.averageRating << endl;
                }
            }
        }
        else if (menuChoice == "6")
        {
            string movieTitle;
            cout << "Enter the exact movie title to remove: ";
            getline(cin, movieTitle);

            if (removeFromWatchlist(watchlist, movieTitle))
            {
                cout << movieTitle << " was removed from your watchlist." << endl;
            }
            else
            {
                cout << "That movie is not in your watchlist." << endl;
            }
        }
        else if (menuChoice != "7")
        {
            cout << "Please enter a number from 1 to 7." << endl;
        }

        cout << endl;
        cout << "What do you want to do?" << endl;
        cout << "1. Choose another genre" << endl;
        cout << "2. Search for a specific movie" << endl;
        cout << "3. Compare Heap Sort and Merge Sort" << endl;
        cout << "4. Add a movie to watchlist" << endl;
        cout << "5. View watchlist" << endl;
        cout << "6. Remove a movie from watchlist" << endl;
        cout << "7. Exit" << endl;
        cout << "Choice: ";

        getline(cin, menuChoice);

        cout << endl;
    }

    cout << "Understood! Hope you liked these recommendations! :)" << endl;

    return 0;
}
