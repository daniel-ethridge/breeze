#include "breeze/orm.h"

#include <QSqlQuery>
#include <QString>
#include <iostream>
#include <QSqlRecord>

namespace Breeze
{
	namespace Orm
	{
		namespace
		{
			typedef DbColumnTypes             colTypes;
			const std::map<colTypes, QString> colMap = { { colTypes::INTEGER, "int" },
																									 { colTypes::FLOAT, "float" },
																									 { colTypes::STRING, "VARCHAR(255)" },
	#ifdef PGSQL
																									 { colTypes::INTEGER_ARR, "int[]" },
																									 { colTypes::FLOAT_ARR, "float[]" }
	#endif
			};

			const std::map<colTypes, QMetaType::Type> typeMap = { { colTypes::INTEGER, QMetaType::Type::Int },
																														{ colTypes::FLOAT, QMetaType::Type::Double },
																														{ colTypes::STRING, QMetaType::Type::QString },
	#ifdef PGSQL
																														{ colTypes::INTEGER_ARR, QMetaType::Type::Int },
																														{ colTypes::FLOAT_ARR, QMetaType::Type::Double }
	#endif
			};

			const std::map<DbDrivers, QString> driverMap = { { DbDrivers::POSTGRESQL, "PGSQL" } };

			void replaceEnd(QString& str, const QString& replacementChar)
			{
				str.replace(str.size() - 1, 1, replacementChar);
			}
		}  // namespace

		QueryBuilder::QueryBuilder(Model* model) { m_model = model; }

		QueryBuilder& QueryBuilder::select(const QVector<QString>& columns)
		{
			m_query.append("SELECT ");
			for (auto iter = columns.begin(); iter != columns.end(); iter++)
				m_query.append(*iter).append(",");

			replaceEnd(m_query, " FROM ");
			m_query.append(m_model->tablename());

			return *this;
		}

		QueryBuilder& QueryBuilder::select(const QString& column)
		{
			m_query.append("SELECT ").append(column).append(" FROM ").append(m_model->tablename());
			return *this;
		}

		QueryBuilder& QueryBuilder::where(const QString& LHS, const QString& op, const QString& RHS)
		{
			m_query.append(" WHERE ").append(LHS).append(" ").append(op).append(" ").append(RHS);
			return *this;
		}

		QueryBuilder& QueryBuilder::whereEq(const QString& LHS, const QString& RHS)
		{
			return where(LHS, "=", RHS);
		}

		QueryBuilder& QueryBuilder::whereEq(const QString& LHS, int RHS)
		{
			return whereEq(LHS, QString::number(RHS));
		}

		QueryBuilder& QueryBuilder::whereEq(const QString& LHS, double RHS)
		{
			return whereEq(LHS, QString::number(RHS));
		}

		QueryBuilder& QueryBuilder::whereGreater(const QString& LHS, int RHS)
		{
			return where(LHS, ">", QString::number(RHS));
		}

		QueryBuilder& QueryBuilder::whereGreater(const QString& LHS, double RHS)
		{
			return where(LHS, ">", QString::number(RHS));
		}

		QueryBuilder& QueryBuilder::whereGreaterEq(const QString& LHS, int RHS)
		{
			return where(LHS, ">=", QString::number(RHS));
		}

		QueryBuilder& QueryBuilder::whereGreaterEq(const QString& LHS, double RHS)
		{
			return where(LHS, ">=", QString::number(RHS));
		}

		QueryBuilder& QueryBuilder::whereLess(const QString& LHS, int RHS)
		{
			return where(LHS, "<", QString::number(RHS));
		}

		QueryBuilder& QueryBuilder::whereLess(const QString& LHS, double RHS)
		{
			return where(LHS, "<", QString::number(RHS));
		}

		QueryBuilder& QueryBuilder::whereLessEq(const QString& LHS, int RHS)
		{
			return where(LHS, "<=", QString::number(RHS));
		}

		QueryBuilder& QueryBuilder::whereLessEq(const QString& LHS, double RHS)
		{
			return where(LHS, "<=", QString::number(RHS));
		}

		QueryBuilder& QueryBuilder::and_()
		{
			m_query.append(" AND ");
			return *this;
		}

		QueryBuilder& QueryBuilder::or_()
		{
			m_query.append(" OR ");
			return *this;
		}

		void QueryBuilder::run(const QString& queryStr)
		{
			if (queryStr.contains(""))
				m_query = queryStr;

			QSqlQuery query(m_query + ";");
			m_query = "";
			m_model->parseQuery(std::move(query));
		}

		QSqlDatabase createDbConnection(DbDrivers      driver,
			const QString& hostName,
			const QString& databaseName,
			const QString& username,
			const QString& password,
			const QString& connectionName)
		{
			QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL", connectionName);
			db.setHostName(hostName);
			db.setDatabaseName(databaseName);
			db.setUserName(username);
			db.setPassword(password);

			if (!db.open())
				throw;

			return db;
		}

		Model::Model(const QString& tablename) : u_attributes({}), u_tableName(tablename), m_queryBuilder(this) {}

		void Model::create()
		{
			QString queryStr("CREATE TABLE ");
			queryStr.append(u_tableName).append(" (id SERIAL PRIMARY KEY");

			QString       columnName;
			DbColumnTypes columnType;

			for (auto iter = u_attributes.begin(); iter != u_attributes.end(); iter++)
			{
				columnName = (*iter)[0].toString();
				columnType = static_cast<DbColumnTypes>((*iter)[1].toInt());
				queryStr.append(", ").append(columnName).append(" ").append(colMap.at(columnType));
			}

			queryStr.append(");");
			m_queryBuilder.run(queryStr);
		}

		void Model::update(const QVector<QVariantList>& values)
		{
			QString queryStr("INSERT INTO ");
			queryStr.append(u_tableName).append(columns()).append(" VALUES ");
			for (size_t i = 0; i < values.size(); i++)
			{
				queryStr.append(convertValues(values[i])).append(",");
			}

			replaceEnd(queryStr, ";");

			m_queryBuilder.run(queryStr);
		}

		QString Model::tablename() { return u_tableName; }

		QueryBuilder& Model::buildQuery() { return m_queryBuilder; }

		void Model::parseQuery(QSqlQuery&& query)
		{
			size_t size = u_attributes.size();
			QSqlRecord rec;
			QVariant data;

			while (query.next())
			{
				rec = query.record();
				for (size_t i = 0; i < size; i++)
				{
					data = rec.value(u_attributes[i][0].toString());
					u_map.at(i)(data);
				}
			}
		}

		QString Model::columns()
		{
			QString ret(" (");
			for (size_t i = 0; i < u_attributes.size(); i++)
				ret.append(u_attributes[i][0].toString()).append(",");

			ret.replace(ret.size() - 1, 1, ") ");
			return ret;
		}

		QString Model::convertValues(const QVariantList& values)
		{
			QString strVals("(");

			for (size_t i = 0; i < u_attributes.size(); i++)
			{
				switch (static_cast<DbColumnTypes>(u_attributes[i][1].toInt()))
				{
				case DbColumnTypes::INTEGER:
					strVals.append(QString::number(values[0].toInt())).append(",");
					break;

				case DbColumnTypes::FLOAT:
					strVals.append(QString::number(values[0].toDouble())).append(",");
					break;

				case DbColumnTypes::STRING:
					strVals.append("'").append(values[0].toString()).append("',");
					break;

				case DbColumnTypes::INTEGER_ARR:
					strVals.append("ARRAY[");
					for (auto e : values[0].value<QVector<int>>())
					{
						strVals.append(QString::number(e)).append(",");
					}
					replaceEnd(strVals, "]");
					strVals.append(",");

				case DbColumnTypes::FLOAT_ARR:
					strVals.append("ARRAY[");
					for (auto e : values[0].value<QVector<double>>())
					{
						strVals.append(QString::number(e)).append(",");
					}
					replaceEnd(strVals, "]");
					strVals.append(",");
					break;
				}
			}
			strVals.replace(strVals.size() - 1, 1, ")");
			return strVals;
		}
	}
}  // namespace Breeze::Orm