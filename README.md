*This project has been created as part of the 42 curriculum by kimdos-s.*

# get_next_line

## Description

`get_next_line` is a C function that reads a file descriptor line by line, returning one line per call — including the newline character `\n` when present. On reaching the end of file or encountering an error, it returns `NULL`.

The core challenge of the project is managing state between calls: since C functions do not retain memory between invocations, a `static` variable is used to persist unprocessed data across calls. This leftover content — called the **stash** — accumulates bytes read from the file descriptor until a newline is found or the file ends.

The bonus implementation extends the mandatory version to support **multiple file descriptors simultaneously**, allowing the function to be called on different open files without losing each file's reading context.

---

## Instructions

### Compilation

`BUFFER_SIZE` must be defined at compile time with the `-D` flag. Any positive integer is valid.

```bash
# Mandatory
gcc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c -o gnl

# Bonus
gcc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line_bonus.c get_next_line_utils_bonus.c -o gnl_bonus
```

### Usage example

```c
#include <fcntl.h>
#include <stdio.h>
#include "get_next_line.h"

int main(void)
{
    int   fd;
    char  *line;

    fd = open("file.txt", O_RDONLY);
    if (fd < 0)
        return (1);
    while ((line = get_next_line(fd)) != NULL)
    {
        printf("%s", line);
        free(line);
    }
    close(fd);
    return (0);
}
```

> The caller is responsible for freeing each line returned by `get_next_line`.

---

## Algorithm

### Mandatory — single static stash

The mandatory implementation uses a single `static char *stash` to persist unread data between calls.

**Reading loop:** while the stash contains no `\n` and `read()` returns more than 0 bytes, new data is appended to the stash via `ft_strjoin`. This version of `ft_strjoin` frees its first argument (`s1`) before returning, since the old stash is always replaced by the joined result — avoiding memory leaks without requiring the caller to manage that pointer.

**Line extraction:** once the stash contains a `\n` (or EOF is reached), `extract_line` separates the content. If a `\n` is present, `ft_substr` copies from the start of the stash up to and including the `\n` — this becomes the returned line. `ft_strdup` then copies everything after the `\n` into a new `rest` pointer, which becomes the new stash. The old stash is freed. If no `\n` exists (end of file), `ft_strdup` copies the entire stash as the return value and the stash is set to `NULL`.

This approach keeps allocation and freeing local to each function, making the memory flow traceable: every `malloc` has a corresponding `free` within the same call cycle.

---

## Resources

### Documentation

- [open(2) — Linux manual page](https://man7.org/linux/man-pages/man2/open.2.html)
- [read(2) — Linux manual page](https://man7.org/linux/man-pages/man2/read.2.html)

### Articles and guides

- [suspectedoceano - get_next_line (Notion)](https://suspectedoceano.notion.site/get_next_line-b3fa44d60dc24817bfd6a642183a93a0)
- [42 Cursus GitBook Guide - get_next_line](https://42-cursus.gitbook.io/guide/1-rank-01/get_next_line)
- [Master File I/O Operations with 42's get_next_line — A. Yigit Ogun (Medium)](https://medium.com/@ayogun/master-file-i-o-operations-with-42s-get-next-line-project-5fb001d1fff5)

### AI usage

IA was used as a study and review tool throughout the project: for understanding concepts like static variables and buffer management, reviewing edge case handling and memory flow and drafting this README. All code was written and understood by the author.