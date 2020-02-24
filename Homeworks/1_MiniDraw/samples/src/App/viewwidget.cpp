#include "viewwidget.h"
#include <QMouseEvent>
#include <qpainter.h>
#include "qcolordialog.h"
#include "qinputdialog.h"

ViewWidget::ViewWidget(QWidget* parent)
	: QWidget(parent) {
	ui.setupUi(this);
	
	isDrawing = false;
	current_figure = nullptr;
	dM_backup = DrawMode::kDefault;
	drawMode = DrawMode::kDefault;
	pointHovered = nullptr;
	canDrag = true;
	setMouseTracking(true);
	linePen.setWidth(5);
	linePen.setColor(QColor(200,200,200));
}

ViewWidget::~ViewWidget() {
	for (auto figure : figureVec)
		delete figure;

	// 每次把 current_figure push to figureVec 后都将其设置为 nullptr
	// 因此下面的 delete 和上面的 delete 不会处理同一指针
	delete current_figure;
}

void ViewWidget::mousePressEvent(QMouseEvent* mouseEvent) {
	if (drawMode == DrawMode::kDrag)
		return;

	QPoint pos = mouseEvent->pos();

	// drag point
	if (isDrawing == false && drawMode != DrawMode::kDrag) {
		isDragAll = Qt::LeftButton == mouseEvent->button() ? false : true;
		for (size_t i = 0; i < figureVec.size(); i++) {
			dragPoints = figureVec[i]->getPoint(pos, isDragAll);
			if (dragPoints.size() != 0) {
				dM_backup = drawMode;
				drawMode = DrawMode::kDrag;
				isDrawing = true;
				return;
			}
		}
	}

	if (Qt::LeftButton == mouseEvent->button()) {
		// start draw

		isDrawing = true;
		switch (drawMode) {
		case DrawMode::kDefault:
			// 处于初始状态
			isDrawing = false;
			break;
		case DrawMode::kLine:
			current_figure = new Line(pos, pos);
			current_figure->setCanDrag(canDrag);
			current_figure->setPen(&linePen, PEN::LINE);
			break;
		case DrawMode::kRectangle:
			current_figure = new Rect(pos, pos);
			current_figure->setCanDrag(canDrag);
			current_figure->setPen(&linePen, PEN::LINE);
			break;
		case DrawMode::kEllipse:
			current_figure = new Ellipse(pos, pos);
			current_figure->setCanDrag(canDrag);
			current_figure->setPen(&linePen, PEN::LINE);
			break;
		case DrawMode::kPolygon:
			if (current_figure == nullptr) {
				Polygon* poly = new Polygon(pos);
				poly->AddPoint(false);
				current_figure = poly;
				current_figure->setCanDrag(canDrag);
				current_figure->setPen(&linePen, PEN::LINE);
				// 保持鼠标追踪
				// 无论鼠标是否有键按下，都会持续触发 mouseMoveEvent
				setMouseTracking(true);
			}
			break;
		case DrawMode::kFreehand:
			current_figure = new Freehand(pos);
			current_figure->setCanDrag(canDrag);
			current_figure->setPen(&linePen, PEN::LINE);
			break;
		case DrawMode::kBezierCurve:
			if (current_figure == nullptr) {
				BezierCurve* bc = new BezierCurve(pos);
				bc->AddPoint(false);
				current_figure = bc;
				current_figure->setCanDrag(canDrag);
				current_figure->setPen(&linePen, PEN::LINE);
				// 保持鼠标追踪
				// 无论鼠标是否有键按下，都会持续触发 mouseMoveEvent
				setMouseTracking(true);
			}
			break;
		default:
			break;
		}
	}
}

void ViewWidget::mouseMoveEvent(QMouseEvent* mouseEvent) {
	if (canDrag) {
		// delete circle around hovered point
		if (pointHovered) {
			delete figureVec.back();
			figureVec.pop_back();
		}

		// draw circle around hovered point
		QPoint pos = mouseEvent->pos();
		for (size_t i = 0; i < figureVec.size(); i++) {
			std::vector<QPoint*> point = figureVec[i]->getPoint(pos, false);
			pointHovered = point.size() == 1 ? point[0] : nullptr;
			if (pointHovered != nullptr) {
				figureVec.push_back(new Ellipse(pos - QPoint(20, 20), pos + QPoint(20, 20)));
				QPen circlePen;
				circlePen.setWidth(2);
				circlePen.setColor(QColor(0x22, 0xB1, 0x4C));
				figureVec.back()->setPen(&circlePen, PEN::LINE);
				figureVec.back()->setCanDrag(false);
				break;
			}
		}
	}

	if (!isDrawing)
		return;

	QPoint end_point = mouseEvent->pos();

	if(drawMode != DrawMode::kDrag)
		current_figure->update(end_point);
	
	// update drag point
	if (drawMode == DrawMode::kDrag) {
		QPoint diff = end_point - *dragPoints[0];
		for (size_t i = 0; i < dragPoints.size(); i++) {
			dragPoints[i]->setX(dragPoints[i]->x() + diff.x());
			dragPoints[i]->setY(dragPoints[i]->y() + diff.y());
		}
	}

}

void ViewWidget::mouseReleaseEvent(QMouseEvent* mouseEvent) {
	if (!isDrawing)
		return;

	if (drawMode == DrawMode::kPolygon || drawMode == DrawMode::kBezierCurve) {
		Polygon* poly = dynamic_cast<Polygon*>(current_figure);
		if (Qt::LeftButton == mouseEvent->button())
			poly->AddPoint(false);// 新增一个点
		if (Qt::RightButton == mouseEvent->button()) {
			isDrawing = false;
			poly->AddPoint(true);// 绘制完成
			figureVec.push_back(current_figure);
			current_figure = nullptr;
			setMouseTracking(canDrag);
		}
	}
	else if (drawMode == DrawMode::kDrag) {
		if (isDragAll) {
			if (Qt::RightButton != mouseEvent->button())
				return;
		}
		else {
			if (Qt::LeftButton != mouseEvent->button())
				return;
		}

		isDrawing = false;
		dragPoints.clear();
		drawMode = dM_backup;
		setMouseTracking(canDrag);
	} 
	else {
		if (Qt::LeftButton != mouseEvent->button())
			return;
		isDrawing = false;
		figureVec.push_back(current_figure);
		current_figure = nullptr;
	}
}

void ViewWidget::paintEvent(QPaintEvent*)
{
	QPainter painter;
	painter.begin(this);
	painter.setPen(linePen);

	for (size_t i = 0; i < figureVec.size(); i++)
		figureVec[i]->Draw(painter);
	if (current_figure != nullptr)
		current_figure->Draw(painter);

	painter.end();
	update();
}

void ViewWidget::set_drawMode_to_line() {
	if (isDrawing)
		return;

	drawMode = DrawMode::kLine;
}

void ViewWidget::set_drawMode_to_ellipse() {
	if (isDrawing)
		return;

	drawMode = DrawMode::kEllipse;
}

void ViewWidget::set_drawMode_to_rectangle() {
	if (isDrawing)
		return;

	drawMode = DrawMode::kRectangle;
}

void ViewWidget::set_drawMode_to_polygon() {
	if (isDrawing)
		return;

	drawMode = DrawMode::kPolygon;
}

void ViewWidget::set_drawMode_to_freehand() {
	if (isDrawing)
		return;

	drawMode = DrawMode::kFreehand;
}

void ViewWidget::set_drawMode_to_bezierCurve() {
	if (isDrawing)
		return;

	drawMode = DrawMode::kBezierCurve;
}

void ViewWidget::undo() {
	if (isDrawing == true)
		return;

	if (figureVec.size() > 0) {
		delete figureVec.back();
		figureVec.pop_back();
	}
}

void ViewWidget::switch_drag() {
	if (isDrawing)
		return;

	// delete circle around hovered point
	if (pointHovered) {
		delete figureVec.back();
		figureVec.pop_back();
		pointHovered = nullptr;
	}

	canDrag = !canDrag;
	setMouseTracking(canDrag);
	for (size_t i = 0; i < figureVec.size(); i++)
		figureVec[i]->setCanDrag(canDrag);
}

void ViewWidget::set_color() {
	if (isDrawing)
		return;

	QColor color = QColorDialog::getColor(Qt::white, this, tr("Set color of pen"));
	linePen.setColor(color);
}

void ViewWidget::set_width() {
	if (isDrawing)
		return;

	bool ok = false;
	int width = QInputDialog::getInt(nullptr, tr("Set width of pen"), tr("width : 1 - 10"), 1, 1, 10, 1, &ok);
	if (ok)
		linePen.setWidth(width);
}
