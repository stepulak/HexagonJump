#include "OpenFileDialog.hpp"

#ifdef _WIN32
#include <Windows.h>
#endif

namespace hexagon {

#ifdef _WIN32

std::string RunOpenFileDialog()
{
	char buffer[512];
	OPENFILENAME dialog;
	std::memset(&dialog, 0, sizeof(dialog));
	dialog.lStructSize = sizeof(dialog);
	dialog.hwndOwner = nullptr;
	dialog.lpstrFile = buffer;
	dialog.lpstrFile[0] = '\0';
	dialog.nMaxFile = sizeof(buffer);
	dialog.lpstrFilter = "Supported Music Files(*.ogg, *wav)\0 *.ogg; *.wav\0";
	dialog.nFilterIndex = 1;
	dialog.lpstrFileTitle = NULL;
	dialog.nMaxFileTitle = 0;
	dialog.lpstrInitialDir = NULL;
	dialog.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	GetOpenFileName(&dialog);
	return buffer;
}

#else
	// TODO Linux solution
#endif

}