#ifdef _WIN32

#include <windows.h>
#include <sstream>
#include <string>
#include <vector>
#include "DataLoader.h"
#include "Performance.h"
#include "Recommendation.h"
#include "Watchlist.h"
using namespace std;

namespace
{
    const COLORREF WALNUT = RGB(63, 38, 28);
    const COLORREF CHARCOAL = RGB(23, 24, 21);
    const COLORREF CREAM = RGB(232, 216, 176);
    const COLORREF ORANGE = RGB(196, 81, 45);
    const COLORREF GREEN = RGB(111, 125, 60);
    const COLORREF GOLD = RGB(208, 154, 53);

    enum ControlId
    {
        ID_RECOMMEND = 101,
        ID_SEARCH,
        ID_WATCHLIST,
        ID_PERFORMANCE,
        ID_INPUT,
        ID_ACTION,
        ID_RESULTS,
        ID_DETAILS,
        ID_ADD,
        ID_REMOVE,
        ID_STATUS
    };

    enum class ViewMode
    {
        Recommend,
        Search,
        Watchlist,
        Performance
    };

    // gui state stays separate while reusing the projects shared backend modules
    vector<Movie> movies;
    vector<Movie> displayedMovies;
    vector<Movie> watchlist;
    ViewMode currentMode = ViewMode::Recommend;

    HWND inputBox;
    HWND actionButton;
    HWND resultsList;
    HWND detailsLabel;
    HWND addButton;
    HWND removeButton;
    HWND statusLabel;
    HFONT headingFont;
    HFONT bodyFont;
    HFONT dataFont;
    HBRUSH walnutBrush;
    HBRUSH charcoalBrush;

    wstring toWide(const string& text)
    {
        if (text.empty())
        {
            return L"";
        }

        int size = MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, nullptr, 0);
        wstring result(size, L'\0');
        MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, result.data(), size);
        result.resize(size - 1);
        return result;
    }

    string toNarrow(const wstring& text)
    {
        if (text.empty())
        {
            return "";
        }

        int size = WideCharToMultiByte(CP_UTF8, 0, text.c_str(), -1,
                                       nullptr, 0, nullptr, nullptr);
        string result(size, '\0');
        WideCharToMultiByte(CP_UTF8, 0, text.c_str(), -1,
                            result.data(), size, nullptr, nullptr);
        result.resize(size - 1);
        return result;
    }

    string getInputText()
    {
        int length = GetWindowTextLengthW(inputBox);
        wstring text(length + 1, L'\0');
        GetWindowTextW(inputBox, text.data(), length + 1);
        text.resize(length);
        return toNarrow(text);
    }

    void setStatus(const wstring& text)
    {
        SetWindowTextW(statusLabel, text.c_str());
    }

    void clearResults()
    {
        displayedMovies.clear();
        SendMessageW(resultsList, LB_RESETCONTENT, 0, 0);
        SetWindowTextW(detailsLabel, L"Select a movie to inspect its details.");
    }

    void addMovieRow(const Movie& movie)
    {
        wstringstream row;
        row << toWide(movie.title) << L"   |   " << toWide(movie.genres);
        SendMessageW(resultsList, LB_ADDSTRING, 0,
                     reinterpret_cast<LPARAM>(row.str().c_str()));
    }

    void showMovies(const vector<Movie>& results)
    {
        clearResults();
        displayedMovies = results;

        for (const Movie& movie : displayedMovies)
        {
            addMovieRow(movie);
        }

        if (displayedMovies.empty())
        {
            SendMessageW(resultsList, LB_ADDSTRING, 0,
                         reinterpret_cast<LPARAM>(L"No movies to display."));
        }
    }

    vector<string> parseGenres(const string& text)
    {
        vector<string> genres;
        stringstream stream(text);
        string genre;

        while (getline(stream, genre, ' '))
        {
            if (!genre.empty())
            {
                genres.push_back(genre);
            }
        }

        return genres;
    }

    Movie findMovie(const string& title)
    {
        for (const Movie& movie : movies)
        {
            if (movie.title == title)
            {
                return movie;
            }
        }

        Movie missing;
        missing.title = "FAIL";
        return missing;
    }

    void showRecommendationView()
    {
        currentMode = ViewMode::Recommend;
        SetWindowTextW(inputBox, L"");
        SetWindowTextW(actionButton, L"GENERATE");
        ShowWindow(inputBox, SW_SHOW);
        ShowWindow(actionButton, SW_SHOW);
        ShowWindow(addButton, SW_SHOW);
        ShowWindow(removeButton, SW_HIDE);
        clearResults();
        setStatus(L"RECOMMENDATION MODE  |  ENTER GENRES SEPARATED BY SPACES");
    }

    void showSearchView()
    {
        currentMode = ViewMode::Search;
        SetWindowTextW(inputBox, L"");
        SetWindowTextW(actionButton, L"SEARCH");
        ShowWindow(inputBox, SW_SHOW);
        ShowWindow(actionButton, SW_SHOW);
        ShowWindow(addButton, SW_SHOW);
        ShowWindow(removeButton, SW_HIDE);
        clearResults();
        setStatus(L"SEARCH MODE  |  USE THE EXACT TITLE AND YEAR");
    }

    void showWatchlistView()
    {
        currentMode = ViewMode::Watchlist;
        ShowWindow(inputBox, SW_HIDE);
        ShowWindow(actionButton, SW_HIDE);
        ShowWindow(addButton, SW_HIDE);
        ShowWindow(removeButton, SW_SHOW);
        showMovies(watchlist);
        setStatus(L"WATCHLIST MODE  |  SAVED FOR THIS SESSION");
    }

    void runRecommendation()
    {
        vector<string> genres = parseGenres(getInputText());
        if (genres.empty())
        {
            MessageBoxW(nullptr, L"Enter at least one genre.", L"Memento", MB_OK);
            return;
        }

        vector<Movie> recommendations = getRecommendations(movies, genres);
        heapSort(recommendations);

        vector<Movie> topMovies;
        for (int i = static_cast<int>(recommendations.size()) - 1;
             i >= 0 && topMovies.size() < 20; i--)
        {
            topMovies.push_back(recommendations[i]);
        }

        showMovies(topMovies);
        setStatus(L"RECOMMENDATIONS READY  |  SELECT A MOVIE FOR DETAILS");
    }

    void runSearch()
    {
        Movie result = findMovie(getInputText());
        if (result.title == "FAIL")
        {
            showMovies({});
            setStatus(L"SEARCH COMPLETE  |  MOVIE NOT FOUND");
            return;
        }

        showMovies({result});
        setStatus(L"SEARCH COMPLETE  |  1 MOVIE FOUND");
    }

    void showPerformanceView()
    {
        currentMode = ViewMode::Performance;
        ShowWindow(inputBox, SW_HIDE);
        ShowWindow(actionButton, SW_HIDE);
        ShowWindow(addButton, SW_HIDE);
        ShowWindow(removeButton, SW_HIDE);
        clearResults();
        setStatus(L"PERFORMANCE MODE  |  RUNNING 10 TRIALS PER SIZE...");
        UpdateWindow(GetParent(statusLabel));
        SetCursor(LoadCursor(nullptr, IDC_WAIT));

        vector<Movie> scoredMovies = movies;
        for (Movie& movie : scoredMovies)
        {
            movie.recommendationScore = movie.averageRating * 20
                                        + getExpirationBonus(movie);
        }

        const vector<int> sizes = {100, 500, 1000, 2500, 5000,
                                   static_cast<int>(scoredMovies.size())};
        SendMessageW(resultsList, LB_ADDSTRING, 0,
                     reinterpret_cast<LPARAM>(L"MOVIES      HEAP (us)      MERGE (us)      MATCH"));

        for (int size : sizes)
        {
            if (size > scoredMovies.size())
            {
                continue;
            }

            vector<Movie> sample(scoredMovies.begin(), scoredMovies.begin() + size);
            PerformanceResult result = compareSorts(sample, 10);
            wstringstream row;
            row.setf(ios::fixed);
            row.precision(2);
            row << size << L"          " << result.heapAverageMicroseconds
                << L"          " << result.mergeAverageMicroseconds
                << L"          " << (result.outputsMatch ? L"PASS" : L"FAIL");
            SendMessageW(resultsList, LB_ADDSTRING, 0,
                         reinterpret_cast<LPARAM>(row.str().c_str()));
        }

        SetCursor(LoadCursor(nullptr, IDC_ARROW));
        SetWindowTextW(detailsLabel,
                       L"Both algorithms use identical input copies. Lower times are faster.");
        setStatus(L"PERFORMANCE COMPLETE  |  ALL RESULTS VERIFIED");
    }

    void showSelectedDetails()
    {
        int index = static_cast<int>(SendMessageW(resultsList, LB_GETCURSEL, 0, 0));
        if (index == LB_ERR || index >= displayedMovies.size())
        {
            return;
        }

        const Movie& movie = displayedMovies[index];
        wstringstream details;
        details.setf(ios::fixed);
        details.precision(2);
        details << toWide(movie.title) << L"\r\n\r\n"
                << L"GENRES  " << toWide(movie.genres) << L"\r\n"
                << L"RATING  " << movie.averageRating << L"\r\n"
                << L"EXPIRES  " << movie.daysUntilExpiration << L" DAYS\r\n"
                << L"SCORE  " << movie.recommendationScore;
        SetWindowTextW(detailsLabel, details.str().c_str());
    }

    void addSelectedMovie()
    {
        int index = static_cast<int>(SendMessageW(resultsList, LB_GETCURSEL, 0, 0));
        if (index == LB_ERR || index >= displayedMovies.size())
        {
            MessageBoxW(nullptr, L"Select a movie first.", L"Memento", MB_OK);
            return;
        }

        const Movie& movie = displayedMovies[index];
        if (addToWatchlist(watchlist, movie))
        {
            setStatus(L"MOVIE ADDED TO WATCHLIST");
        }
        else
        {
            setStatus(L"MOVIE IS ALREADY IN WATCHLIST");
        }
    }

    void removeSelectedMovie()
    {
        int index = static_cast<int>(SendMessageW(resultsList, LB_GETCURSEL, 0, 0));
        if (index == LB_ERR || index >= displayedMovies.size())
        {
            return;
        }

        removeFromWatchlist(watchlist, displayedMovies[index].title);
        showWatchlistView();
        setStatus(L"MOVIE REMOVED FROM WATCHLIST");
    }

    HWND makeButton(HWND parent, const wchar_t* text, int id, int x, int y,
                    int width, int height)
    {
        HWND button = CreateWindowW(L"BUTTON", text,
                                    WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
                                    x, y, width, height, parent,
                                    reinterpret_cast<HMENU>(static_cast<INT_PTR>(id)),
                                    nullptr, nullptr);
        SendMessageW(button, WM_SETFONT, reinterpret_cast<WPARAM>(bodyFont), TRUE);
        return button;
    }

    void createControls(HWND window)
    {
        HWND title = CreateWindowW(L"STATIC", L"MEMENTO", WS_CHILD | WS_VISIBLE,
                                   36, 24, 280, 45, window, nullptr, nullptr, nullptr);
        SendMessageW(title, WM_SETFONT, reinterpret_cast<WPARAM>(headingFont), TRUE);

        HWND subtitle = CreateWindowW(L"STATIC", L"STREAMING MEMORY SYSTEM // 2026",
                                      WS_CHILD | WS_VISIBLE,
                                      38, 67, 330, 24, window, nullptr, nullptr, nullptr);
        SendMessageW(subtitle, WM_SETFONT, reinterpret_cast<WPARAM>(dataFont), TRUE);

        makeButton(window, L"RECOMMEND", ID_RECOMMEND, 35, 125, 190, 48);
        makeButton(window, L"SEARCH", ID_SEARCH, 35, 185, 190, 48);
        makeButton(window, L"WATCHLIST", ID_WATCHLIST, 35, 245, 190, 48);
        makeButton(window, L"PERFORMANCE", ID_PERFORMANCE, 35, 305, 190, 48);

        inputBox = CreateWindowExW(0, L"EDIT", L"",
                                   WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
                                   265, 112, 590, 38, window,
                                   reinterpret_cast<HMENU>(ID_INPUT), nullptr, nullptr);
        SendMessageW(inputBox, WM_SETFONT, reinterpret_cast<WPARAM>(bodyFont), TRUE);

        actionButton = makeButton(window, L"GENERATE", ID_ACTION, 870, 112, 150, 38);

        resultsList = CreateWindowExW(0, L"LISTBOX", L"",
                                      WS_CHILD | WS_VISIBLE | WS_BORDER |
                                      WS_VSCROLL | LBS_NOTIFY,
                                      265, 168, 755, 330, window,
                                      reinterpret_cast<HMENU>(ID_RESULTS), nullptr, nullptr);
        SendMessageW(resultsList, WM_SETFONT, reinterpret_cast<WPARAM>(dataFont), TRUE);

        detailsLabel = CreateWindowW(L"STATIC",
                                     L"Select a movie to inspect its details.",
                                     WS_CHILD | WS_VISIBLE | SS_LEFT,
                                     265, 520, 570, 145, window,
                                     reinterpret_cast<HMENU>(ID_DETAILS), nullptr, nullptr);
        SendMessageW(detailsLabel, WM_SETFONT, reinterpret_cast<WPARAM>(dataFont), TRUE);

        addButton = makeButton(window, L"ADD TO WATCHLIST", ID_ADD, 850, 530, 170, 45);
        removeButton = makeButton(window, L"REMOVE", ID_REMOVE, 850, 585, 170, 45);
        ShowWindow(removeButton, SW_HIDE);

        statusLabel = CreateWindowW(L"STATIC",
                                    L"SYSTEM READY  |  ENTER GENRES SEPARATED BY SPACES",
                                    WS_CHILD | WS_VISIBLE,
                                    35, 688, 985, 27, window,
                                    reinterpret_cast<HMENU>(ID_STATUS), nullptr, nullptr);
        SendMessageW(statusLabel, WM_SETFONT, reinterpret_cast<WPARAM>(dataFont), TRUE);
    }

    void drawButton(const DRAWITEMSTRUCT* item)
    {
        bool pressed = (item->itemState & ODS_SELECTED) != 0;
        COLORREF fill = pressed ? GREEN : CHARCOAL;
        HBRUSH brush = CreateSolidBrush(fill);
        FillRect(item->hDC, &item->rcItem, brush);
        DeleteObject(brush);

        HPEN pen = CreatePen(PS_SOLID, 2, pressed ? CREAM : GOLD);
        HPEN oldPen = static_cast<HPEN>(SelectObject(item->hDC, pen));
        HBRUSH oldBrush = static_cast<HBRUSH>(SelectObject(item->hDC,
                                                           GetStockObject(NULL_BRUSH)));
        Rectangle(item->hDC, item->rcItem.left, item->rcItem.top,
                  item->rcItem.right, item->rcItem.bottom);
        SelectObject(item->hDC, oldPen);
        SelectObject(item->hDC, oldBrush);
        DeleteObject(pen);

        wchar_t text[128];
        GetWindowTextW(item->hwndItem, text, 128);
        SetBkMode(item->hDC, TRANSPARENT);
        SetTextColor(item->hDC, pressed ? CREAM : GOLD);
        SelectObject(item->hDC, bodyFont);
        DrawTextW(item->hDC, text, -1, const_cast<RECT*>(&item->rcItem),
                  DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }

    LRESULT CALLBACK windowProcedure(HWND window, UINT message,
                                     WPARAM wParam, LPARAM lParam)
    {
        switch (message)
        {
            case WM_CREATE:
                createControls(window);
                showRecommendationView();
                return 0;

            case WM_COMMAND:
            {
                int id = LOWORD(wParam);
                if (id == ID_RECOMMEND) showRecommendationView();
                else if (id == ID_SEARCH) showSearchView();
                else if (id == ID_WATCHLIST) showWatchlistView();
                else if (id == ID_PERFORMANCE) showPerformanceView();
                else if (id == ID_ACTION && currentMode == ViewMode::Recommend)
                    runRecommendation();
                else if (id == ID_ACTION && currentMode == ViewMode::Search)
                    runSearch();
                else if (id == ID_RESULTS && HIWORD(wParam) == LBN_SELCHANGE)
                    showSelectedDetails();
                else if (id == ID_ADD) addSelectedMovie();
                else if (id == ID_REMOVE) removeSelectedMovie();
                return 0;
            }

            case WM_DRAWITEM:
                drawButton(reinterpret_cast<DRAWITEMSTRUCT*>(lParam));
                return TRUE;

            case WM_CTLCOLORSTATIC:
            {
                HDC dc = reinterpret_cast<HDC>(wParam);
                HWND control = reinterpret_cast<HWND>(lParam);
                SetBkMode(dc, TRANSPARENT);
                SetTextColor(dc, control == statusLabel ? GREEN : CREAM);
                if (control == detailsLabel)
                {
                    SetBkMode(dc, OPAQUE);
                    SetBkColor(dc, CHARCOAL);
                    return reinterpret_cast<LRESULT>(charcoalBrush);
                }
                return reinterpret_cast<LRESULT>(walnutBrush);
            }

            case WM_CTLCOLOREDIT:
            case WM_CTLCOLORLISTBOX:
            {
                HDC dc = reinterpret_cast<HDC>(wParam);
                SetBkColor(dc, CHARCOAL);
                SetTextColor(dc, CREAM);
                return reinterpret_cast<LRESULT>(charcoalBrush);
            }

            case WM_PAINT:
            {
                PAINTSTRUCT paint;
                HDC dc = BeginPaint(window, &paint);
                RECT content = {248, 96, 1038, 675};
                FillRect(dc, &content, charcoalBrush);
                HPEN pen = CreatePen(PS_SOLID, 3, ORANGE);
                HPEN oldPen = static_cast<HPEN>(SelectObject(dc, pen));
                HBRUSH oldBrush = static_cast<HBRUSH>(SelectObject(dc,
                                                                   GetStockObject(NULL_BRUSH)));
                Rectangle(dc, content.left, content.top, content.right, content.bottom);
                SelectObject(dc, oldPen);
                SelectObject(dc, oldBrush);
                DeleteObject(pen);
                EndPaint(window, &paint);
                return 0;
            }

            case WM_DESTROY:
                PostQuitMessage(0);
                return 0;
        }

        return DefWindowProcW(window, message, wParam, lParam);
    }
}

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE, PWSTR, int showCommand)
{
    SetProcessDPIAware();
    walnutBrush = CreateSolidBrush(WALNUT);
    charcoalBrush = CreateSolidBrush(CHARCOAL);
    headingFont = CreateFontW(38, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                              DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                              CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
                              DEFAULT_PITCH, L"Bahnschrift SemiCondensed");
    bodyFont = CreateFontW(20, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE,
                           DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                           CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
                           DEFAULT_PITCH, L"Bahnschrift SemiCondensed");
    dataFont = CreateFontW(17, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                           DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                           CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
                           FIXED_PITCH, L"Consolas");

    movies = loadMovies("Data/movies.csv", "Data/ratings.csv");
    if (movies.empty())
    {
        movies = loadMovies("../Data/movies.csv", "../Data/ratings.csv");
    }

    const wchar_t className[] = L"MementoRetroWindow";
    WNDCLASSW windowClass = {};
    windowClass.lpfnWndProc = windowProcedure;
    windowClass.hInstance = instance;
    windowClass.lpszClassName = className;
    windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    windowClass.hbrBackground = walnutBrush;
    RegisterClassW(&windowClass);

    HWND window = CreateWindowExW(0, className,
                                  L"Memento // Streaming Memory System",
                                  WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU |
                                  WS_MINIMIZEBOX,
                                  CW_USEDEFAULT, CW_USEDEFAULT, 1080, 780,
                                  nullptr, nullptr, instance, nullptr);

    if (!window)
    {
        return 0;
    }

    ShowWindow(window, showCommand);
    UpdateWindow(window);

    MSG message;
    while (GetMessageW(&message, nullptr, 0, 0))
    {
        TranslateMessage(&message);
        DispatchMessageW(&message);
    }

    DeleteObject(headingFont);
    DeleteObject(bodyFont);
    DeleteObject(dataFont);
    DeleteObject(walnutBrush);
    DeleteObject(charcoalBrush);
    return static_cast<int>(message.wParam);
}

#endif
