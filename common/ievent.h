#ifndef IEVENT_H
#define IEVENT_H

class IEvent
{
public:
	virtual ~IEvent(){}

	virtual bool Wait(size_t miliSecs)=0;
	virtual void Set()=0;
	virtual void Reset()=0;

};

#endif
