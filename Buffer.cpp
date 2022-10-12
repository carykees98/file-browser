#include <iostream>
#include <fstream>
#include <iomanip>

#include "Buffer.h"

void Buffer::display()
/*
    TODO: rewrite to account for both line max and character max
*/
{
    uint32_t lineCharCount = 0;
    uint64_t lineNumber = m_topLineNum;
    bool fitsInLine;

    std::cout << std::setw(3) << lineNumber << "  "; // Prints first line number
    for (uint32_t i = m_lastPrintedWord; i < m_bufferData.size(); i++)
    {
        fitsInLine = (lineCharCount + m_bufferData[i].size()) <= m_charsPerLine;

        if (!fitsInLine || m_bufferData[i] == "\n")
        {
            if (((lineNumber += 1) - m_topLineNum) < m_viewableLines)
            {
                lineCharCount = 0;
                std::cout << std::endl
                          << std::setw(3) << lineNumber << "  ";
            }
            else
            {
                m_lastPrintedWord = i - 1;
                break;
            }
        }
        else if (fitsInLine)
        {
            std::cout << m_bufferData[i] << " ";
        }
    }
}

void Buffer::nextPage()
{
    m_topLineNum += m_viewableLines;
}

void Buffer::openLastFile()
{
    openFile(m_history[m_history.size() - 1]);
    m_history.erase(m_history.begin() + (m_history.size() - 1));
}

void Buffer::openLink(uint32_t linkNumber)
{
    linkNumber -= 1; // Converts number provided to index for accessing file name in vector
    m_bufferData.clear();
    m_history.push_back(m_currentFileName);
    openFile(m_fileNames[linkNumber]);
}

void Buffer::printError(std::ostream &out)
{
    if (!m_BufferErrorMessage.empty())
    {
        out << "Buffer Error: " + m_BufferErrorMessage << std::endl;
        m_BufferErrorMessage.clear();
    }
    if (!m_UIErrorMessage.empty())
    {
        out << "UI Error: " + m_UIErrorMessage << std::endl;
        m_UIErrorMessage.clear();
    }
}

void Buffer::openFile(std::string fileName)
{
    std::ifstream infile(fileName);
    if (!infile.fail())
    {
        m_currentFileName = fileName;
        std::string currentWord;

        while (infile >> currentWord)
        {
            if (currentWord == "<a")
            {
                std::string fileName;

                infile >> fileName;
                infile >> currentWord;

                currentWord.erase(currentWord.begin() + (currentWord.size() - 1)); // removes '>' from the end of the file name
                m_fileNames.push_back(fileName);
                currentWord = "<" + currentWord + ">[" + std::to_string(m_fileNames.size()) + "]";
            }
            else if (currentWord == "<br>")
            {
                currentWord = "\n";
            }
            else if (currentWord == "<p>")
            {
                if (m_bufferData[m_bufferData.size() - 1] != "\n") 
                // checks to make sure there isn't a newline character before the <p> tag
                {
                    m_bufferData.push_back("\n");
                }
                m_bufferData.push_back("\n");
                continue;
            }
            m_bufferData.push_back(currentWord);

            if (infile.get() == '\n')
            {
                m_bufferData.push_back("\n");
            }
            if (infile.peek() == static_cast<char>(9)) // Tab Character
            {
                m_bufferData.push_back("	");
            }
        }
    }
    else
    {
        m_BufferErrorMessage = "File: " + fileName + " failed to open.";
        return;
    }
    infile.close();
}

void Buffer::setUIError(std::string errorMessage)
{
    m_UIErrorMessage = errorMessage;
}

void Buffer::setViewableArea(uint32_t verticalLines, uint32_t horizontalCharacters)
{
    m_viewableLines = verticalLines;
    m_charsPerLine = horizontalCharacters;
}
