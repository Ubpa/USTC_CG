#include "Picker.h"

#include <UI/Attribute.h>

#include <Engine/Viewer/Viewer.h>
#include <Engine/Scene/Scene.h>
#include <Engine/Scene/SObj.h>
#include <Engine/Intersector/ClosestIntersector.h>
#include <Engine/Viewer/Ray.h>
#include <Engine/Viewer/Roamer.h>
#include <Engine/Scene/CmptGeometry.h>
#include <Engine/Primitive/TriMesh.h>

#include <Qt/RawAPI_OGLW.h>

#include <OpenGL/Camera.h>

#include <Basic/Op/LambdaOp.h>
#include <Basic/EventManager.h>

#include <QtWidgets/QApplication>

#include <deque>

using namespace Ubpa;

using namespace std;

Picker::Picker(Viewer * viewer)
	: viewer(viewer) { }

void Picker::Init() {
	cout << "WARNING::Picker::Init:" << endl
		<< "\t" << "not implemented" << endl;
}
