#include "UtilityEntryMenu.h"

#include "imgui.h"

UtilityEntryMenu::UtilityEntryMenu()
{
	add_entry();
}

UtilityEntryMenu::~UtilityEntryMenu()
{
	if (this->bundle) delete this->bundle;
}

void UtilityEntryMenu::show(int x, int y, int width, int height, bool* p_open)
{
	static bool no_titlebar = false;
	static bool no_scrollbar = false;
	static bool no_menu = true;
	static bool no_move = true;

	static bool no_resize = false;
	//static bool no_resize = true;

	static bool no_collapse = true;
	static bool no_close = true;
	static bool no_nav = false;
	static bool no_background = false;
	static bool no_bring_to_front = false;

	ImGuiWindowFlags window_flags = 0;
	if (no_titlebar)        window_flags |= ImGuiWindowFlags_NoTitleBar;
	if (no_scrollbar)       window_flags |= ImGuiWindowFlags_NoScrollbar;
	if (!no_menu)           window_flags |= ImGuiWindowFlags_MenuBar;
	if (no_move)            window_flags |= ImGuiWindowFlags_NoMove;
	if (no_resize)          window_flags |= ImGuiWindowFlags_NoResize;
	if (no_collapse)        window_flags |= ImGuiWindowFlags_NoCollapse;
	if (no_nav)             window_flags |= ImGuiWindowFlags_NoNav;
	if (no_background)      window_flags |= ImGuiWindowFlags_NoBackground;
	if (no_bring_to_front)  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
	if (no_close)           p_open = NULL; // Don't pass our bool* to Begin

	ImGui::SetNextWindowPos(glm::vec2(x, y), ImGuiCond_Always);
	ImGui::SetNextWindowSize(glm::vec2(width, height), ImGuiCond_Always);

	if (!ImGui::Begin("Utilities", p_open, window_flags))
	{
		//Early out if the window is collapsed, as an optimization
		ImGui::End();
		return;
	}

	ImGui::Separator();

	const char* items[] = { "Water", "Electricity", "Gas", "Sewer", "Generic", "Fiber", "Phone" };

	int utility_index = 1;
	for (std::vector<UtilityEntry>::iterator e = this->entries.begin(); e != this->entries.end(); e++)
	{
		ImGui::Columns(1);
		ImGui::Text("%d)", utility_index);

		ImGui::Columns(2, "Entries", false);
		ImGui::SetColumnWidth(0, 100);
		ImGui::SetColumnWidth(1, 400);

		ImGui::Text("Utility Type");
		ImGui::NextColumn();

		char utility_type_combo_label[20];
		sprintf(utility_type_combo_label, "##UtilityTypeCombo%d", utility_index);
		ImGui::Combo(utility_type_combo_label, &e->type_index, items, IM_ARRAYSIZE(items));
		ImGui::NextColumn();

		ImGui::Text("Diameter");
		ImGui::NextColumn();
		char diameter_field_label[22];
		sprintf(diameter_field_label, "\"##DiameterField%d", utility_index);
		ImGui::InputText(diameter_field_label, e->diameter_inches, 8, ImGuiInputTextFlags_CharsDecimal);
		ImGui::NextColumn();

		ImGui::Text("Number");
		ImGui::NextColumn();
		char number_field_label[20];
		sprintf(number_field_label, "##NumberField%d", utility_index);
		ImGui::InputText(number_field_label, e->number, 8, ImGuiInputTextFlags_CharsDecimal);
		ImGui::NextColumn();

		ImGui::Separator();

		utility_index++;
	}

	ImGui::Columns(1);

	if (ImGui::Button("Execute"))
	{
		this->on_execute();
	}
	ImGui::SameLine();
	if (ImGui::Button("Add Entry"))
		this->add_entry();

	ImGui::End();
}

void UtilityEntryMenu::add_entry()
{
	UtilityEntry new_entry;
	this->entries.push_back(new_entry);
}

void UtilityEntryMenu::create_utilities()
{
	//Convert utility entries into a UtilityBundle object

	if (this->bundle)
	{
		delete this->bundle;
	}

	std::vector<Utility> utilities;

	for (std::vector<UtilityEntry>::iterator e = this->entries.begin(); e != this->entries.end(); e++)
	{
		for (int i = 0; i < atoi(e->number); i++)
		{
			float x = 0, y = 0;

			utilities.push_back(Utility(x, y, atof(e->diameter_inches) / 2.0f, (UtilityType)e->type_index));
		}
	}

	this->bundle = new UtilityBundle(utilities);
}

void UtilityEntryMenu::on_execute()
{
	this->create_utilities();

	this->execute_algorithm_flag = true;
}
