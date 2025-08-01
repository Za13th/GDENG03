#include "DebugUIManager.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

DebugUIManager* DebugUIManager::sharedInstance = nullptr;

DebugUIManager* DebugUIManager::getInstance()
{
	if (!sharedInstance)
	{
		sharedInstance = new DebugUIManager();
	}
	return sharedInstance;
}

void DebugUIManager::initialize(void* shaderByteCode, size_t sizeShader)
{
	if (!sharedInstance)
	{
		sharedInstance = new DebugUIManager();
	}
}

void DebugUIManager::destroy()
{
	if (sharedInstance != nullptr)
	{
		delete sharedInstance;
		sharedInstance = nullptr;
	}
}

void DebugUIManager::draw()
{
	static bool scroll = false;
	ImGuiWindowFlags debugFlags = 0;
	debugFlags |= ImGuiWindowFlags_NoMove;
	debugFlags |= ImGuiWindowFlags_NoResize;
	ImGui::Begin("Debugger", nullptr, debugFlags);
	ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 0, 255));
	for(auto line : debugLines) {
		char* convert = line.data();
		ImGui::Text(convert);
		scroll = true;
	}
	ImGui::PopStyleColor();

	if (scroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
		ImGui::SetScrollHereY(1.0f);
		scroll = false;
	}
	ImGui::End();
}

void DebugUIManager::Log(std::string msg)
{
	debugLines.push_back(msg);
}

DebugUIManager::DebugUIManager()
{

}

DebugUIManager::~DebugUIManager()
{

}