#ifndef _BREEZE_ORM_H_
#define _BREEZE_ORM_H_

#include <QMap>
#include <QSqlDatabase>
#include <QVariant>
#include <memory>

#define PGSQL
#define COL_TYPE(x) QVariant::fromValue(x)

#define REGISTER_TYPE(x) m_dTypeVec.push_back()

#define ADD_ATTRIBUTE(member_var, column_name, db_column_type, qvariant_toT_method) \
	u_attributes.push_back({ column_name, QVariant::fromValue(db_column_type) });     \
	u_map[static_cast<int>(u_attributes.size() - 1)] = [this](const QVariant& val) { this->member_var.push_back(val.qvariant_toT_method); }

namespace Breeze::Orm
{
	enum class DbColumnTypes
	{
		INTEGER,
		FLOAT,
		STRING,
#ifdef PGSQL
		INTEGER_ARR,
		FLOAT_ARR,
#endif PGSQL
		COUNT
	};
	Q_DECLARE_METATYPE(DbColumnTypes);

	enum class DbDrivers
	{
		POSTGRESQL
	};

	class DatabaseConnection : public QSqlDatabase
	{};

	DatabaseConnection createDbConnection(DbDrivers      driver,
	                                      const QString& hostName,
	                                      const QString& databaseName,
	                                      const QString& username,
	                                      const QString& password,
	                                      const QString& connectionName = "");

	class Model;
	class QueryBuilder
	{
	public:
		QueryBuilder(Model* model);
		QueryBuilder& select(const QVector<QString>& columns);
		QueryBuilder& select(const QString& column);
		/*QueryBuilder<T>& where(const QString& LHS, const QString& op, const QString& RHS);*/
		void        run();
		static void run(const QString& queryStr);

	private:
		QString m_query = "";
		Model*  m_model;
	};

	class Model
	{
	public:
		Model(const QString& tablename);
		void    create();
		void    update(const QVector<QVariantList>& values);
		QString tablename();

		QueryBuilder& buildQuery();
		void          parseQuery(QSqlQuery&& query);

	protected:
		QVector<QVariantList>                               u_attributes;
		std::map<int, std::function<void(const QVariant&)>> u_map;
		QString                                             u_tableName;

	private:
		QueryBuilder m_queryBuilder;
		QString      columns();
		QString      convertValues(const QVariantList& values);
	};
}  // namespace Breeze::Orm

#endif  // _BREEZE_ORM_H_