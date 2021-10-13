#include "zf_log/zf_log.h"
#include "DbTable.h"
#include <sstream>
#include <string>
#include <iostream>
#include "Constant.h"
#include <unistd.h>

DbTable::DbTable(const std::string& dbName)
{
    this->dbName = dbName;
    this->dbFileName = dbName;
    this->dbPath = DB_PATH + std::string("/") + this->dbFileName + ".db";
    this->initDb();
}

DbTable::DbTable(const std::string& dbPath, const std::string& dbName)
{
    this->dbName = dbName;
    this->dbFileName = dbName;
    this->dbPath = dbPath + std::string("/") + this->dbFileName + ".db";
    this->initDb();
}

DbTable::DbTable(const std::string& dbPath, const std::string& dbFileName,
    const std::string& dbName)
{
    this->dbName = dbName;
    this->dbFileName = dbFileName;
    this->dbPath = dbPath + std::string("/") + this->dbFileName + ".db";
    this->initDb();
}

DbTable::DbTable(const DbTable& orig)
{
    this->dbName = orig.dbName;
    this->dbFileName = orig.dbFileName;
    this->dbPath = orig.dbPath;
    this->initDb();
}

DbTable::~DbTable()
{
    closeDb();
}

DbTable& DbTable::operator=(const DbTable& right)
{
    this->dbName = right.dbName;
    this->dbFileName = right.dbFileName;
    this->dbPath = right.dbPath;
    this->initDb();

    return *this;
}

int DbTable::tableExist()
{
    sqlite3* db;
    int sqliteRet;
    bool force = true;
    sqliteRet = sqlite3_open_v2(this->dbPath.c_str(), &db, SQLITE_OPEN_READONLY, NULL);
    if (sqliteRet != SQLITE_OK) {
        ZF_LOGE("Cannot open DB: (%s)", this->dbPath.c_str());
        return sqliteRet;
    }
    sqlite3_stmt* statement;
    std::string query = "";
    query.append("pragma table_info(");
    query.append(this->dbName);
    query.append(");");
    sqliteRet = sqlite3_prepare_v2(db, query.c_str(), -1, &statement, NULL);
    if (sqliteRet != SQLITE_OK) {
        ZF_LOGE("while preparing query (%s)", sqlite3_errmsg(getDb()));
    }
    else {
        sqliteRet = sqlite3_step(statement, force);
        if (sqliteRet != SQLITE_DONE && sqliteRet != SQLITE_ROW) {
            ZF_LOGE("Error in pragma table_info query (%s)", sqlite3_errmsg(getDb()));
        }
        else {
            if (sqlite3_data_count(statement) > 0) {
                sqliteRet = SQLITE_OK;
            }
        }
    }

    sqlite3_finalize(statement);
    sqlite3_close(db);
    return sqliteRet;
}

sqlite3* DbTable::getDb()
{
    if (dbName.empty()) {
        return NULL;
    }

    if (db != NULL) {
        return DbTable::db;
    }

    if (sqlite3_open_v2(this->dbPath.c_str(), &db, SQLITE_OPEN_READWRITE |
        SQLITE_OPEN_CREATE | SQLITE_OPEN_FULLMUTEX, NULL) != SQLITE_OK) {
        ZF_LOGE("while opening database %s (%s)", this->dbPath.c_str(), sqlite3_errmsg(db));
        closeDb();
        return NULL;
    }
    if (tableExist() != SQLITE_OK) {
        if (createSchema() != 0) {
            closeDb();
            return NULL;
        }
    }
    return db;

}

void DbTable::closeDb()
{
    if (db != NULL) {
        //sqlite3_close(DbTable::db);
        sqlite3_close_v2(DbTable::db);
        db = NULL;
    }
}

void DbTable::initDb()
{
    this->db = NULL;
}

const std::string& DbTable::getDbName() const
{
    return this->dbName;
}

void DbTable::setDbName(const std::string name)
{
    this->dbName = name;
}

void printSchema(std::string schema)
{
#if _ZF_LOG_LEVEL <= ZF_LOG_VERBOSE 
    std::cout << "CreateSchema:\n" << schema << "\n";
#endif
}

int DbTable::createSchema()
{
    sqlite3_trace(db, sqliteCallbackFunc, NULL);
    bool force = true;
    int sqliteRet;
    do {
        sqliteRet = sqlite3_exec(db, this->getSchema().c_str(), NULL, NULL, NULL);
        if (sqliteRet == SQLITE_BUSY || sqliteRet == SQLITE_LOCKED) {
            ZF_LOGE("*********SQLITE_BUSY or SQLITE_LOCKED*********");
            usleep(SAVE_RETRY_SLEEP);
        }
    } while (force && (sqliteRet == SQLITE_BUSY || sqliteRet == SQLITE_LOCKED));
    if (sqliteRet == SQLITE_OK || sqliteRet == SQLITE_DONE) {
        printSchema(this->getSchema());
    }
    else {
        ZF_LOGE("while creating table schema (%s)", sqlite3_errmsg(getDb()));
    }
    return 0;
}

void DbTable::sqliteCallbackFunc(void *foo, const char* statement)
{
#if _ZF_LOG_LEVEL <= ZF_LOG_VERBOSE 
    std::cout << "Query:\n" << statement << "\n";
#endif
}

int DbTable::sqlite3_prepare_v2(sqlite3* db, const char* zSql, int nByte, sqlite3_stmt** ppStmt, const char** pzTail)
{
    int sqliteRet;
    do {
        sqliteRet = ::sqlite3_prepare_v2(db, zSql, nByte, ppStmt, pzTail);
        if (sqliteRet == SQLITE_BUSY || sqliteRet == SQLITE_LOCKED) {
            ZF_LOGE("*** %s ******SQLITE_BUSY or SQLITE_LOCKED*********", this->getDbName().c_str());
            usleep(SAVE_RETRY_SLEEP);
        }
    } while ((sqliteRet == SQLITE_BUSY || sqliteRet == SQLITE_LOCKED));

    return sqliteRet;
}

int DbTable::sqlite3_step(sqlite3_stmt* statement, bool force)
{
    int sqliteRet;
    sqlite3_trace(getDb(), sqliteCallbackFunc, NULL);
    do {
        sqliteRet = ::sqlite3_step(statement);
        if (sqliteRet == SQLITE_BUSY || sqliteRet == SQLITE_LOCKED) {
            ZF_LOGE("*** %s ******SQLITE_BUSY or SQLITE_LOCKED*********", this->getDbName().c_str());
            usleep(SAVE_RETRY_SLEEP);
        }
    } while (force && (sqliteRet == SQLITE_BUSY || sqliteRet == SQLITE_LOCKED));

    return sqliteRet;
}
