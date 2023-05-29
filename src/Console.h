#include <imgui.h>
#include <vector>
#include <string>
#include <algorithm>
#ifndef CONSOLE_H
#define CONSOLE_H
class Console
{
public:
    Console()
    {
        memset(input_buf, 0, sizeof(input_buf));
    }

    void log(const char* fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        char buf[1024];
        vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
        buf[IM_ARRAYSIZE(buf) - 1] = 0;
        va_end(args);

        logs.push_back(buf);
    }

    template<typename T>
    void log(const T& value)
    {
        std::string str = std::to_string(value);
        log(str.c_str());
    }

    void Draw(bool no_resize, bool no_move)
    {
        if (!ImGui::Begin("Console", 0, (no_resize ? ImGuiWindowFlags_NoResize : 0) | (no_move ? ImGuiWindowFlags_NoMove : 0)))
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



        if (ImGui::InputText("Input", input_buf, IM_ARRAYSIZE(input_buf), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory, NULL, NULL))
        {
            char* username;
            size_t len;
            errno_t err = _dupenv_s(&username, &len, "USERNAME");
            if (err == 0 && username != NULL)
            {
                std::string username_str(username);
                free(username);
                std::transform(username_str.begin(), username_str.end(), username_str.begin(), ::toupper);
                std::string MODIFIEDname = "{" + username_str + "}";
                std::string input = "[" + MODIFIEDname + "(USER)" + "]: " + """\"" + "%s" + """\"";
                log(input.c_str(), input_buf);

                memset(input_buf, 0, sizeof(input_buf));
            }
        }


        ImGui::End();
    }

    void CLEAR_CONSOLE()
    {
        logs.clear();
    }

    char input_buf[256];

private:
    std::vector<std::string> logs;
};
#endif // CONSOLE_H