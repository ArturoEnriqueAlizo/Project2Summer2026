# Memento

Memento is a movie recommendation and expiration alert system built in C++. It uses MovieLens ratings and genres to calculate recommendation scores, show movies that are leaving soon, search exact titles, maintain an in-memory watchlist, and compare Heap Sort with Merge Sort. The project includes both a portable console program and an optional retro-futurist Windows GUI.

## Team

- Angelina Coutsoukes (`MyDogIsCool4`)
- Arturo Alizo (`ArturoEnriqueAlizo`)
- Samuel Calle (`SamCalle`)

## Features

- Recommendations using one or more preferred genres
- Scores combining average rating, genre preference, and expiration urgency
- Heap Sort and Merge Sort implemented from scratch
- Leaving-soon results ranked by recommendation score
- Exact-title movie search
- Add, view, and remove watchlist entries during the current session
- Benchmark comparison over multiple input sizes and ten trials
- Automatic verification that both sorting algorithms return equivalent results
- Automated tests for scoring, sorting, benchmarking, and watchlist behavior
- Console interface on all supported platforms
- Native retro-futurist GUI on Windows

## Requirements

- CMake 3.16 or newer
- A compiler with C++20 support
- Windows GUI: Visual Studio Community or Build Tools with **Desktop development with C++**

The console application can also build with Apple Clang on macOS or GCC/Clang on Linux. The GUI uses the native Win32 API and is therefore available only on Windows.

## Dataset

The repository includes the two files required from the [MovieLens Latest Small dataset](https://grouplens.org/datasets/movielens/latest/):

```text
Data/movies.csv
Data/ratings.csv
```

The dataset contains 100,836 ratings across 9,742 movies. `Data/README.txt` contains the original GroupLens description, license conditions, and citation.

MovieLens does not provide streaming expiration dates. Memento generates demonstration values from 1 to 60 days so the expiration-aware features can be tested without presenting the values as live streaming-service data.

## Build on Windows

### Simplest GUI setup

For a new Windows computer:

1. Install [Visual Studio Community](https://visualstudio.microsoft.com/vs/community/).
2. In Visual Studio Installer, select **Desktop development with C++** and **CMake tools for Windows**.
3. Clone or download this repository.
4. Double-click `run_gui.bat` in the repository folder.

The launcher finds Visual Studio, configures CMake, builds `MementoGUI`, and starts it with the correct working directory. No additional GUI framework is required because Memento uses the native Win32 API.

### Manual Windows build

Open **Developer PowerShell for Visual Studio**, move to the repository root, and run:

```powershell
cmake -S . -B build
cmake --build build --config Release
```

Run the console application with one of these paths, depending on the CMake generator:

```powershell
.\build\Release\Project2Summer2026Final.exe
.\build\Project2Summer2026Final.exe
```

Run the GUI with one of these paths:

```powershell
.\build\Release\MementoGUI.exe
.\build\MementoGUI.exe
```

Run executables from the repository root so the relative `Data/` paths resolve correctly.

## Build on macOS or Linux

The portable console application can be built with:

```bash
cmake -S . -B build
cmake --build build
./build/Project2Summer2026Final
```

`MementoGUI` is intentionally omitted on non-Windows systems.

## Run Automated Tests

After building, run:

```powershell
ctest --test-dir build -C Release --output-on-failure
```

For a single-configuration generator, `-C Release` can be omitted.

## Console Menu

The console application provides seven options:

1. Get movie recommendations
2. Search for a specific movie
3. Compare Heap Sort and Merge Sort
4. Add a movie to the watchlist
5. View the watchlist
6. Remove a movie from the watchlist
7. Exit

Enter genres separated by spaces, such as `Action Comedy`. Exact-title searches must include the year in the MovieLens format, such as `Toy Story (1995)`.

## Performance Comparison

The comparison assigns scores to the full dataset and tests identical copies at sizes of 100, 500, 1,000, 2,500, 5,000, and the largest available size. Each algorithm runs ten times per size. The program reports average microseconds and prints `PASS` when Heap Sort and Merge Sort produce equivalent score ordering and contain the same movies.

Heap Sort produces ascending output and is read from the end when displaying recommendations. Merge Sort produces descending output. The verifier accounts for this difference rather than changing either algorithm solely for the benchmark.

## Project Structure

- `main.cpp`: portable console interface and feature integration
- `GuiMain.cpp`: optional Windows GUI
- `Movie.h`: shared movie structure
- `DataLoader.h/.cpp`: CSV loading, average ratings, and simulated expiration values
- `Recommendation.h/.cpp`: genre filtering, scoring, Heap Sort, and Merge Sort
- `Performance.h/.cpp`: repeated timing and output verification
- `Watchlist.h/.cpp`: shared in-memory watchlist operations
- `ProjectTests.cpp`: automated correctness tests

## Troubleshooting

### CMake is not recognized

Use **Developer PowerShell for Visual Studio**, or install the CMake component through the Visual Studio Installer under **Desktop development with C++**.

### CMake reports that version 4.0 is required

Pull the latest repository version. The project now supports CMake 3.16 and newer. If an old error remains cached, delete the local `build` directory and configure again.

### MementoGUI is not listed as a target

The GUI target exists only on Windows. On Windows, configure with a Visual Studio or NMake generator and reload the CMake project.

### The program says the MovieLens files were not found

Confirm that `Data/movies.csv` and `Data/ratings.csv` exist and launch the executable with the repository root as the working directory.

### LINK error LNK1168 cannot open MementoGUI.exe

Close the running Memento GUI before rebuilding. Windows cannot replace the executable while it is open.

### Visual Studio uses a stale configuration

Close the running program, delete the local `build` directory, and run the two build commands again.

## Data Acknowledgment

This project uses the MovieLens `ml-latest-small` dataset provided by GroupLens Research at the University of Minnesota. Dataset use and redistribution remain subject to the conditions in `Data/README.txt`.

F. Maxwell Harper and Joseph A. Konstan. 2015. *The MovieLens Datasets: History and Context.* ACM Transactions on Interactive Intelligent Systems 5, 4, Article 19. https://doi.org/10.1145/2827872
