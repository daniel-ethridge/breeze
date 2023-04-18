#include <breeze/orm.h>

#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>
#include <iostream>

using namespace Breeze::Orm;

/**
 * Attributes:
 * name (string)
 * age (integer)
 */
class Person : public Model
{
public:
	Person(const QString& tablename) : Model(tablename)
	{
		ADD_ATTRIBUTE(m_name, "name", DbColumnTypes::STRING, toString());
		ADD_ATTRIBUTE(m_age, "age", DbColumnTypes::INTEGER, toInt());
	};

	QVector<QString> getName() { return m_name; }
	QVector<int> getAge() { return m_age; }

private:
	QVector<QString> m_name;
	QVector<int>     m_age;
};

int main()
{
	auto db = QSqlDatabase::addDatabase("QPSQL");
	db.setHostName("127.0.0.1");
	db.setDatabaseName("testdb");
	db.setUserName("postgres");
	db.setPassword("hacsdb");

	if (!db.open())
		throw;

	Person p("person");
	p.create();

	p.buildQuery().select("*").run();

	auto test = p.getName();
	auto test2 = p.getAge();

	return 0;
}
