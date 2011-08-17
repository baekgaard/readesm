#include "VuActivities.h"


VuActivities::VuActivities(const constDataPointer& filewalker) : RawVuActivities(filewalker)
{}

QString VuActivities::name() const {
	return tr("Activities on %1").arg(timeReal.date().toString());
}
