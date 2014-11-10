#ifndef JSONREPORTER_H
#define JSONREPORTER_H

#include "Reporter.h"

class JSONReporter : public Reporter {
	Q_DECLARE_TR_FUNCTIONS(JSONReporter)
protected:
	int linkNumber;
	int toggleNumber;
	bool isFirst;
	int indent;
	virtual void subBlock(const Block& value, const QString& tag);
	virtual void arrayStart(int count, const QString& title, bool defaultShown);
	virtual void arrayElemStart(int index);
	virtual void arrayElemEnd(int index);
	virtual void arrayEnd(int count);
public:
	JSONReporter();
	virtual void tagValuePair(const QString& tag, const QString& value);
	virtual void tagValuePair(const QString& tag, int value);
	QByteArray toQByteArray() const;
	virtual bool allowSvg() const;
};


#endif
