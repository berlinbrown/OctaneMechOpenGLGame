#include "text.hpp"
#include "fireants.hpp"

float framerate = 0.0f;
int keyCodes[20] = {0};

TextBoxPtr InitTextBox(int, int, int, int) { return nullptr; }
void DestroyTextBox(TextBoxPtr) {}
void SetTextMode(TextBoxPtr, unsigned int) {}
void SetTextColor(TextBoxPtr, unsigned char, unsigned char, unsigned char) {}
void FormatStrCat(TextBoxPtr, char*) {}
void DrawText(TextBoxPtr) {}
void TextBegin(TextBoxPtr) {}
void TextEnd(void) {}
void DrawString(TextBoxPtr, int, int, char*) {}
void Printf(TextBoxPtr, char*, ...) {}
void PrintText(const char*, ...) {}
void Reset_FontID(void) {}
