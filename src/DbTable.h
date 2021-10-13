


#include <string>
#include "sqlite3.h"

#define DB_TABLE_INVALID_ID -1
#define SAVE_RETRY_SLEEP 100000 //microsecond

class DbTable {
public:

    DbTable(const std::string& dbName);

    DbTable(const std::string& dbPath, const std::string& dbName);

    DbTable(const std::string& dbPath, const std::string& dbFileName,
        const std::string& dbName);

    /**
     * copy constructor, just copy dbName and set db to NULL
     * @param orig
     */
    DbTable(const DbTable& orig);

    /**
     * Destructor 
     */
    ~DbTable();

    /**
     * Overload assignment operator.
     * @param right
     * @return *this to support cascading
     */
    DbTable& operator=(const DbTable& right);

    /**
     * This method will create and open database if it is not open.
     * @return pointer to db handler
     */
    sqlite3* getDb();

    /**
     * This method save/update the table in local database
     * @return true on success, false on failure
     */
    virtual bool save(bool force = true) = 0;


private:
    /**
     * The name of db
     */
    std::string dbName;
    /**
     * The name of db file
     */
    std::string dbFileName;
    /**
     * The path of db
     */
    std::string dbPath;

    /**
     * A pointer to shared database between all derived class
     */
    sqlite3* db;

    /**
     * This method will return schema of this table.
     * @return query that create table schema
     */
    virtual const std::string getSchema() = 0;

    /**
     * This method will set member from the value given by statement with
     * respect to its type.
     * 
     * @param statement result of sqlite3 query
     */
    virtual void setAttributesBySqliteStmt(sqlite3_stmt *statement) = 0;

    /**
     * This member will bind member value to statement
     * @param statement
     */
    virtual void bindValueToSqliteStmt(sqlite3_stmt *statement) = 0;

    /**
     * This method will set default value of each members (default values are
     * similar to table schema)
     */
    virtual void setDefaultMemberValues() = 0;

    int tableExist();
protected:

    /**
     * this is for table who need to have dynamic db name
     * @return 
     */
    const std::string& getDbName() const;

    /**
     * 
     * @param name
     */
    void setDbName(const std::string name);

    /**
     * This method will close and free the resource associated to db.
     */
    void closeDb();

    /**
     * set db to NULL, Used in any copy constructor
     */
    void initDb();

    /**
     * Print sql query , how to use :
     *  sqlite3_trace(getDb(), sqliteCallbackFunc, NULL);
     * @param foo
     * @param statement
     */
    static void sqliteCallbackFunc(void *foo, const char* statement);

    int sqlite3_prepare_v2(sqlite3 *db, const char *zSql, int nByte, sqlite3_stmt **ppStmt, const char **pzTail);

    int sqlite3_step(sqlite3_stmt*, bool force = true);

    /**
     * This method will create table schema
     * @return 0 on success, -1 on failure
     */
    int createSchema();
};

