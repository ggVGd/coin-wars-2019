#pragma once
#include <string>
#include <vector>

struct sqlite3;
struct sqlite3_stmt;

class Database
{
	Database();
	Database(const Database&) = delete;
	Database& operator=(const Database&) = delete;
public:
	~Database();
	static Database& getSingleton();

	void createDepartment(const std::string& name);
	void givePoints(const std::string& department, int points);

	struct DepartmentPoints
	{
		std::string department;
		int points;
	};
	const std::vector<DepartmentPoints>& getPoints();
	int getPoints(const std::string&);

private:
	sqlite3* _db;
	sqlite3_stmt* _stmtCreateDepartment;
	sqlite3_stmt* _stmtGivePoints;
	sqlite3_stmt* _stmtGetPoints;

	bool _pointCacheValid;
	std::vector<DepartmentPoints> _pointCache;

	void _init();
	void _handleError(int);
};
