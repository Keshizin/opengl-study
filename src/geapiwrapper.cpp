#include <geapiwrapper.h>

GEEventHandler *GEApiWrapper::eventHandler;

GEApiWrapper::GEApiWrapper()
{
}

void GEApiWrapper::setEventHandler(GEEventHandler *eventHandler)
{
	this->eventHandler = eventHandler;
}

GEEventHandler * GEApiWrapper::getEventHandler()
{
	return eventHandler;
}