/* -------------------------------------------------------- *
*
* RepSnapper.cpp
*
* Copyright 2009+ Michael Holm - www.kulitorum.com
*
* This file is part of RepSnapper and is made available under
* the terms of the GNU General Public License, version 2, or at your
* option, any later version, incorporated herein by reference.
*
* ------------------------------------------------------------------------- */
#include "stdafx.h"

//#include <windows.h>													// Header File For The Windows Library
#include <GL/gl.h>														// Header File For The OpenGL32 Library
#include <GL/glu.h>														// Header File For The GLu32 Library

#include "ModelViewController.h"

#include "gcode.h"
#include "UI.h"

GUI *gui;


#include <boost/thread.hpp>

using namespace std;

namespace RepSnapper
{
	class CommandLineOptions
	{
	public:
		CommandLineOptions() :
			// specify defaults here or in the block below
			use_gui(true)
			{}
		bool use_gui;
		string stl_input_path;
		string gcode_output_path;
		string settings_path;
		// add more options above
	};
}

int main(int argc, char *argv[])
{
	// example: Repsnapper.exe --no-gui -i Input.stl -o Output.gcode -s Settings.xml
	RepSnapper::CommandLineOptions options;
	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "--no-gui") == 0)
		{
			options.use_gui = false;
		}
		else if (strcmp(argv[i], "-i") == 0 && i + 1 < argc)
		{
			options.stl_input_path = argv[i+1];
			i++;
		}
		else if (strcmp(argv[i], "-o") == 0 && i + 1 < argc)
		{
			options.gcode_output_path = argv[i+1];
			i++;
		}
		else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc)
		{
			options.settings_path = argv[i+1];
			i++;
		}
	}
	
	gui = new GUI();
	MVC = gui->MVC;
	Fl::visual(FL_DOUBLE|FL_INDEX);

	char WindowTitle[100] = "GCodeView";
	char* W = &WindowTitle[0];
	MVC->gui = gui;
	MVC->serial.setGUI(gui);
	MVC->ProcessControl.gui = gui;
	gui->show(1,&W);

	if (!options.use_gui)
	{
		if (options.stl_input_path.size() > 0)
		{
			MVC->ReadStl(options.stl_input_path);

			if (options.settings_path.size() > 0)
			{
				MVC->ProcessControl.LoadXML(options.settings_path);
				MVC->CopySettingsToGUI();
			}

			MVC->ConvertToGCode();

			if (options.gcode_output_path.size() > 0)
			{
				Fl_Text_Buffer *buffer = gui->GCodeResult->buffer();

				return buffer->savefile(options.gcode_output_path.c_str());
			}
		}
		return 0;
	}
	
	return Fl::run();
}

