#pragma once

#include <cstdint>
#include <string_view>
#include <vector>

class WindowClass
{
public:
    using FileContent = std::vector<std::string>;

public:
    WindowClass()
        : filePath1("text1.txt"), filePath2("text2.txt"), fileContent1({}),
          fileContent2({}), textContent1(""), textContent2(""), diffResult1({}),
          diffResult2({}) {};

    void Draw(std::string_view label);

private:
    void DrawSelections();
    void DrawInputText();
    void DrawDiffView();
    void DrawStats();

    FileContent LoadFileContent(std::string_view file_path);
    void SaveFileContent(std::string_view file_path, FileContent &file_content);
    void CreateDiff();

private:
    std::string filePath1;
    std::string filePath2;

    FileContent fileContent1;
    FileContent fileContent2;

    std::string textContent1;
    std::string textContent2;

    FileContent diffResult1;
    FileContent diffResult2;
};

void render(WindowClass &window_obj);
