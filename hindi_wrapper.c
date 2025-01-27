#include <sqlite3.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// hindi-to-English keyword mappings (sorted by length)
const char *keywords[][2] = {
    {"चुनें", "SELECT"}, {"बनाएं", "CREATE"}, {"टेबल", "TABLE"},
    {"डालें", "INSERT"}, {"हटाएं", "DELETE"}, {"अपडेट", "UPDATE"},
    {"से", "FROM"}, {"में", "INTO"}, {"मान", "VALUES"}, 
    {"जहां", "WHERE"}, {NULL, NULL}
};

// Callback for SELECT results
static int callback(void *not_used, int argc, char **argv, char **col_names) {
    for(int i = 0; i < argc; i++) {
        printf("%s: %s\n", col_names[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

// Translate hindi-query English
char* translate_hindi_to_english(const char *hindi_query) {
    size_t len = strlen(hindi_query);
    char *translated = malloc(len * 3);
    translated[0] = '\0';
    const char *remaining = hindi_query;

    while(*remaining) {
        int matched = 0;
        for(int i = 0; keywords[i][0]; i++) {
            size_t hindi_len = strlen(keywords[i][0]);
            if(strncmp(remaining, keywords[i][0], hindi_len) == 0) {
                strcat(translated, keywords[i][1]);
                remaining += hindi_len;
                matched = 1;
                break;
            }
        }
        if(!matched) {
            strncat(translated, remaining, 1);
            remaining++;
        }
    }
    return translated;
}

// Execute query
void execute_query(sqlite3 *db, const char *query) {
    char *err_msg = 0;
    int rc = sqlite3_exec(db, query, callback, 0, &err_msg);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Error: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
}

//shell
void interactive_shell(sqlite3 *db) {
    char input[1024];
    printf("hindi SQLite3 Shell (Type '.exit' to quit)\n");
    while(1) {
        printf("hindi_sqlite3> ");
        if(!fgets(input, sizeof(input), stdin)) break;
        input[strcspn(input, "\n")] = '\0';

        if(strcmp(input, ".exit") == 0) break;

        if(strstr(input, ";") != NULL) {
            char *translated = translate_hindi_to_english(input);
            execute_query(db, translated);
            free(translated);
        }
    }
}

int main(int argc, char *argv[]) {
    sqlite3 *db;
    char *db_name = "default.db";
    
    if(argc > 1) db_name = argv[1];
    
    if(sqlite3_open(db_name, &db) != SQLITE_OK) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    if(argc == 2) {
        interactive_shell(db);
    } 
    else if(argc == 4 && strcmp(argv[2], "-cmd") == 0) {
        char *translated = translate_hindi_to_english(argv[3]);
        execute_query(db, translated);
        free(translated);
    }

    sqlite3_close(db);
    return 0;
}
