#include "shape.h"


shape::shape()
{

}


shape::~shape()
{
}

void shape::set_start(QPoint s)
{
	start = s;
}

void shape::set_end(QPoint e)
{
	end = e;
}
