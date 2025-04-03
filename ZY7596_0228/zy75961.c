#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>

#define MAX_LENGTH 256

// Szöveg átalakítása nagybetűssé
void to_upper(char *str)
{
    while (*str)
    {
        *str = toupper((unsigned char)*str);
        str++;
    }
}

// Karakterkódolás beállítása
void set_encoding()
{
    system("chcp 65001 > nul");
    setlocale(LC_ALL, "hu_HU.UTF-8");
}

// Adatok beolvasása
void read_user_data(const char *prompt, char *buffer, size_t size, const char *data_name)
{
    do
    {
        printf("%s", prompt);
        fgets(buffer, size, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';

        // Ellenőrizzük, hogy a beírt adat végén # szerepel-e
        if (buffer[strlen(buffer) - 1] != '#')
        {
            printf("Error: Your %s must end with a '#' character. Please try again!\n", data_name);
        }
    } while (buffer[strlen(buffer) - 1] != '#');

    // Ha az adat végén # szerepel, akkor eltávolítjuk azt
    buffer[strlen(buffer) - 1] = '\0';
}

// Fájl megnyitása írásra/olvasásra
FILE *open_file(const char *filename, const char *method)
{
    FILE *file = fopen(filename, method);

    if (!file)
    {
        perror("Opening file error!");
        exit(1);
    }

    return file;
}

// Adatok mentése a fájlba
void save_data_to_file(FILE *file, const char *fullname, const char *neptun_code)
{
    fprintf(file, "%s\n", fullname);
    fprintf(file, "%s\n", neptun_code);
    fclose(file);
}

// Fájl tartalmának kiírása nagybetűsen
void print_file_text_upper(FILE *file)
{
    char line[MAX_LENGTH];
    printf("\n");
    while (fgets(line, MAX_LENGTH, file))
    {
        to_upper(line);
        printf("%s", line);
    }
    fclose(file);
}

int main()
{
    char filename[MAX_LENGTH];
    char fullname[MAX_LENGTH];
    char neptun_code[20];
    char txt_filename[MAX_LENGTH + 4];

    // Karakterkódolás beállítása
    set_encoding();

    // Adatok beolvasása
    read_user_data("File name: ", filename, sizeof(filename), "file name");
    read_user_data("Full name: ", fullname, sizeof(fullname), "full name");
    read_user_data("Neptun code: ", neptun_code, sizeof(neptun_code), "neptun code");

    // .txt kiterjesztés hozzáadása az általunk megadott fájlhoz
    snprintf(txt_filename, sizeof(txt_filename), "%s.txt", filename);

    // Fájl megnyitása és adatok mentése
    FILE *file = open_file(txt_filename, "w");
    save_data_to_file(file, fullname, neptun_code);

    // Fájl olvasása és tartalom kiírása nagybetűsen
    file = open_file(txt_filename, "r");
    print_file_text_upper(file);

    // Kilépés előtt várjunk egy Enter lenyomásra
    printf("\nPress Enter to exit...");
    getchar();

    return 0;
}