# Soundex

A lightweight, C-based terminal fuzzy finder for searching files by name without requiring exact matches.

Soundex recursively searches file paths from the current working directory and interactively ranks matching results as the user types.

## Features

- **Recursive File Search** – Searches through directories and their nested subdirectories.
- **Fuzzy Matching** – Finds files using partial or approximate filename matches.
- **Live Search** – Updates and ranks matching results as the user types.
- **Interactive Terminal UI** – Provides a terminal-based interface built with ncurses.
- **Result Ranking** – Uses a fuzzy-matching scoring algorithm to prioritize relevant file paths.

## Technologies

- C
- ncurses
- Make

## Requirements

- Linux
- GCC or another compatible C compiler
- ncurses development library
- Make

## Installation and Usage

Clone the repository:

```bash
git clone https://github.com/emmanrn/soundex.git
cd soundex
```

Compile and install:

```bash
make install
```

Run from any directory:

```bash
fzn
```

## Current Limitations

- Only works in **Linux**
- Currently best suited for smaller codebases.
- Performance and optimization for large codebases are still areas for improvement.

## Future Improvements

- Fix name
- Make it available for other desktop platforms
- Optimize search performance for larger codebases.
- Improve file indexing and search efficiency.
- Further refine fuzzy-matching and result ranking.

For now only works in small codebases