#ifndef UTILITY_ENTRY_MENU_H
#define UTILITY_ENTRY_MENU_H

#include <vector>
#include "UtilityBundle.h"

struct UtilityEntry
{
	int type_index = 0;
	char diameter_inches[8] = "0";
	char number[8] = "0";
};

class UtilityEntryMenu
{
public:

	UtilityBundle* bundle = nullptr;

	bool execute_algorithm_flag = false;

	UtilityEntryMenu();
	~UtilityEntryMenu();

	void show(int x, int y, int width, int height, bool* p_open);

private:

	std::vector<UtilityEntry> entries;

	void add_entry();
	void create_utilities();
	void on_execute();

};

#endif
