#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "Movie.h"
#include "DataLoader.h"
#include "Recommendation.h"


Movie searchinForAFilm(vector<Movie>& movies, string searchTitle) {
    if (movies.empty()) {
        Movie temp;
        temp.title = "FAIL";
        return temp;
    }

    if (movies[0].title == searchTitle) {
        return movies[0];
    }
    if (movies.size() == 1 && movies[0].title != searchTitle) {
        Movie temp;
        temp.title = "FAIL";
        return temp;
    }
    int merginsearch = movies.size()/2;
    vector <Movie> filmleft(movies.begin(), movies.begin() + merginsearch);
    vector <Movie> filmright(movies.begin() + merginsearch, movies.end());

    Movie leftsearch = searchinForAFilm(filmleft, searchTitle);
    if (leftsearch.title !="FAIL") {
        return leftsearch;
    }
    Movie rightsearch = searchinForAFilm(filmright, searchTitle);
    if (rightsearch.title !="FAIL") {
        return rightsearch;
    }
    Movie temp;
    temp.title = "FAIL";
    return temp;

}
using namespace std;

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

    vector<string> favoriteGenres;

    string genre;

    cout << "Enter your favorite genre(s) (use space to separate genres): ";
    getline(cin,genre);

    stringstream stringy(genre);
    string gene;

    while (getline(stringy,gene,' ')) {
        favoriteGenres.push_back(gene);
    }



    vector<Movie> recommendations;

    recommendations = getRecommendations(movies, favoriteGenres);
    if (recommendations.size() == 0) {
        cout << "No recommendations found! Maybe try a different genre?" << endl;
        return 0;
    }

    heapSort(recommendations);
    vector<Movie> mergedRecs = mergeSort(recommendations);

    cout << endl;
    cout << "Heapsort Implementation"<<endl;
    cout<<endl;
    cout << "Top Recommended Movies" << endl;
    cout << "----------------------" << endl;
    cout << endl;

    int count = 0;

    for (int i = recommendations.size() - 1; i >= 0; i--)
    {
        cout << recommendations[i].title << endl;
        cout << "Genres: " << recommendations[i].genres << endl;
        cout << "Rating: " << recommendations[i].averageRating << endl;
        cout << "Days Until Expiration: " << recommendations[i].daysUntilExpiration << endl;
        cout << "Recommendation Score: " << recommendations[i].recommendationScore << endl;
        cout << endl;

        count++;

        if (count == 10)
        {
            break;
        }
    }

    vector<Movie> leavingSoon;

    leavingSoon = getLeavingSoonMovies(recommendations);

    cout << endl;
    cout << "Movies Leaving Soon" << endl;
    cout << "-------------------" << endl;
    cout << endl;

    count = 0;

    for (int i = 0; i < leavingSoon.size(); i++)
    {
        cout << leavingSoon[i].title << endl;
        cout << "Days Until Expiration: " << leavingSoon[i].daysUntilExpiration << endl;
        cout << "Reccomendation Score: " << leavingSoon[i].recommendationScore << endl;
        cout << endl;

        count++;

        if (count == 10)
        {
            break;
        }
    }

    string movieTitle;
    string answer;
    while (answer != "N") {
        cout << "Search for a film? (Y/N)";
        cin >> answer;
        if (answer == "Y") {
            cin.ignore();
            cout<<"Enter a movie title to search: (Please use the format: Title (year))"<<endl;
            getline(cin,movieTitle);

            Movie todd = searchinForAFilm(movies, movieTitle);
            if (todd.title =="FAIL") {
                cout<<"Sorry! We couldn't find that movie! :("<<endl;
            }
            else {
                cout << "Genres: " << todd.genres << endl;
                cout << "Rating: " << todd.averageRating << endl;
                cout << "Days Until Expiration: " << todd.daysUntilExpiration << endl;
                cout << "Recommendation Score: " << todd.recommendationScore << endl;
                cout << endl;
            }
        }
        else {
            cout<<"Understood! Hope you liked these recommendations! :)"<<endl;
            break;
        }
    }

    return 0;
}