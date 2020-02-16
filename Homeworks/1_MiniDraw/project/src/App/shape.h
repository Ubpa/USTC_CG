#pragma once
#include <QtGui> 
class shape
{
public:
	shape();
	virtual ~shape();
	virtual void Draw(QPainter &paint)=0;	void set_start(QPoint s);	void set_end(QPoint e);

public:
	enum Type
	{
		kDefault = 0,
		kLine=1,
		kRect=2,
	};
	
protected:
	QPoint start;
	QPoint end;
	
};

