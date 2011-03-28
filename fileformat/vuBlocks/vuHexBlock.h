#ifndef HEXBLOCk_H
#define HEXBLOCK_H

#include "config.h"
#include "vuBlock.h"

#ifndef HAVE_NO_CRYPTO
#include "../crypto.h"
#endif

#include <QtCore/QString>
#include <QtCore/QTextStream>


/** This block is for debugging, in case the program encounters
 * some data it so far cannot read, it reports hex data and as
 * many useful information as possible **/
class vuHexBlock : public vuBlock {
	public:
	vuHexBlock(constDataPointer start) :
		vuBlock(start)
	{
		hassignature = false;
	}
	
	virtual int size() const {
		return start.bytesLeft();
	}
	
	virtual QString name() const{
		return "Unknown block";
	}
	virtual void CompleteReport(reporter& o) const {
		o.single("Oops! readesm encountered a chunk of data it cannot read.  Since the length of this chunk is also unknown, it cannot read the remaining file.");
		o("Block offset", QString("%1 (0x%2)")
					.arg(start.offset)
					.arg(start.offset, 8, 16, QChar('0')));
		o("Block size", QString("%1").arg(size()));
		o.single("If you are convinced readesm should be able to read this file, inform the author");
		QString hexDecodeString;
		QTextStream hexDecoder(&hexDecodeString);
		hexout(hexDecoder, start, start.bytesLeft());
		o.single("Block hex data: " + hexDecodeString);
	}
#ifndef HAVE_NO_CRYPTO
	virtual bool checksig(const rsa&){
		return false;
	}
#endif
};


#endif