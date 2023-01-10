#include "figure.h"

Figure::Figure() {
	size = 0;
	capacity = 10;
	ps = new QPoint[capacity];
	type = kDefault;
}

Figure::Figure(Type t) {
	size = 0;
	capacity = 10;
	ps = new QPoint[capacity];
	type = t;
}

Figure::~Figure () {
	delete[] ps;
}

void Figure::set_start(QPoint s) {
	start = s;
}

void Figure::set_end(QPoint e) {//末点可能是动态变化的，所以要将点集中最后一个元素及时的修改
	end = e;
	ps[size - 1] = e;
}
QPoint Figure::get_start_point() {
	return start;
}

QPoint Figure::get_end_point() {
	return end;
}
//push_point 函数是用来存储鼠标确认过的点集，方便多边形与Freehand之后的画图
void Figure::push_point(QPoint p) {
	if (size < capacity-1) {
		ps[size] = p;
		size++;
	}
	else {//用来处理动态增长的过程，后续可以将数据结构调整成链表类型 TODO
		QPoint* tmp = new QPoint[2 * capacity];
		for (int i = 0; i < size; i++) {
			tmp[i] = ps[i];
		}
		tmp[size] = p;
		size++;
		delete[] ps;
		ps = tmp;
		capacity *= 2;
	}
	
	
	/*int s = size;
	std::cout << "size: " << s << std::endl;
	for (int i = 0; i < s; i++)
		qDebug() << ps[i];
	std::cout<< std::endl;*/
}

void Figure::set_type(Type t) {
	type = t;
}
