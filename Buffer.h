#pragma once

#include <iostream>
#include <vector>
#include <string>

class Buffer
{
public:
    // For UI startup
    void setViewableArea(uint32_t verticalLines, uint32_t horizontalCharacters);

    // Functions for main UI run loop
    void display();
    std::string currentFile() { return m_currentFileName; }

    // Functions for UI menu
    void nextPage();
    void lastPage();
    void openLastFile();
    void openLink(int32_t linkNumber);
    void openFile(std::string fileName);

    // Error Reporting
    void printError(std::ostream &out = std::cout);
    void setUIError(std::string errorMessage);

private:
    std::vector<std::string> m_bufferData;
    int64_t m_topLineNum = 1;
    int16_t m_viewableLines;
    uint32_t m_charsPerLine;

    std::vector<std::string> m_linkFileNames;
    std::vector<std::string> m_history;
    std::string m_currentFileName;

    std::string m_BufferErrorMessage;
    std::string m_UIErrorMessage;
};
