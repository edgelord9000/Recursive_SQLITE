//Avery Tan
//assignment1 CMPUT391
#include <stdio.h>
#include <sqlite3.h>

int main(int argc, char **argv){
  sqlite3 *db; //the database
  sqlite3_stmt *stmt; //the update statement
  char database_name[25] = "openflights.db";



  /*
  Q6 (5 pts)

  Write a C program, in a file called q6.c that prints the list of 
  airports that can be reached from YEG with at most 3 connections.
  */
  char sql_statement[999] =  "WITH RECURSIVE "\
                              "cnt(id, iata, level) AS ( "\
                                    "SELECT r.Destination_airport_ID, r.Destination_airport, 1 "\
                                    "FROM routes r "\
                                    "WHERE r.source_airport = \"YEG\" "\
                                          "UNION "\
                                    "SELECT r1.Destination_airport_ID, r1.Destination_airport, cn.level+1 "\
                                    "FROM cnt cn, routes r1 "\
                                    "WHERE r1.source_airport_ID = cn.id AND (cn.level = 1 or cn.level = 2 or cn.level = 3 ) "\
                                    "ORDER BY 2 DESC "\
                                    
                              ") "\
                              "SELECT DISTINCT ap66.name "\
                              "FROM airports ap66, cnt cn66 "\
                              "WHERE cn66.id = ap66.airport_id;";


  int rc;


  rc = sqlite3_open(database_name, &db);
  if( rc ){
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return(1);
  }
  //char *sql_stmt = sql_statement;

  rc = sqlite3_prepare_v2(db, sql_statement, -1, &stmt, 0);

  if (rc != SQLITE_OK) {  
    fprintf(stderr, "Preparation failed: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return 1;
  }    
    
  while((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
    int col;
    for(col=0; col<sqlite3_column_count(stmt)-1; col++) {
      printf("%s|", sqlite3_column_text(stmt, col));
    }
    printf("%s", sqlite3_column_text(stmt, col));
    printf("\n");
  }

  sqlite3_finalize(stmt); //always finalize a statement
}