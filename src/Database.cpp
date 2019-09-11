#include "Database.h"
#include <boost/filesystem.hpp>
#include <sqlite3.h>

Database::Database()
{
	sqlite3_open("data.db", &_db);

	_init();

	createDepartment("QWERTY");
	createDepartment("ASDFGH");
	createDepartment("ZXCVBN");

	givePoints("QWERTY", 123);
	givePoints("ASDFGH", 123);
	givePoints("QWERTY", 123);
	givePoints("QWERTY", 123);
}
Database::~Database()
{
	sqlite3_finalize(_stmtCreateDepartment);
	sqlite3_finalize(_stmtGivePoints);
	sqlite3_finalize(_stmtGetPoints);
	sqlite3_close(_db);
}
Database& Database::getSingleton()
{
	static Database instance;
	return instance;
}
void Database::createDepartment(const std::string& name)
{
	_handleError(sqlite3_bind_text(_stmtCreateDepartment, 1, name.c_str(), -1, SQLITE_TRANSIENT));
	_handleError(sqlite3_step(_stmtCreateDepartment));
	_handleError(sqlite3_reset(_stmtCreateDepartment));

	_pointCacheValid = false;
}
void Database::givePoints(const std::string& department, int points)
{
	_handleError(sqlite3_bind_text(_stmtGivePoints, 1, department.c_str(), -1, SQLITE_TRANSIENT));
	_handleError(sqlite3_bind_int(_stmtGivePoints, 2, points));
	_handleError(sqlite3_step(_stmtGivePoints));
	_handleError(sqlite3_reset(_stmtGivePoints));

	_pointCacheValid = false;
}
const std::vector<Database::DepartmentPoints>& Database::getPoints()
{
	if(!_pointCacheValid)
	{
		_pointCache.clear();
		while(sqlite3_step(_stmtGetPoints) == SQLITE_ROW)
		{
			const std::string department(reinterpret_cast<const char*>(sqlite3_column_text(_stmtGetPoints, 0)));
			const int points = sqlite3_column_int(_stmtGetPoints, 1);
			_pointCache.push_back({ department, points });
		}
		_handleError(sqlite3_reset(_stmtGetPoints));
		_pointCacheValid = true;
	}
	return _pointCache;
}
int Database::getPoints(const std::string& department)
{
	const auto& points = getPoints();
	for(const auto& item : points)
	{
		if(item.department == department)
			return item.points;
	}
	return 0;
}
void Database::_init()
{
	_handleError(sqlite3_exec(_db, R"(
		CREATE TABLE IF NOT EXISTS department (
			id    INTEGER PRIMARY KEY AUTOINCREMENT,
			name  TEXT    NOT NULL
		);

		CREATE TABLE IF NOT EXISTS department_points (
			id              INTEGER PRIMARY KEY AUTOINCREMENT,
			department_id   INTEGER NOT NULL,
			points          INTEGER NOT NULL,
			timestamp       INTEGER NOT NULL,

			FOREIGN KEY (department_id) REFERENCES department(id)
		);
	)", nullptr, 0, nullptr));

	_handleError(sqlite3_prepare_v2(_db, R"(
		INSERT INTO department (name)
		SELECT ?1
		WHERE NOT EXISTS (
			SELECT 1 FROM department WHERE name=?1
		);
	)", -1, &_stmtCreateDepartment, nullptr));

	_handleError(sqlite3_prepare_v2(_db, R"(
		INSERT INTO department_points (department_id, points, timestamp)
		VALUES (
			( SELECT id FROM department WHERE name=?1 ),
			?2,
			strftime('%s', 'now')
		);
	)", -1, &_stmtGivePoints, nullptr));

	_handleError(sqlite3_prepare_v2(_db, R"(
		SELECT d.name, SUM(dp.points)
		FROM department d
		INNER JOIN department_points dp ON d.id = dp.department_id
		GROUP BY d.name
		ORDER BY 2 DESC
	)", -1, &_stmtGetPoints, nullptr));
}
void Database::_handleError(int rc)
{
	if(rc == SQLITE_OK)
		return;
	if(rc == SQLITE_DONE)
		return;
	throw std::runtime_error(sqlite3_errmsg(_db));
}
