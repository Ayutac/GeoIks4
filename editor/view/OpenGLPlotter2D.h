#ifndef OPENGLPLOTTER2D_H
#define OPENGLPLOTTER2D_H

#include "GeoXOpenGLView.h"
#include "ObjectListProperty.h"
#include "LinearAlgebra.hpp"
#include "DynamicLinearAlgebra.hpp"


// -------------------- helper classes --------------------


class DynamicArrayOfStructures;


/// Helper class for the OpenGLPlotter2D.
/// Represents the mapping of attribute channels to a curve.
///
class GLPlotterCurveSelection : public Persistent {
	GEOX_CLASS(GLPlotterCurveSelection)
 public:
	string yAxisChannelName;
	string xAxisChannelName;
	card32 yAxisIndex;
	card32 xAxisIndex;
	GLPlotterCurveSelection();
	GLPlotterCurveSelection(string yAxisChannelName, string xAxisChannelName, card32 yAxisIndex, card32 xAxisIndex);
};



/// Helper class for the OpenGLPlotter2D.
/// Represents the style of a curve.
///
class GLPlotterCurveStyle : public Persistent {
	GEOX_CLASS(GLPlotterCurveStyle)
 public:
	/// default color suggestions (modern)
	static const Vector4f COLOR_MATERIAL_RED;
	/// default color suggestions (modern)
	static const Vector4f COLOR_MATERIAL_ORANGE;
	/// default color suggestions (modern)
	static const Vector4f COLOR_MATERIAL_YELLOW;
	/// default color suggestions (modern)
	static const Vector4f COLOR_MATERIAL_GREEN;
	/// default color suggestions (modern)
	static const Vector4f COLOR_MATERIAL_DARK_BLUE;
	/// default color suggestions (modern)
	static const Vector4f COLOR_MATERIAL_LIGHT_BLUE;
	/// default color suggestions (modern)
	static const Vector4f COLOR_MATERIAL_VIOLET;
	/// default color suggestions (modern)
	static const Vector4f COLOR_MATERIAL_PINK;
	/// default color suggestions (modern)
	static const Vector4f COLOR_MATERIAL_GRAY;

	/// default color suggestions (oldskool)
	static const Vector4f COLOR_DEFAULT_RED;
	/// default color suggestions (oldskool)
	static const Vector4f COLOR_DEFAULT_DARK_YELLOW;
	/// default color suggestions (oldskool)
	static const Vector4f COLOR_DEFAULT_GREEN;
	/// default color suggestions (oldskool)
	static const Vector4f COLOR_DEFAULT_BLUE;
	/// default color suggestions (oldskool)
	static const Vector4f COLOR_DEFAULT_GRAY;

	/// Color in RGBA format. Set 4th entry to 1 for full opacity.
	Vector4f color;
	/// Dots as markers; set size to zero to disable
	float32 markerSize;
	/// Line thickness; set to zero to disable
	float32 lineThickness;

	GLPlotterCurveStyle();
	GLPlotterCurveStyle(Vector4f color, float32 markerSize, float32 lineThickness);
};



// -------------------- main class --------------------


/// This class is a view that plots the contents of a dynamic array of structures in a 2D diagram
/// (similar to a charting program like GNU Plot, just much simpler, with fewer options)
///
class OpenGLPlotter2D : public GeoXOpenGLView {
   GEOX_CLASS(OpenGLPlotter2D)
 private:
	/// show axes
	bool drawAxes;
	/// show axis labels
	bool drawAxesLabels;
	/// show a legend
	bool drawLegend;
	/// relative font size (scaled with zoom); default = 12.
	card32 fontSize;
	/// sets minX, maxX, minY, maxY automatically to fit the whole data set
	bool autoZoom;

	/// Start of viewing window (x-axis). Viewing window is mapped to range [-1,1] x [-1,1]*yToYAspectRatio.
	float32 minX;
	/// End of viewing window (x-axis). Viewing window is mapped to range [-1,1] x [-1,1]*yToYAspectRatio.
	float32 maxX;
	/// Start of viewing window (y-axis). Viewing window is mapped to range [-1,1] x [-1,1]*yToYAspectRatio.
	float32 minY;
	/// End of viewing window (y-axis). Viewing window is mapped to range [-1,1] x [-1,1]*yToYAspectRatio.
	float32 maxY;

	/// scatter plot (use two coordinates to determine position)
	static const card32 TYPE_XY_CURVE = 0;
	/// y-only plot (use one coordinate and index to determine position)
	static const card32 TYPE_FUNC_CURVE = 1;
	/// same as FUNC_CURVE_STYLE, but shown as bars
	static const card32 TYPE_BAR_DIAGRAM = 2;

	/// configure type of diagram plotted
	card32 type;

	/// list of curve styles; repeated when
	vector<GLPlotterCurveStyle*> styles;
	IMPLEMENT_OBJECT_LIST_ACCESS_STL(styles, GLPlotterCurveStyle)

	/// List of curves (mappings of attribute channels to x/y coordinates).
	/// If this list is empty, the plotter just plots all channels one after another.
	/// It uses all dimensions as separate channels in "FUNC" or "BAR" mode.
	/// It uses the first two dimensions (if present) in XY mode.
	vector<GLPlotterCurveSelection*> curves;
	IMPLEMENT_OBJECT_LIST_ACCESS_STL(curves, GLPlotterCurveSelection)

 private:
	void setDefaultStyles();
	void setMaterialStyles();

	DMatrixF xValuesCache;
	DMatrixF yValuesCache;
	vector<string> captionsCache;

 protected:
	/// This method fetches the data; returns NULL if not found.
	const DynamicArrayOfStructures *getDataSet();

	/// returns true if a scatter plot type has been selected
	bool inXYMode();

	/// This method collects all the data and stores it in to matrices.
	/// The colums represent multiple curves.
	/// In XY mode (diagram type), the xAxis is read from data, otherwise it is created
	/// by simple incrementing and scaling.
	void gatherData(DMatrixF &yAxis, DMatrixF &xAxis, vector<string> &captions);

 public:
   OpenGLPlotter2D();

	/// Use this method to select which curves should be plotted.
	/// If no curves are selected, all dimensions of all channels are plotted by default.
	///
	/// Parameters:
	///  - yAxisChannelName:  attribute channel name for the y-axis (converted to an AAT; values default to zero of not found)
	///  - yAxisChannelIndex: index of the y-axis within the named channel
	///  - xAxisChannelName:  name for x-axis; optional unless type == TYPE_XY_CURVE
	///  - xAxisChannelIndex: index for x-axis; optional unless type == TYPE_XY_CURVE
	void addCurve(string yAxisChannelName,      card32 yAxisChannelIndex,
					  string xAxisChannelName = "", card32 xAxisChannelIndex = 0);

	virtual void renderGL(QGLWidget *glWidget);

	// -- getters / setters (see private member documentation above)

	bool getDrawAxes() const;
	void setDrawAxes(bool value);

	bool getDrawAxesLabels() const;
	void setDrawAxesLabels(bool value);

	bool getAutoZoom() const;
	void setAutoZoom(bool value);

	float32 getMinX() const;
	void setMinX(const float32 &value);

	float32 getMaxX() const;
	void setMaxX(const float32 &value);

	float32 getMinY() const;
	void setMinY(const float32 &value);

	float32 getMaxY() const;
	void setMaxY(const float32 &value);

	card32 getType() const;
	void setType(const card32 &value);

	/// Add a new style.
	void addCurveStyle(GLPlotterCurveStyle* style);
	/// Clear all styles (before adding new ones); the constructor provides
	/// a few default styles - so clear first if you prefer to use your own.
	void clearCurveStyles();

	// inherited
	virtual bool canHandleCanvas(const MetaClass *canvasType);
	// inherited
	virtual bool canHandleData(const MetaClass *dataType);
	// inherited
	virtual const MetaClass *preferredCanvasType();

	~OpenGLPlotter2D();
};



#endif
