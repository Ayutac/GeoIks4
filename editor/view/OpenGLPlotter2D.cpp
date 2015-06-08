#include "precompiledHeaders.h"
#include "OpenGLPlotter2D.h"
#include "GeoXWorkSpace.h"
#include "NamedObjectList.h"
#include "DynamicArrayOfStructures.hpp"
#include "NumericalClassProperty.h"
#include "StringClassProperty.h"
#include "BooleanClassProperty.h"
#include "ClassMethods.h"
#include "OpenGLCanvas.h"
#include "OpenGLCanvas2D.h"
#include "NumericalClassProperty.h"
#include "StringClassProperty.h"
#include "FixedArrayClassProperty.h"
#include "PanZoomCameraController2D.h"
#include <QGLWidget>
#include <GL/gl.h>
#include <GL/glu.h>
#include "GeoXOutput.h"


const Vector4f GLPlotterCurveStyle::COLOR_MATERIAL_RED         = makeVector4f(229,  28,  35, 255) / 255.0f;
const Vector4f GLPlotterCurveStyle::COLOR_MATERIAL_ORANGE      = makeVector4f(255, 152,   0, 255) / 255.0f;
const Vector4f GLPlotterCurveStyle::COLOR_MATERIAL_YELLOW      = makeVector4f(255, 235,  59, 255) / 255.0f;
const Vector4f GLPlotterCurveStyle::COLOR_MATERIAL_GREEN       = makeVector4f(139, 195,  74, 255) / 255.0f;
const Vector4f GLPlotterCurveStyle::COLOR_MATERIAL_DARK_BLUE   = makeVector4f( 63,  81, 181, 255) / 255.0f;
const Vector4f GLPlotterCurveStyle::COLOR_MATERIAL_LIGHT_BLUE  = makeVector4f(104, 137, 255, 255) / 255.0f;
const Vector4f GLPlotterCurveStyle::COLOR_MATERIAL_VIOLET      = makeVector4f(103,  58, 183, 255) / 255.0f;
const Vector4f GLPlotterCurveStyle::COLOR_MATERIAL_PINK        = makeVector4f(233,  30,  99, 255) / 255.0f;
const Vector4f GLPlotterCurveStyle::COLOR_MATERIAL_GRAY        = makeVector4f(128, 128, 128, 255) / 255.0f;

const Vector4f GLPlotterCurveStyle::COLOR_DEFAULT_RED          = makeVector4f(208,   0,   0, 255) / 255.0f;
const Vector4f GLPlotterCurveStyle::COLOR_DEFAULT_DARK_YELLOW  = makeVector4f(242, 196,   0, 255) / 255.0f;
const Vector4f GLPlotterCurveStyle::COLOR_DEFAULT_GREEN        = makeVector4f(  0, 152,   0, 255) / 255.0f;
const Vector4f GLPlotterCurveStyle::COLOR_DEFAULT_BLUE         = makeVector4f( 32,  30, 125, 255) / 255.0f;
const Vector4f GLPlotterCurveStyle::COLOR_DEFAULT_GRAY         = makeVector4f(128, 128, 128, 255) / 255.0f;


IMPLEMENT_GEOX_CLASS(GLPlotterCurveSelection, 0)
{
	BEGIN_CLASS_INIT(GLPlotterCurveSelection)
	ADD_STRING_PROP(yAxisChannelName, 0)
	ADD_STRING_PROP(xAxisChannelName, 0)
	ADD_CARD32_PROP(yAxisIndex, 0)
	ADD_CARD32_PROP(xAxisIndex, 0)
}

IMPLEMENT_GEOX_CLASS(GLPlotterCurveStyle, 0)
{
	BEGIN_CLASS_INIT(GLPlotterCurveStyle)
	ADD_VECTOR4F_PROP(color, 0)
	ADD_FLOAT32_PROP(markerSize, 0)
	ADD_FLOAT32_PROP(lineThickness, 0)
}

static string describeType(card32 i) {
	if (i==0) return "TYPE_XY_CURVE";
	if (i==1) return "TYPE_FUNC_CURVE";
	if (i==2) return "TYPE_BAR_DIAGRAM";
	return "undefined";
}

IMPLEMENT_GEOX_CLASS(OpenGLPlotter2D, 2)
{
   BEGIN_CLASS_INIT(OpenGLPlotter2D)
	ADD_BOOLEAN_PROP(drawAxes, 0)
	ADD_BOOLEAN_PROP(drawAxesLabels, 0)
	ADD_BOOLEAN_PROP(drawLegend, 2)
	ADD_CARD32_PROP(fontSize, 2)
	ADD_BOOLEAN_PROP(autoZoom, 0)

	ADD_FLOAT32_PROP(minX, 0)
	ADD_FLOAT32_PROP(maxX, 0)
	ADD_FLOAT32_PROP(minY, 0)
	ADD_FLOAT32_PROP(maxY, 0)

	ADD_CARD32_PROP_UPDATE_DESCR(type, 0, NULL, describeType, 3)

	ADD_OBJECT_LIST_PROP(OpenGLPlotter2D, styles, 0, GLPlotterCurveStyle::getClass())
	ADD_OBJECT_LIST_PROP(OpenGLPlotter2D, curves, 1, GLPlotterCurveSelection::getClass())

	ADD_NOARGS_METHOD(OpenGLPlotter2D::setDefaultStyles)
	ADD_NOARGS_METHOD(OpenGLPlotter2D::setMaterialStyles)
}


OpenGLPlotter2D::OpenGLPlotter2D()
{
	drawAxes = true;
	drawAxesLabels = true;
	drawLegend = true;
	fontSize = 12;
	autoZoom = true;

	minX = -1;
	maxX =  1;
	minY = -1;
	maxY =  1;

	type = TYPE_FUNC_CURVE;

	styles.push_back( new GLPlotterCurveStyle(GLPlotterCurveStyle::COLOR_DEFAULT_RED,         5, 2) );
	styles.push_back( new GLPlotterCurveStyle(GLPlotterCurveStyle::COLOR_DEFAULT_DARK_YELLOW, 5, 2) );
	styles.push_back( new GLPlotterCurveStyle(GLPlotterCurveStyle::COLOR_DEFAULT_GREEN,       5, 2) );
	styles.push_back( new GLPlotterCurveStyle(GLPlotterCurveStyle::COLOR_DEFAULT_BLUE,        5, 2) );
	styles.push_back( new GLPlotterCurveStyle(GLPlotterCurveStyle::COLOR_DEFAULT_GRAY,        5, 2) );

}

void OpenGLPlotter2D::addCurve(string yAxisChannelName, card32 yAxisChannelIndex, string xAxisChannelName, card32 xAxisChannelIndex)
{
	curves.push_back( new GLPlotterCurveSelection(yAxisChannelName, xAxisChannelName, yAxisChannelIndex, xAxisChannelIndex) );
}

static void computeAxis(const float32 minV, const float32 maxV,
								float32 &aStartV, float32 &aStepV, card32 &aNumV, float32 logShift)
{
	const float32 scaleV = maxV-minV;
	if (scaleV <= 0) {
		aStartV = 0;
		aStepV = 0;
		aNumV = 0;
	} else {
		aStepV = pow(10, floor(log10(scaleV))-logShift);
		aStartV = floor(minV/aStepV)*aStepV;
		aNumV = (card32)floor((maxV - aStartV) / aStepV);
		if (aNumV > 1000) aNumV = 1000;
	}
}

void OpenGLPlotter2D::renderGL(QGLWidget *glWidget)
{
	bool curveType = false;
	if (type == TYPE_XY_CURVE || type == TYPE_FUNC_CURVE) {
		curveType = true;
	}
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POINT_SMOOTH);
	if (drawAxes) {
		glColor4f(0,0,0,1);
		glBegin(GL_LINE_STRIP);
		glVertex2f(-1,-1);
		glVertex2f(-1, 1);
		glVertex2f( 1, 1);
		glVertex2f( 1,-1);
		glVertex2f(-1,-1);
		glEnd();
	}
	string dataName = getData();
	Persistent *dataObj = getContext()->getData()->getObject(dataName);
	DynamicArrayOfStructures *aos = dynamic_cast<DynamicArrayOfStructures*>(dataObj);
	if (aos && !aos->getNumEntries() == 0 && !styles.empty()) {
		gatherData(yValuesCache, xValuesCache, captionsCache);
		if (   yValuesCache.getColumns() == 0 || yValuesCache.getRows() == 0
			 || xValuesCache.getColumns() == 0 || xValuesCache.getRows() == 0) return;
		if (autoZoom) {
			minX = xValuesCache.minComponent();
			maxX = xValuesCache.maxComponent();
			minY = yValuesCache.minComponent();
			maxY = yValuesCache.maxComponent();
		}

		float32 scaleX = 0;
		float32 scaleY = 0;
		float32 extraScale = 1;
		if (maxX > minX) {
			scaleX = 2/(maxX-minX);
			/// for bar diagrams
			if (type == TYPE_BAR_DIAGRAM) extraScale = 2.0f/(2.0f+scaleX);
		} else {
			warning("minX>=maxX");
		}

//		// encoded projection:
//		//    xval <-- (x - minX)*scaleX*extraScale-1;
//		//    yval <-- (y - minY)*scaleY-1;

//		Matrix3f transf =
//			makeMatrix3f(
//				1, 0, -1,
//				0, 1, -1,
//				0, 0,  1
//			)
//			* makeMatrix3f(
//			  scaleX*extraScale,      0, 0,
//			                  0, scaleY, 0,
//			                  0,      0, 1
//		   )
//		   * makeMatrix3f(
//				1, 0, -minX,
//				0, 1, -minY,
//				0, 0,     1
//			);


		if (maxY > minY) {
			scaleY = 2/(maxY-minY);
		} else {
			warning("minY>=maxY");
		}
		float32 xAxisLevel = (0-minY)*scaleY-1;

		// -- draw major axes

		if (drawAxes && scaleX>0 && scaleY>0) {
			float32 aStartX, aStepX; card32 aNumX;
			computeAxis(minX, maxX, aStartX, aStepX, aNumX, 0);
			float32 aStartY, aStepY; card32 aNumY;
			computeAxis(minY, maxY, aStartY, aStepY, aNumY, 0);

			glColor4f(0,0,0,0.5);
			glLineWidth(1);
			glBegin(GL_LINES);
			// transformation:
			//	xval <-- (x - minX)*scaleX*extraScale-1;
			//	yval <-- (y - minY)*scaleY-1;
			for (card32 xa=0; xa<=aNumX; xa++) {
				float32 xPos = (aStartX+aStepX*xa - minX)*scaleX*extraScale-1;
				glVertex2f(xPos, -1.025f );
				glVertex2f(xPos,  1.0f   );
			}
			for (card32 ya=0; ya<=aNumY; ya++) {
				float32 yPos = (aStartY+aStepY*ya - minY)*scaleY-1;
				glVertex2f(-1.025f, yPos);
				glVertex2f( 1.0f,   yPos);
			}
			glEnd();
		}


		// -- draw minor axes

		if (drawAxes && scaleX>0 && scaleY>0) {
			float32 aStartX, aStepX; card32 aNumX;
			computeAxis(minX, maxX, aStartX, aStepX, aNumX, 1);
			float32 aStartY, aStepY; card32 aNumY;
			computeAxis(minY, maxY, aStartY, aStepY, aNumY, 1);

			glColor4f(0,0,0,0.25);
			glLineWidth(1);
			glBegin(GL_LINES);
			// transformation:
			//	xval <-- (x - minX)*scaleX*extraScale-1;
			//	yval <-- (y - minY)*scaleY-1;
			for (card32 xa=0; xa<=aNumX; xa++) {
				float32 xPos = (aStartX+aStepX*xa - minX)*scaleX*extraScale-1;
				glVertex2f(xPos, -1.0f );
				glVertex2f(xPos,  1.0f );
			}
			for (card32 ya=0; ya<=aNumY; ya++) {
				float32 yPos = (aStartY+aStepY*ya - minY)*scaleY-1;
				glVertex2f(-1.0f, yPos);
				glVertex2f( 1.0f, yPos);
			}
			glEnd();
		}

		// -- annotate major axes

		OpenGLCanvas2D *glc = dynamic_cast<OpenGLCanvas2D*>(getContainer());
		int fontSize = this->fontSize;
		if (glc) {
			float32 extraZoom = 1;
			extraZoom = glc->getCameraController()->getZoom();
			if (extraZoom < 0.001f) extraZoom = 0.001f;
			if (extraZoom > 1000) extraZoom = 1000;
			fontSize = (int)(fontSize/extraZoom);
			if (fontSize < 1) fontSize = 1;
			if (fontSize > 200) fontSize = 200;
		}

		glColor4f(0,0,0,1);
		if (drawAxesLabels && scaleX>0 && scaleY>0) {
			float32 aStartX, aStepX; card32 aNumX;
			computeAxis(minX, maxX, aStartX, aStepX, aNumX, 0);
			float32 aStartY, aStepY; card32 aNumY;
			computeAxis(minY, maxY, aStartY, aStepY, aNumY, 0);

			for (card32 xa=0; xa<=aNumX; xa++) {
				float32 xPos = (aStartX+aStepX*xa - minX)*scaleX*extraScale-1;
				double d = aStartX+aStepX*xa;
				QString s = QString("%1").arg(d, 0, 'g', 3);
				glWidget->renderText(xPos-0.018, -1.07, 0, s, QFont("Segoe UI", fontSize));
			}
			for (card32 ya=0; ya<=aNumY; ya++) {
				float32 yPos = (aStartY+aStepY*ya - minY)*scaleY-1;
				double d = aStartY+aStepY*ya;
				QString s = QString("%1").arg(d, 4, 'g', 3);
				glWidget->renderText(-1.125, yPos, 0, s, QFont("Segoe UI", fontSize));
			}
		}

		// -- draw data

		const mpcard numRows = yValuesCache.getRows();
		const mpcard numColumns = yValuesCache.getColumns();

		for (mpcard c=0; c<numColumns; c++) {
			const GLPlotterCurveStyle* currentStyle = styles[c%styles.size()];
			if (curveType && currentStyle->lineThickness > 0) {
				glLineWidth(currentStyle->lineThickness);
				glColor4fv(currentStyle->color.data());

				glBegin(GL_LINE_STRIP);
				for (mpcard r=0; r<numRows; r++) {
					glVertex2f((xValuesCache[c][r] - minX)*scaleX-1,(yValuesCache[c][r]-minY)*scaleY-1);
				}
				glEnd();

			}
			if (curveType && currentStyle->markerSize > 0) {
				glPointSize(currentStyle->markerSize);
				glColor4fv(currentStyle->color.data());

				glBegin(GL_POINTS);
				for (mpcard r=0; r<numRows; r++) {
					glVertex2f((xValuesCache[c][r] - minX)*scaleX-1,(yValuesCache[c][r]-minY)*scaleY-1);
				}
				glEnd();
			}
			if (!curveType) {
				Vector4f col = currentStyle->color;
				if (col[3] >= 1) col[3]=0.5f;
				glColor4fv(col.data());
				glBegin(GL_QUADS);
				for (mpcard r=0; r<numRows; r++) {
					float32 xval = (xValuesCache[c][r] - minX)*scaleX*extraScale-1;
					float32 yval = (yValuesCache[c][r]-minY)*scaleY-1;
					glVertex2f(xval,xAxisLevel);
					glVertex2f(xval,yval);
					glVertex2f(xval+scaleX,yval);
					glVertex2f(xval+scaleX,xAxisLevel);
				}
				glEnd();

				glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
				glLineWidth(currentStyle->lineThickness);
				col = currentStyle->color/1.25;
				col[3]=1;
				glColor4fv(col.data());
				glBegin(GL_QUADS);
				for (mpcard r=0; r<numRows; r++) {
					float32 xval = (xValuesCache[c][r] - minX)*scaleX*extraScale-1;
					float32 yval = (yValuesCache[c][r]-minY)*scaleY-1;
					glVertex2f(xval,xAxisLevel);
					glVertex2f(xval,yval);
					glVertex2f(xval+scaleX,yval);
					glVertex2f(xval+scaleX,xAxisLevel);
				}
				glEnd();
				glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
			}
		}

		// -- draw legend

		if (drawLegend && styles.size() > 0) {
			for (mpcard i=0; i<captionsCache.size(); i++) {
				float32 yPos = 1.0f - i*0.05f;

				glColor4fv(styles[i%(styles.size())]->color.data());
				glLineWidth(3);
				glBegin(GL_LINES);
				glVertex2f(1.05f,yPos+0.01f);
				glVertex2f(1.10f,yPos+0.01f);
				glEnd();

				glColor4f(0,0,0,1);
				QString s = captionsCache[i].c_str();
				glWidget->renderText(1.125, yPos, 0, s, QFont("Segoe UI", fontSize));
			}
		}


	} else {
		error("Can't render. Please check if the data and at least one style are defined correctly.");
	}
	glPopAttrib();
}



bool OpenGLPlotter2D::getDrawAxes() const
{
	return drawAxes;
}

void OpenGLPlotter2D::setDrawAxes(bool value)
{
	drawAxes = value;
}

bool OpenGLPlotter2D::getDrawAxesLabels() const
{
	return drawAxesLabels;
}

void OpenGLPlotter2D::setDrawAxesLabels(bool value)
{
	drawAxesLabels = value;
}

bool OpenGLPlotter2D::getAutoZoom() const
{
	return autoZoom;
}

void OpenGLPlotter2D::setAutoZoom(bool value)
{
	autoZoom = value;
}

float32 OpenGLPlotter2D::getMinX() const
{
	return minX;
}

void OpenGLPlotter2D::setMinX(const float32 &value)
{
	minX = value;
}

float32 OpenGLPlotter2D::getMaxX() const
{
	return maxX;
}

void OpenGLPlotter2D::setMaxX(const float32 &value)
{
	maxX = value;
}

float32 OpenGLPlotter2D::getMinY() const
{
	return minY;
}

void OpenGLPlotter2D::setMinY(const float32 &value)
{
	minY = value;
}

float32 OpenGLPlotter2D::getMaxY() const
{
	return maxY;
}

void OpenGLPlotter2D::setMaxY(const float32 &value)
{
	maxY = value;
}

card32 OpenGLPlotter2D::getType() const
{
	return type;
}

void OpenGLPlotter2D::setType(const card32 &value)
{
	type = value;
}

void OpenGLPlotter2D::addCurveStyle(GLPlotterCurveStyle *style)
{
	styles.push_back(style);
}

void OpenGLPlotter2D::clearCurveStyles()
{
	for (size_t i=0; i<styles.size(); i++) delete styles[i];
	styles.clear();
}

void OpenGLPlotter2D::setDefaultStyles()
{
	clearCurveStyles();
	styles.push_back( new GLPlotterCurveStyle(GLPlotterCurveStyle::COLOR_DEFAULT_RED,         5, 2) );
	styles.push_back( new GLPlotterCurveStyle(GLPlotterCurveStyle::COLOR_DEFAULT_DARK_YELLOW, 5, 2) );
	styles.push_back( new GLPlotterCurveStyle(GLPlotterCurveStyle::COLOR_DEFAULT_GREEN,       5, 2) );
	styles.push_back( new GLPlotterCurveStyle(GLPlotterCurveStyle::COLOR_DEFAULT_BLUE,        5, 2) );
	styles.push_back( new GLPlotterCurveStyle(GLPlotterCurveStyle::COLOR_DEFAULT_GRAY,        5, 2) );
}

void OpenGLPlotter2D::setMaterialStyles()
{
	clearCurveStyles();
	styles.push_back( new GLPlotterCurveStyle(GLPlotterCurveStyle::COLOR_MATERIAL_RED,        0, 4) );
	styles.push_back( new GLPlotterCurveStyle(GLPlotterCurveStyle::COLOR_MATERIAL_ORANGE,     0, 4) );
	styles.push_back( new GLPlotterCurveStyle(GLPlotterCurveStyle::COLOR_MATERIAL_YELLOW,     0, 4) );
	styles.push_back( new GLPlotterCurveStyle(GLPlotterCurveStyle::COLOR_MATERIAL_GREEN,      0, 4) );
	styles.push_back( new GLPlotterCurveStyle(GLPlotterCurveStyle::COLOR_MATERIAL_DARK_BLUE,  0, 4) );
	styles.push_back( new GLPlotterCurveStyle(GLPlotterCurveStyle::COLOR_MATERIAL_LIGHT_BLUE, 0, 4) );
	styles.push_back( new GLPlotterCurveStyle(GLPlotterCurveStyle::COLOR_MATERIAL_VIOLET,     0, 4) );
	styles.push_back( new GLPlotterCurveStyle(GLPlotterCurveStyle::COLOR_MATERIAL_PINK,       0, 4) );
	styles.push_back( new GLPlotterCurveStyle(GLPlotterCurveStyle::COLOR_MATERIAL_GRAY,       0, 4) );
}

const DynamicArrayOfStructures *OpenGLPlotter2D::getDataSet()
{
	return dynamic_cast<DynamicArrayOfStructures*>(getContext()->getData()->getObject(getData()));

}

bool OpenGLPlotter2D::inXYMode()
{
	return type == TYPE_XY_CURVE;
}

void OpenGLPlotter2D::gatherData(DMatrixF &yAxis, DMatrixF &xAxis, vector<string> &captions)
{
	const DAOS *aos = getDataSet();
	const mpcard numRows = aos->getNumEntries();
	const StructureDescriptor *sd = aos->getDescr();

	captions.clear();

	mpcard numColumns = 0;
	if (curves.empty()) { // case one - no specific curve mapping defined

		vector<mpcard> offsets;
		vector<SAD> attribs;
		for (mpcard i=0; i<sd->getNumAttributes(); i++) {
			offsets.push_back(sd->getAttributeOffset(i));
			attribs.push_back(sd->getAttribute(i));
		}

		if (inXYMode()) {
			numColumns = aos->getDescr()->getNumAttributes();
		} else {
			numColumns = aos->getDescr()->computeOverallDimension();
		}
		xAxis.setDimension(numColumns, numRows);
		xAxis.setZero();
		yAxis.setDimension(numColumns, numRows);
		yAxis.setZero();
		if (inXYMode()) {
			for (mpcard row=0; row<numRows; row++) {
				const void* dataPointer = aos->getDataPointer(row);
				for (size_t attrib=0; attrib<attribs.size(); attrib++) {
					if (attribs[attrib].numComponents < 2) continue;
					if (row==0) {
						captions.push_back(attribs[attrib].name);
					}
					mpcard offset = offsets[attrib];
					float32 valuex=0;
					float32 valuey=0;
					switch (attribs[attrib].dataFormat) {
						case SAD::DATA_FORMAT_CARD8: {
							valuex = *(((card8*)((card8*)dataPointer+offset)));
							valuey = *(((card8*)((card8*)dataPointer+offset))+1);
							break;
						}
						case SAD::DATA_FORMAT_CARD16: {
							valuex = (float32)*(((card16*)((card8*)dataPointer+offset)));
							valuey = (float32)*(((card16*)((card8*)dataPointer+offset))+1);
							break;
						}
						case SAD::DATA_FORMAT_CARD32: {
							valuex = (float32)*(((card32*)((card8*)dataPointer+offset)));
							valuey = (float32)*(((card32*)((card8*)dataPointer+offset))+1);
							break;
						}
						case SAD::DATA_FORMAT_CARD64: {
							valuex = (float32)*(((card64*)((card8*)dataPointer+offset)));
							valuey = (float32)*(((card64*)((card8*)dataPointer+offset))+1);
							break;
						}
						case SAD::DATA_FORMAT_FLOAT32: {
							valuex = (float32)*(((float32*)((card8*)dataPointer+offset)));
							valuey = (float32)*(((float32*)((card8*)dataPointer+offset))+1);
							break;
						}
						case SAD::DATA_FORMAT_FLOAT64: {
							valuex = (float32)*(((float64*)((card8*)dataPointer+offset)));
							valuey = (float32)*(((float64*)((card8*)dataPointer+offset))+1);
							break;
						}
						case SAD::DATA_FORMAT_INT8:  {
							valuex = (float32)*(((int8*)((card8*)dataPointer+offset)));
							valuey = (float32)*(((int8*)((card8*)dataPointer+offset))+1);
							break;
						}
						case SAD::DATA_FORMAT_INT16:  {
							valuex = (float32)*(((int16*)((card8*)dataPointer+offset)));
							valuey = (float32)*(((int16*)((card8*)dataPointer+offset))+1);
							break;
						}
						case SAD::DATA_FORMAT_INT32:  {
							valuex = (float32)*(((int32*)((card8*)dataPointer+offset)));
							valuey = (float32)*(((int32*)((card8*)dataPointer+offset))+1);
							break;
						}
						case SAD::DATA_FORMAT_INT64:  {
							valuex = (float32)*(((int64*)((card8*)dataPointer+offset)));
							valuey = (float32)*(((int64*)((card8*)dataPointer+offset))+1);
							break;
						}
					}
					xAxis[attrib][row] = valuex;
					yAxis[attrib][row] = valuey;
				}
			}

		} else { // not in xy mode

			for (mpcard row=0; row<numRows; row++) {
				const void* dataPointer = aos->getDataPointer(row);
				mpcard col = 0;
				for (size_t attrib=0; attrib<attribs.size(); attrib++) {
					mpcard extra = 0;
					for (card32 d=0; d<attribs[attrib].numComponents; d++) {
						if (row==0) {
							captions.push_back(attribs[attrib].name+string("[")+intToStr(d)+string("]"));
						}
						mpcard offset = offsets[attrib]+extra;
						float valuey = 0;
						switch (attribs[attrib].dataFormat) {
							case SAD::DATA_FORMAT_CARD8: {
								valuey = *(((card8*)((card8*)dataPointer+offset)));
								extra += sizeof(card8);
								break;
							}
							case SAD::DATA_FORMAT_CARD16: {
								valuey = (float32)*(((card16*)((card8*)dataPointer+offset)));
								extra += sizeof(card16);
								break;
							}
							case SAD::DATA_FORMAT_CARD32: {
								valuey = (float32)*(((card32*)((card8*)dataPointer+offset)));
								extra += sizeof(card32);
								break;
							}
							case SAD::DATA_FORMAT_CARD64: {
								valuey = (float32)*(((card64*)((card8*)dataPointer+offset)));
								extra += sizeof(card64);
								break;
							}
							case SAD::DATA_FORMAT_FLOAT32: {
								valuey = (float32)*(((float32*)((card8*)dataPointer+offset)));
								extra += sizeof(float32);
								break;
							}
							case SAD::DATA_FORMAT_FLOAT64: {
								valuey = (float32)*(((float64*)((card8*)dataPointer+offset)));
								extra += sizeof(float64);
								break;
							}
							case SAD::DATA_FORMAT_INT8:  {
								valuey = (float32)*(((int8*)((card8*)dataPointer+offset)));
								extra += sizeof(int8);
								break;
							}
							case SAD::DATA_FORMAT_INT16:  {
								valuey = (float32)*(((int16*)((card8*)dataPointer+offset)));
								extra += sizeof(int16);
								break;
							}
							case SAD::DATA_FORMAT_INT32:  {
								valuey = (float32)*(((int32*)((card8*)dataPointer+offset)));
								extra += sizeof(int32);
								break;
							}
							case SAD::DATA_FORMAT_INT64:  {
								valuey = (float32)*(((int64*)((card8*)dataPointer+offset)));
								extra += sizeof(int64);
								break;
							}
						}
						xAxis[col][row] = row;
						yAxis[col][row] = valuey;
						col++;
					}
				}
			}

		} // end if not in xy mode

	} else { // case two - gather only curves listed

		xAxis.setDimension(curves.size(), numRows);
		xAxis.setZero();
		yAxis.setDimension(curves.size(), numRows);
		yAxis.setZero();
		captions.clear();

		for (size_t i=0; i<curves.size(); i++) {
			GLPlotterCurveSelection *sel = curves[i];
			AAT xChannelAAT = NULL_AAT;
			SAD xChannelSAD;
			if (aos->providesAttribute(sel->xAxisChannelName)) {
				xChannelAAT = aos->getAAT(sel->xAxisChannelName);
				xChannelSAD = aos->getDescr()->getAttribute(sel->xAxisChannelName);
				if (xChannelSAD.numComponents <= sel->xAxisIndex) {
					warning(string("curve ")+intToStr(i) + string(" xAxisIndex out of range. ignoring entry."));
					continue;
				}
			}
			AAT yChannelAAT = NULL_AAT;
			SAD yChannelSAD;
			if (aos->providesAttribute(sel->yAxisChannelName)) {
				yChannelAAT = aos->getAAT(sel->yAxisChannelName);
				yChannelSAD = aos->getDescr()->getAttribute(sel->yAxisChannelName);
				if (yChannelSAD.numComponents <= sel->yAxisIndex) {
					warning(string("curve ")+intToStr(i) + string(" yAxisIndex out of range. ignoring entry."));
					continue;
				}
			} else {
				warning(string("curve ")+intToStr(i) + string(" yAxisChannelName not found. ignoring entry."));
				continue;
			}
			string cap;
			if (xChannelAAT != NULL_AAT) {
				cap = string("(")+sel->xAxisChannelName+string("[")+intToStr(sel->xAxisIndex)+string("], ");
			}
			cap +=sel->yAxisChannelName+string("[")+intToStr(sel->yAxisIndex)+string("]");
			if (xChannelAAT != NULL_AAT) {
				cap += ")";
			}
			captions.push_back(cap);
			for (mpcard row=0; row<numRows; row++) {
				float32 valuey=0;
				float32 valuex=i;

				const void* dataPointer = aos->getDataPointer(row);
				mpcard offsetY = yChannelAAT.getOffset();
				mpcard itemY   = sel->yAxisIndex;

				switch (yChannelSAD.dataFormat) {
					case SAD::DATA_FORMAT_CARD8:   valuey = (float32)*(((  card8*)((card8*)dataPointer+offsetY))+itemY); break;
					case SAD::DATA_FORMAT_CARD16:  valuey = (float32)*((( card16*)((card8*)dataPointer+offsetY))+itemY); break;
					case SAD::DATA_FORMAT_CARD32:  valuey = (float32)*((( card32*)((card8*)dataPointer+offsetY))+itemY); break;
					case SAD::DATA_FORMAT_CARD64:  valuey = (float32)*((( card64*)((card8*)dataPointer+offsetY))+itemY); break;
					case SAD::DATA_FORMAT_FLOAT32: valuey = (float32)*(((float32*)((card8*)dataPointer+offsetY))+itemY); break;
					case SAD::DATA_FORMAT_FLOAT64: valuey = (float32)*(((float64*)((card8*)dataPointer+offsetY))+itemY); break;
					case SAD::DATA_FORMAT_INT8:    valuey = (float32)*(((   int8*)((card8*)dataPointer+offsetY))+itemY); break;
					case SAD::DATA_FORMAT_INT16:   valuey = (float32)*(((  int16*)((card8*)dataPointer+offsetY))+itemY); break;
					case SAD::DATA_FORMAT_INT32:   valuey = (float32)*(((  int32*)((card8*)dataPointer+offsetY))+itemY); break;
					case SAD::DATA_FORMAT_INT64:   valuey = (float32)*(((  int64*)((card8*)dataPointer+offsetY))+itemY); break;
				}
				if (xChannelAAT != NULL_AAT) {
					mpcard offsetX = xChannelAAT.getOffset();
					mpcard itemX   = sel->xAxisIndex;
					switch (yChannelSAD.dataFormat) {
						case SAD::DATA_FORMAT_CARD8:   valuex = (float32)*(((  card8*)((card8*)dataPointer+offsetX))+itemX); break;
						case SAD::DATA_FORMAT_CARD16:  valuex = (float32)*((( card16*)((card8*)dataPointer+offsetX))+itemX); break;
						case SAD::DATA_FORMAT_CARD32:  valuex = (float32)*((( card32*)((card8*)dataPointer+offsetX))+itemX); break;
						case SAD::DATA_FORMAT_CARD64:  valuex = (float32)*((( card64*)((card8*)dataPointer+offsetX))+itemX); break;
						case SAD::DATA_FORMAT_FLOAT32: valuex = (float32)*(((float32*)((card8*)dataPointer+offsetX))+itemX); break;
						case SAD::DATA_FORMAT_FLOAT64: valuex = (float32)*(((float64*)((card8*)dataPointer+offsetX))+itemX); break;
						case SAD::DATA_FORMAT_INT8:    valuex = (float32)*(((   int8*)((card8*)dataPointer+offsetX))+itemX); break;
						case SAD::DATA_FORMAT_INT16:   valuex = (float32)*(((  int16*)((card8*)dataPointer+offsetX))+itemX); break;
						case SAD::DATA_FORMAT_INT32:   valuex = (float32)*(((  int32*)((card8*)dataPointer+offsetX))+itemX); break;
						case SAD::DATA_FORMAT_INT64:   valuex = (float32)*(((  int64*)((card8*)dataPointer+offsetX))+itemX); break;
					}
				}
				xAxis[i][row] = valuex;
				yAxis[i][row] = valuey;
			}
		}
	}
}


OpenGLPlotter2D::~OpenGLPlotter2D()
{
	clearCurveStyles();
}

GLPlotterCurveStyle::GLPlotterCurveStyle()
{
	color = makeVector4f(1,0,0,1);
	markerSize = 5;
	lineThickness = 2;
}

GLPlotterCurveStyle::GLPlotterCurveStyle(Vector4f color, float32 markerSize, float32 lineThickness)
{
	this->color = color;
	this->markerSize = markerSize;
	this->lineThickness = lineThickness;
}

GLPlotterCurveSelection::GLPlotterCurveSelection()
{
	yAxisIndex = 0;
	xAxisIndex = 0;
}

GLPlotterCurveSelection::GLPlotterCurveSelection(string yAxisChannelName, string xAxisChannelName, card32 yAxisIndex, card32 xAxisIndex)
{
	this->yAxisChannelName = yAxisChannelName;
	this->xAxisChannelName = xAxisChannelName;
	this->yAxisIndex		  = yAxisIndex;
	this->xAxisIndex		  = xAxisIndex;
}

bool OpenGLPlotter2D::canHandleCanvas(const MetaClass *canvasType)
{
	return canvasType != NULL && canvasType->inheritsFrom(OpenGLCanvas::getClass());
}

bool OpenGLPlotter2D::canHandleData(const MetaClass *dataType)
{
	return dataType != NULL && dataType->inheritsFrom(DynamicArrayOfStructures::getClass());
}

const MetaClass *OpenGLPlotter2D::preferredCanvasType()
{
	return OpenGLCanvas2D::getClass();
}
