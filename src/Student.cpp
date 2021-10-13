#include "Student.h"
#include "Constant.h"
#include "zf_log/zf_log.h"
#include <string.h>

Student::Student()
    :DbTable(DB_PATH, DB_FILE_NAME, TABLE_NAME_STUDENT)
{
    setDefaultMemberValues();
}

Student::~Student()
{
}


Student& Student::operator=(const Student& right)
{
    if (this == &right)
        return *this;

    this->setDbName(right.getDbName());
    this->initDb();

    this->id = right.id;
    this->first_name = right.first_name;
    this->last_name = right.last_name;
    this->nation_code = right.nation_code;
    this->birth_date = right.birth_date;

    return *this;
}

bool Student::load(bool force)
{
    sqlite3* db = getDb();
    if (db == NULL)
        return false;
    std::string query = "";
    query.append("SELECT * FROM `");
    query.append(TABLE_NAME_STUDENT);
    query.append("` ");
    query.append("ORDER BY id DESC LIMIT 1");

    sqlite3_stmt* statement;
    int sqliteRet = sqlite3_prepare_v2(db, query.c_str(), -1,
        &statement, NULL);
    if (sqliteRet == SQLITE_OK || sqliteRet == SQLITE_DONE) {
        sqliteRet = sqlite3_step(statement, force);
        if (sqliteRet == SQLITE_ROW)
            setAttributesBySqliteStmt(statement);
        else if (sqliteRet == SQLITE_DONE)
            ZF_LOGV("There is no data");
        else
            ZF_LOGE("while running sqlite3_step (%s) (%s)", query.c_str(), sqlite3_errmsg(db));
    }
    else {
        ZF_LOGE("while preparing query(%s) (%s)", query.c_str(), sqlite3_errmsg(db));
    }

    ZF_LOGV("sqlite return code: %d", sqliteRet);
    sqlite3_finalize(statement);
    closeDb();

    return (sqliteRet == SQLITE_ROW);
}

bool Student::save(bool force)
{
    sqlite3* db = getDb();
    if (db == NULL)
        return false;

    std::string query = "";
    query.append("REPLACE INTO `");
    query.append(TABLE_NAME_STUDENT);
    query.append("` ");
    query.append("(`first_name`, `last_name`, `nation_code`, `birth_date`");
    if (id != DB_TABLE_INVALID_ID)
        query.append(",`id`");
    query.append(")");
    query.append(" VALUES (@first_name, @last_name, @nation_code, @birth_date");
    if (id != DB_TABLE_INVALID_ID)
        query.append(", @id ");
    query.append(")");

    sqlite3_stmt* statement;
    int sqliteRet = sqlite3_prepare_v2(db, query.c_str(), -1,
        &statement, NULL);
    ZF_LOGI("sqlite return code: %d", sqliteRet);
    if (sqliteRet == SQLITE_OK || sqliteRet == SQLITE_DONE) {
        bindValueToSqliteStmt(statement);
        sqliteRet = sqlite3_step(statement, force);
        if (sqliteRet != SQLITE_DONE)
            ZF_LOGE("while running sqlite3_step (%s)", sqlite3_errmsg(db));
    }
    else {
        ZF_LOGE("while preparing query (%s)", sqlite3_errmsg(db));
    }
    ZF_LOGV("sqlite return code: %d", sqliteRet);
    sqlite3_finalize(statement);
    closeDb();

    return (sqliteRet == SQLITE_DONE);
}

void Student::print()
{
    ZF_LOGI("++++++++++++++++++++++Student++++++++++++++++++++++");
    ZF_LOGI("id                            %d", id);
    ZF_LOGI("first_name                          %s", first_name.c_str());
    ZF_LOGI("last_name                          %s", last_name.c_str());
    ZF_LOGI("nation_code                          %s", nation_code.c_str());
    ZF_LOGI("birth_date                          %s", birth_date.c_str());
    ZF_LOGI("----------------------Student----------------------");

}

const std::string Student::getSchema()
{
    std::string query = "";
    query.append("CREATE TABLE IF NOT EXISTS `");
    query.append(TABLE_NAME_STUDENT);
    query.append("` (");
    query.append("`id` INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,");
    query.append("`first_name` TEXT NOT NULL DEFAULT \"\",");
    query.append("`last_name` TEXT NOT NULL DEFAULT \"\",");
    query.append("`nation_code` TEXT NOT NULL DEFAULT \"\",");
    query.append("`birth_date` TEXT NOT NULL DEFAULT \"\")");

    return query;
}

void Student::setAttributesBySqliteStmt(sqlite3_stmt* statement)
{
    int columnCnt = sqlite3_column_count(statement);
    for (int i = 0; i < columnCnt; i++) {
        if (strcmp("id", sqlite3_column_name(statement, i)) == 0)
            id = sqlite3_column_int(statement, i);
        else if (strcmp("first_name", sqlite3_column_name(statement, i)) == 0)
            first_name = reinterpret_cast<const char*> (
                sqlite3_column_text(statement, i));
        else if (strcmp("last_name", sqlite3_column_name(statement, i)) == 0)
            last_name = reinterpret_cast<const char*> (
                sqlite3_column_text(statement, i));
        else if (strcmp("nation_code", sqlite3_column_name(statement, i)) == 0)
            nation_code = reinterpret_cast<const char*> (
                sqlite3_column_text(statement, i));
        else if (strcmp("birth_date", sqlite3_column_name(statement, i)) == 0)
            birth_date = reinterpret_cast<const char*> (
                sqlite3_column_text(statement, i));
        else {
            ZF_LOGW("Invalid column name(%s) in table name Student",
                sqlite3_column_name(statement, i));
        }
    }
}

void Student::bindValueToSqliteStmt(sqlite3_stmt* statement)
{
    int paramCnt = sqlite3_bind_parameter_count(statement);
    for (int i = 1; i <= paramCnt; i++) {
        if (i == sqlite3_bind_parameter_index(statement, "@id"))
            sqlite3_bind_int(statement, i, id);
        else if (i == sqlite3_bind_parameter_index(statement, "@first_name"))
            sqlite3_bind_text(statement, i, first_name.c_str(), first_name.size(), NULL);
        else if (i == sqlite3_bind_parameter_index(statement, "@last_name"))
            sqlite3_bind_text(statement, i, last_name.c_str(), last_name.size(), NULL);
        else if (i == sqlite3_bind_parameter_index(statement, "@nation_code"))
            sqlite3_bind_text(statement, i, nation_code.c_str(), nation_code.size(), NULL);
        else if (i == sqlite3_bind_parameter_index(statement, "@birth_date"))
            sqlite3_bind_text(statement, i, birth_date.c_str(), birth_date.size(), NULL);
        else {
            ZF_LOGW("Invalid param name(%s) in table name Student",
                sqlite3_bind_parameter_name(statement, i));
        }
    }
}

void Student::resetToDefaultValue()
{
    setDefaultMemberValues();

}

void Student::setDefaultMemberValues()
{
    id = -1;
    first_name = std::string("");
    last_name = std::string("");
    nation_code = std::string("");
    birth_date = std::string("");

}

void Student::setId(int id)
{
    this->id = id;
}
int Student::getId()
{
    return this->id;
}


void Student::setFirstName(std::string firstName)
{
    this->first_name = firstName;
}
std::string Student::getFirstName()
{
    return this->first_name;
}

void Student::setLastName(std::string lastName)
{
    this->last_name = lastName;
}
std::string Student::getLastName()
{
    return this->last_name;
}

void Student::setNationcode(std::string nationCode)
{
    this->nation_code = nationCode;
}
std::string Student::getNationcode()
{
    return this->nation_code;
}
void Student::setBirthDate(std::string birthDate)
{
    this->birth_date = birthDate;
}
std::string Student::getBirthDate()
{
    return this->birth_date;
}
