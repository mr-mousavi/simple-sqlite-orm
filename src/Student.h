


#include "DbTable.h"


class Student : public DbTable {
public:
    Student();
    ~Student();

    /**
     * Overload assignment operator.
     * @param right
     * @return *this to support cascading
     */
    Student& operator=(const Student& right);

    /**
     * This method retrieves a Student.
     * @return true on success, false on failure
     */
    bool load(bool force = true);

    /**
     * This method save/update the Student in local database
     * @return ture on success, false on failure
     */
    virtual bool save(bool force = true);

    void setId(int id);
    int getId();

    void setFirstName(std::string firstName);
    std::string getFirstName();

    void setLastName(std::string lastName);
    std::string getLastName();

    void setNationcode(std::string nationCode);
    std::string getNationcode();

    void setBirthDate(std::string birthDate);
    std::string getBirthDate();


    /**
     * print member using logger
     */
    void print();
    void resetToDefaultValue();

private:

    /**
 * @copydoc DbTable::getSchema()
 */
    virtual const std::string getSchema();

    /**
     * @copydoc DbTable::setAttributesBySqliteStmt()
     */
    virtual void setAttributesBySqliteStmt(sqlite3_stmt* statement);

    /**
     * @copydoc DbTable::bindValueToSqliteStmt()
     */
    virtual void bindValueToSqliteStmt(sqlite3_stmt* statement);

    /**
     * @copydoc DbTable::setDefaultMemberValues()
     */
    virtual void setDefaultMemberValues();


    int id;
    std::string first_name;
    std::string last_name;
    std::string nation_code;
    std::string birth_date;

};

