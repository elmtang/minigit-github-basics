# MiniGit Storage Format

This document describes the layout of `.minigit/`, what each file and folder stores, and which commands create or modify each item.

## 1. `.minigit/` Directory & File Layout

When `minigit init` runs in a fresh directory, MiniGit creates a `.minigit/` directory inside the project root (`<project-root>/.minigit`):

```text
<project-root>/
├── .minigit/
│   ├── HEAD
│   ├── index.txt
│   ├── stage/
│   └── commits/
└── <project files...>
```

## 2. Contents & Command Responsibilities

| Path inside `.minigit/` | What It Holds | Created / Changed By |
| :--- | :--- | :--- |
| `.minigit/` | Metadata root directory for the repository (`metadataRoot()`). Checked by `isInitialized()` to verify whether the repository exists. | Created by `init`; skipped by `WorkingTree` when scanning project files. |
| `HEAD` | Stores the reference/ID of the latest commit. Initialized as an empty (0-byte) file before the first commit. | Created as an empty file by `init`; updated after a commit completes (`commit` / `HeadRef`). |
| `index.txt` | Lists tracked/staged file paths (one relative path per line). Initialized as an empty (0-byte) file with 0 tracked paths. | Created as an empty file by `init`; updated when staging (`add`) or committing (`commit` / `IndexTable`). |
| `stage/` | Staging area holding copies of staged files prior to committing. | Created empty by `init`; populated by `add` and cleared/updated during `commit`. |
| `commits/` | Stores saved commit snapshots and metadata. | Created empty by `init`; written to by `commit` and read by `log` / `status`. |

## 3. Re-initializing an Existing Repository

If `minigit init` is run when `.minigit/` already exists:

- **Valid repository already exists** (`.minigit/`, `HEAD`, and `index.txt` are all present): `MiniGitRepository::init()` throws a `std::runtime_error` (reported as an error by the CLI) and leaves all existing files and directories completely unchanged.
- **Damaged or partial `.minigit/`** (e.g., `.minigit/` exists without `HEAD` or `index.txt`): `isInitialized()` evaluates to `false` without throwing, and running `init` reports an error without overwriting existing repository data.
