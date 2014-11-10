//TODO
// -- clean up and ensure tags and values are written correctly with no wrong characters
// -- should we somehow change the "Activities on ..." to a better heading
// -- better data formats (somewhere)?
// -- print out integers better?
// -- reformat timesheet

#include "JSONReporter.h"

#include "config.h"

#include "../TopLevelBlock.h"
#include "../DataTypes/DataType.h"
#include "../loadFile.h"

#include <QtCore/QTextCodec>

#define TABLEVEL 2

JSONReporter::JSONReporter() : Reporter(), isFirst(true), indent(1)
{
	collector << "{";
	sss = true;
}

void JSONReporter::tagValuePair(const QString& tag, const QString& value){
	QString copy(value);
	copy.replace("\"","\\\"");
	copy.replace('\0',' ');
	collector << (!isFirst?",":"") << QString("\n%1").arg(" ",indent*TABLEVEL);
	collector << "\"" << tag << "\" : \"" << copy << "\"";
	isFirst = false;
}

void JSONReporter::tagValuePair(const QString& tag, int value){
	collector << (!isFirst?",":"") << QString("\n%1").arg(" ",indent*TABLEVEL);
	collector << "\"" << tag << "\" : " << QString("%1").arg(value) ;
	isFirst = false;
}

void JSONReporter::subBlock(const Block& value, const QString& tag){
	if(nestLevel == 1){
		collector << (!isFirst?",":"") << QString("\n%1\"").arg(" ",indent*TABLEVEL) << tag << "\" : {";
		indent = 2;
		isFirst = true;
		*this << value;
		indent = 1;
		collector << QString("\n%1}").arg(" ",indent*TABLEVEL);
		isFirst = false;
	} else {
		if(value.toString() != ""){
			collector << (!isFirst?",":"") << QString("\n%1").arg(" ",indent*TABLEVEL);
			isFirst = false;
			if(tag == "") collector << "\"_summary\" : " << "\"" << value.toString().replace("\"","\\\"") << "\"";
			else {
				indent++;
				collector << QString("\"%1\" : {").arg(tag);
				collector << QString("\n%1").arg(" ",(indent)*TABLEVEL);
				collector << QString("\"_summary\" : \"%1\"").arg(value.toString().replace("\"","\\\""));
				isFirst = false;
			}
			*this << value;
			if(tag != "") {
				indent--;
				collector << QString("\n%1}").arg(" ",indent*TABLEVEL);
				isFirst = false;
			}
		} else {
			if(tag == "") {
				// collector << "*";
				// indent++;
				*this << value;
				// indent--;
				// collector << "*";
			} else {
				collector << (!isFirst?",":"") << QString("\n%1\"").arg(" ",indent*TABLEVEL) << tag << "\" : {";
				indent++;
				isFirst = true;
				*this << value;
				indent--;
				collector << QString("\n%1}").arg(" ",indent*TABLEVEL);
				isFirst = false;
			}
		}
	}
}

QByteArray JSONReporter::toQByteArray() const{
	collector << "\n}\n";
	collector.flush();
	if(collected.size() == 0) return QByteArray();
	return collected;
}

bool JSONReporter::allowSvg() const{
	return false;
}

void JSONReporter::arrayStart(int count, const QString& title, bool defaultShown){
	if(count > 0){
		collector << (!isFirst?",":"") << QString("\n%1").arg(" ",indent*TABLEVEL);
		collector << QString("\"%2\" : {\n%3\"_len\" : %4,\n%5\"_data\" : [").arg(title).arg(" ",(indent+1)*TABLEVEL).arg(count).arg(" ",(indent+1)*TABLEVEL);
		isFirst = true;
	} else {
		// collector << "null" ;
		// collector << "[]";
		// isFirst = false;
	}
	indent+=1;
}
void JSONReporter::arrayElemStart(int index){
	collector << (!isFirst?",":"");
	collector << QString("\n%1{").arg(" ",(indent+1)*TABLEVEL);
	indent+=2;
	isFirst = true;
}
void JSONReporter::arrayElemEnd(int index){
	collector << QString("\n%1}").arg(" ",(indent-1)*TABLEVEL);
	isFirst = false;
	indent-=2;
}
void JSONReporter::arrayEnd(int count){
	indent-=1;
	if(count > 0) {
		collector << QString("\n%1]\n%2}").arg(" ",(indent+1)*TABLEVEL).arg(" ",indent*TABLEVEL);
		isFirst = false;
	}
}
