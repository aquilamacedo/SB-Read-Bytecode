## How to Run

To run the program, follow these steps:

1. Open your terminal.
2. Go to src/ folder.
3. Use the provided `Makefile` or compile the source code using `gcc`. You can compile the program with the following command:

    ```bash
    gcc main.c reader.c display.c -o output -lm
    ```

    This command compiles the source files `main.c`, `reader.c`, and `display.c` into an executable named `output` while linking the math library (`-lm`).

4. After successfully compiling, you can execute the program by running the following command:

    ```bash
    ./output <somefile.class>
    ```

    Replace `<somefile.class>` with the actual input file you want to process. Ensure that the input file is in the appropriate format and contains the required data for the program to function correctly.
