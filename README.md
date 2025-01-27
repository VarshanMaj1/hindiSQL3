# hindiSQL3

1. Clone the repository in your local machine.
2. gcc wrapper.c -o hindi_sqlite3 -lsqlite3
3. sudo mv hindi_sqlite3 /usr/local/bin/
4. hindi_sqlite3 aDB.db

   eg . 1) बनाएं टेबल उपयोगकर्ता (आईडी INTEGER, नाम TEXT);
        2) डालें INTO उपयोगकर्ता VALUES (1, 'Varshan');
        3) चुनें * FROM उपयोगकर्ता;
