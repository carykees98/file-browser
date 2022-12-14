#pragma once

#include "Buffer.h"

class UI
{
public:
    void run();
private:
    Buffer m_buffer;
    void display();
    void execute(char selection, bool &done);
    int m_vertical_lines, m_horizontal_lines;
};