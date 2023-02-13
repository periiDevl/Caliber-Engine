#include <imgui.h>
#include <vector>
#include <string>

class Console
{
public:
    Console()
    {
        memset(input_buf, 0, sizeof(input_buf));
    }

    void AddLog(const char* fmt, ...)
    {
        // Retrieve the message to be added to the logs buffer
        va_list args;
        va_start(args, fmt);
        char buf[1024];
        vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
        buf[IM_ARRAYSIZE(buf) - 1] = 0;
        va_end(args);

        logs.push_back(buf);
    }

    void Draw()
    {
        if (!ImGui::Begin("Console"))
        {
            ImGui::End();
            return;
        }

        // Display logs
        ImGui::BeginChild("ScrollingRegion", ImVec2(0, -ImGui::GetTextLineHeightWithSpacing()), false, ImGuiWindowFlags_HorizontalScrollbar);
        for (int i = 0; i < logs.size(); i++)
            ImGui::TextUnformatted(logs[i].c_str());
        ImGui::SetScrollHereY(1.0f);
        ImGui::EndChild();

        // Command input
        if (ImGui::InputText("Input", input_buf, IM_ARRAYSIZE(input_buf), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory, NULL, NULL))
        {
            
            std::string username = getenv("USERNAME");
            std::transform(username.begin(), username.end(), username.begin(), ::toupper);
            std::string MODIFIEDname = "{" + username + "}";
            std::string input = "[" + MODIFIEDname + "(USER)" + "]: " + """\"" + "%s" + """\"";
            AddLog(input.c_str(), input_buf);

            // Add the input to the logs buffer and reset the input buffer
            memset(input_buf, 0, sizeof(input_buf));
        }

        ImGui::End();
    }

    char input_buf[256];

private:
    std::vector<std::string> logs;
};
