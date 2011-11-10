/** \file readesm.cpp
 \brief contains main() and handling of command line arguments.

 readesm mainly defines which files to read and parse. The actual parsing
 of files is described in DataDefinitions.xml and the information about the parsing of
 the individual blocks just there.
 */


#include "config.h"
#include "fileformat/EsmFile.h"
#include "fileformat/Reporter/HtmlReporter.h"
#include "gui/mainWindow.h"
#include "OptionParser.h"

#include <QtCore/QByteArray>
#include <QtCore/QFile>
#include <QtCore/QLocale>
#include <QtCore/QString>
#include <QtCore/QTextStream>
#include <QtCore/QTranslator>
#include <QtGui/QApplication>

int main(int argc, char** argv)
{
	QTextStream cerr(stderr);
	QTextStream cout(stdout);

	OptionParser parser;
	parser.addOption("format","f","Format used for output");
	parser.addOption("locale","l","Language used for output");
	parser.parseArgs(argc,argv);
	if(parser.checkFlag("help")){
		cout << "Readesm " << QString("%1 (%2)")
			.arg(VERSION)
			.arg(VERSION_DATE);
		cout << "\n\n Readesm takes an ESM file, as downloaded from a digital tachograph or a driver card, a compulsory equipment for trucks heavier than 3.5 tons within the European Union and converts it into human-readable form.\n\n";
		cout << "usage: Readesm [OPTIONS] [INPUT FILE [OUTPUT FILE]]\n\n";
		cout << parser.helpText();
		cout << "\n\n Being a QT (http://qt.nokia.com/) application, you can also pass qt command line parameters to Readesm.";
		cout << "\n\n  This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.\n\n This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.\n";
		return 0;
	}
	bool gui = true;
	if(parser.argumentCount() >= 2) gui = false;

	QApplication app(argc, argv, gui);
	app.setApplicationName("Readesm");
	app.setApplicationVersion(QString("%1 (%2)")
			.arg(VERSION)
			.arg(VERSION_DATE));
	app.setOrganizationName(QString::fromUtf8("Andreas Gölzer"));
	app.setOrganizationDomain("http://andreas.goelzer.de");

	QTranslator qTranslator;
	QString locale = parser.get("locale", QLocale::system().name());
	if(locale != "C"){
		QString trans = "readesm_" + locale;
		if(!qTranslator.load(trans) && !qTranslator.load(trans, QCoreApplication::applicationDirPath() + "/../translations") && !qTranslator.load(trans, PREFIX "share/readesm/translations")) {
			cerr << "Could not load internationalization files for your locale :(" << endl;
		}
		app.installTranslator(&qTranslator);
	}
	
	if(!gui){
		QString inputFile(parser.getArgument(0));
		QString outputFile(parser.getArgument(1));
		bool converted = false;

		EsmFile esm(inputFile);
		QByteArray outData;

		//determine extension of output file
		QString extension(outputFile.section(".",-1).toLower());
		if(parser.get("format") == "html" || extension == "html" || extension == "htm" || extension == "xhtml") {
			HtmlReporter rep;
			rep << esm;
			outData = rep.toQByteArray();
			converted = true;
		}
		
		if(!converted) {
			cerr << "Format for output not recognized." << endl;
			return 1;
		}
		
		if(outputFile != "-"){
			QFile out(outputFile);
			out.open(QIODevice::WriteOnly | QIODevice::Text);

			if(!out.isOpen()) {
				cerr << "Could not open file " << outputFile << " for output. dumping to stdout" << endl;
				cout << outData;
				return 1;
			} else out.write(outData);
		} else {
			cout << outData;
		}
	} else if(parser.argumentCount() < 2){
		mainWindow widgetMainWindow;
		widgetMainWindow.show();
		if(parser.argumentCount() == 1) widgetMainWindow.openFile(parser.getArgument(0));
		return app.exec();
	} else {
		cerr << "too many arguments to readesm";
		return 1;
	}
}
