#include "text.hpp"
#include "gldrawlib.hpp"
#include "fireants.hpp"

#include <GLUT/glut.h>

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>

float framerate = 0.0f;
int keyCodes[20] = {0};

static void RenderGlyphString(const char* str)
{
	if (!str) return;

	for (const unsigned char* cursor = reinterpret_cast<const unsigned char*>(str); *cursor; ++cursor)
	{
		if (*cursor == '\n')
		{
			continue;
		}
		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *cursor);
	}
}

TextBoxPtr InitTextBox(int inL, int inR, int inT, int inB)
{
	TextBoxPtr text = static_cast<TextBoxPtr>(std::calloc(1, sizeof(TextBox)));
	if (!text) return nullptr;

	text->boxL = inL;
	text->boxR = inR;
	text->boxT = inT;
	text->boxB = inB;
	text->buffer = static_cast<char*>(std::calloc(MAX_STR, sizeof(char)));
	text->drawBufferPtr = text->buffer;
	text->mode = TEXT_NONE;
	SET_COLOR(text->color, 255, 255, 255, 255);
	return text;
}

void DestroyTextBox(TextBoxPtr text)
{
	if (!text) return;

	std::free(text->buffer);
	std::free(text);
}

void SetTextMode(TextBoxPtr text, unsigned int modeIn)
{
	if (!text) return;
	text->mode = modeIn;
}

void SetTextColor(TextBoxPtr text, unsigned char r, unsigned char g, unsigned char b)
{
	if (!text) return;
	SET_COLOR(text->color, r, g, b, 255);
}

void FormatStrCat(TextBoxPtr text, char* str)
{
	if (!text || !text->buffer || !str) return;

	const std::size_t current_len = std::strlen(text->buffer);
	if (current_len >= MAX_STR - 1) return;

	std::strncat(text->buffer, str, MAX_STR - current_len - 1);
}

void TextBegin(TextBoxPtr)
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0, -1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT | GL_TEXTURE_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void TextEnd(void)
{
	glPopAttrib();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void DrawString(TextBoxPtr text, int x, int y, char* str)
{
	if (!text || !str) return;

	glColor3ub(text->color.r, text->color.g, text->color.b);
	glRasterPos2i(x, y);

	for (const char* cursor = str; *cursor; ++cursor)
	{
		if (*cursor == '\n')
		{
			y += 14;
			glRasterPos2i(x, y);
			continue;
		}

		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, static_cast<unsigned char>(*cursor));
	}
}

void DrawBoundingBox(TextBoxPtr text)
{
	if (!text) return;

	glBegin(GL_LINE_LOOP);
	glVertex2i(text->boxL, text->boxT);
	glVertex2i(text->boxR, text->boxT);
	glVertex2i(text->boxR, text->boxB);
	glVertex2i(text->boxL, text->boxB);
	glEnd();
}

void DrawText(TextBoxPtr text)
{
	if (!text || !text->buffer) return;

	glColor3ub(text->color.r, text->color.g, text->color.b);

	int cursor_y = text->boxT;
	char line[MAX_STR];
	const char* source = text->buffer;

	while (*source)
	{
		std::size_t len = 0;
		while (source[len] && source[len] != '\n' && len < sizeof(line) - 1)
		{
			line[len] = source[len];
			++len;
		}
		line[len] = '\0';

		glRasterPos2i(text->boxL, cursor_y);
		RenderGlyphString(line);

		if (!source[len]) break;
		source += len + 1;
		cursor_y += 14;
	}
}

void Printf(TextBoxPtr text, char* fmt, ...)
{
	if (!text || !text->buffer || !fmt) return;

	char buff[MAX_STR];
	va_list vlist;
	va_start(vlist, fmt);
	std::vsnprintf(buff, sizeof(buff), fmt, vlist);
	va_end(vlist);

	FormatStrCat(text, buff);
}

void PrintText(const char* fmt, ...)
{
	if (!fmt) return;

	char buff[MAX_STR];
	va_list vlist;
	va_start(vlist, fmt);
	std::vsnprintf(buff, sizeof(buff), fmt, vlist);
	va_end(vlist);

	RenderGlyphString(buff);
}

void Reset_FontID(void) {}
