//---------------------------------------------------------------------------
#include "precompiledHeaders.h"
//---------------------------------------------------------------------------
#include "MeshImporterSMFOBJ.h"
//---------------------------------------------------------------------------
#include "StringHelper.h"
#include <fstream>
#include <sstream>
#include <QImage>
#include <iosfwd>
#include <string>
#include <vector>
#include "TriangleMesh.h"
#include "DynamicArrayOfStructures.hpp"
#include "GeoXOutput.h"
//---------------------------------------------------------------------------


IMPLEMENT_GEOX_CLASS(MeshImporterSMFOBJ,0) {
   BEGIN_CLASS_INIT(MeshImporterSMFOBJ)
}

MeshImporterSMFOBJ::MeshImporterSMFOBJ() {}

MeshImporterSMFOBJ::~MeshImporterSMFOBJ() {}


bool MeshImporterSMFOBJ::performImport(string fileName) {

	string ext = extractFileExt(strUCase(fileName));
   if (!((ext==".SMF")||(ext==".OBJ"))) {
		output << "warning -- PointCloudImporterReaderSMFOBJ::beginImport()\nfile ending is not .SMF or .OBJ.\nTrying anyways...";
	};

	// clear caches
	positions.clear();
	colors.clear();
	normals.clear();
	faces.clear();
	textureCoords.clear();

	// open file
	ifstream in;
	in.clear();
	in.open(fileName.c_str(), ifstream::in);
	if (!in.good()) { 
		output << "PointCloudImporterReaderSMFOBJ::beginImport()\nError opening file \"" + fileName + "\"!"; 
		return false;
	};

	string line;
	card64 lineCount = 0;
	string x;
	float32 v1, v2, v3;
	bool bindColorToVertex = true;
	bool bindNormalToVertex = false;
	bool hasObjTexture = false;
	bool objMtlDefined = false;
	card32 currentPolygonID = 0;

   while (!in.eof()) {
		getline(in, line);
		lineCount++;
		line = removeDblSpace(line); // remove multiple spaces between values and make them single space
		line = trimStr(line); // remove beginning and ending spaces from line

		if( line.length() && line[line.size()-1] == '\\' )	// dosch obj files
		{
			string line2;
			getline(in, line2);
			lineCount++;
			line = line.substr(0,line.size()-1) + " " + line2;
			line = removeDblSpace(line); // remove multiple spaces between values and make them single space
			line = trimStr(line); // remove beginning and ending spaces from line
		}

		istringstream lineStream;
		lineStream.str(line);
		x = ""; 
		lineStream >> x;

		if (x=="v") {
			// vertex position
		   lineStream >> v1; lineStream >> v2; lineStream >> v3;
			positions.push_back(makeVector3f(v1,v2,v3));
		} else if ((x=="f")||(x=="t")) {
			// face:  "f p p p" or "f p/t/n ..."
			// attention: "t" is a quick-hack for smf's with triangles, don't know if there will be some problems with other formats
			vector<int32> pindices;
			vector<int32> tindices;
			vector<int32> nindices;
			if ((line.find_first_of("f ")==0)||(line.find_first_of("t ")==0)) line = line.substr(2,line.length());
			vector<string> faceEntries = splitString(line, " ");
			for (card32 i=0; i<faceEntries.size(); i++) {
				vector<string> faceEntryIndexes = splitString(faceEntries[i], "/");
				// position
				int32 posIdx = 0;
				bool success = strToInt(faceEntryIndexes[0], posIdx);
				if( posIdx < 0 )
				{
					posIdx = (int)positions.size()+posIdx+1;
				}
				if( posIdx <= 0 || posIdx > (int32)positions.size())
				{
                    output << (int)positions.size() << "| "<< faceEntryIndexes[0] << "\n";
				}
				if (success) pindices.push_back(posIdx);
				if (x=="f") pindices.back()--; // idx begins with 1 but vertex list with 0 if "f"
			}
         if (pindices.size() >= 3) {
				// force triangles (mabye quads in file)
            for (card32 i=0; i<=pindices.size()-3; i++) {
					faces.push_back(makeVector3i(pindices[0], pindices[1+i], pindices[2+i]));
					facePolyID.push_back(currentPolygonID);
            }
			currentPolygonID++;
         }
			// todo: handle texture information
			// todo: handle normal information

		} else if (x=="c") {
			// color
	      lineStream >> v1; lineStream >> v2; lineStream >> v3;
	      colors.push_back(makeVector3f(v1,v2,v3));
		} else if (x=="n") {
			// normal (smf only)
	      lineStream >> v1; lineStream >> v2; lineStream >> v3;
	      normals.push_back(makeVector3f(v1,v2,v3));
		  bindNormalToVertex = true;
		} else if (x=="vn") {
			// normal (obj only)
	      lineStream >> v1; lineStream >> v2; lineStream >> v3;
	      normals.push_back(makeVector3f(v1,v2,v3));
			bindNormalToVertex = true;
		} else if (x=="vt") {
			// texture coordinates (obj only)
	      lineStream >> v1; lineStream >> v2;
	      textureCoords.push_back(makeVector2f(v1,v2));
			hasObjTexture = true;
			bindColorToVertex = true;
		} else if ((x=="mtllib") || (x=="usemtl")) {
			// .obj material library files not supported!
			objMtlDefined = true;
		} else if (x=="bind") {
			lineStream >> x;
			if (x=="c") {
				lineStream >> x;
				if (x=="vertex") {
					bindColorToVertex = true;
				} else if (x=="face") {
					bindColorToVertex = false;
				} else {
					// not a supported binding
					continue;
				}
			} else if (x=="n") {
				lineStream >> x;
				if (x=="vertex") {
					bindNormalToVertex = true;
				} else if (x=="face") {
					bindNormalToVertex = false;
				} else {
					// not a supported binding
					continue;
				}
			} else {
				// not a supported binding
				continue;
			};
		} else {
			// jump over comments and unsupported or invalid lines
			continue;
		};

	}; // end while !in.eof

	// end loading from file
	in.close();

   if(normals.size()<positions.size()) bindNormalToVertex = false;

	if (bindNormalToVertex) {
		// one normal per vertex in file
		pAssert(normals.size()>=positions.size());

		// todo: lookup face normal indices, without that maybe results in wrong normal, see above faceEntryIndexes[2]


	} else {
		// one normal per face or no normals in file, so calculate vertex normals from faces
		Vector3f tempVF;

		if(normals.size()<faces.size()){
			normals.clear();
		}

		if (normals.empty()) {
			// calc one normal per face
			for (card32 i=0; i<faces.size(); i++) {
				tempVF = normalize(positions[(faces[i])[1]]-positions[(faces[i])[0]]).crossProduct(normalize(positions[(faces[i])[2]]-positions[(faces[i])[0]]));
				tempVF = normalize(tempVF);
				normals.push_back(tempVF);
			};
		};

		pAssert(normals.size()>=faces.size());
		vector<Vector3f> tmpNormals;

		// initialize with zero-Normals first
		for (card32 i=0; i<positions.size(); i++) {
			tmpNormals.push_back(makeVector3f(0.0f,0.0f,0.0f));
		};

		// for every face add face normal to edge vertices normal
		for (card32 i=0; i<faces.size(); i++) {
			tempVF = normals[i];
			tmpNormals[(faces[i])[0]] += tempVF;
			tmpNormals[(faces[i])[1]] += tempVF;
			tmpNormals[(faces[i])[2]] += tempVF;
		}

		// finally add normalized normals
		normals.clear();
		for (card32 i=0; i<positions.size(); i++) {
			normals.push_back(normalize(tmpNormals[i]));
		};
		tmpNormals.clear();

	};	// end if bindNormalToVertex

	if (bindColorToVertex) {
		// one color per vertex, no colors in file or obj texture
		if (colors.empty()) {
			bool foundTextureFile = false;
			if (hasObjTexture && objMtlDefined) {
				// test if texture exists without obj-material-file
				string FileNameTex = fileName.replace(fileName.size()-3, 3, "bmp");
				if (geoxFileExists(FileNameTex.c_str())) foundTextureFile = true;
			}
			if ((hasObjTexture && !objMtlDefined) || foundTextureFile) {
				// is obj with texture and no obj-material-file defined
				bool textureFileNotValid = false;
				pAssert(textureCoords.size()>=positions.size());
				string FileNameTex = fileName.replace(fileName.size()-3, 3, "bmp");
				if (!geoxFileExists(FileNameTex.c_str())) {
					output << "PointCloudImporterReaderSMFOBJ::beginImport()\nFile not found! Textured .obj must have a .bmp with same name!\nAll vertexes are set to white.\n";
					textureFileNotValid = true;
				};
				QImage *myImage = new QImage(QImage(QString(FileNameTex.c_str())).convertToFormat(QImage::Format_RGB32));
				if (myImage->isNull()) {
					output << "PointCloudImporterReaderSMFOBJ::beginImport()\nTexture file is not a valid picture!\nAll vertexes are set to white.\n";
					textureFileNotValid = true;
				};
				if (!textureFileNotValid) {
					card32 iW = myImage->width();
					card32 iH = myImage->height();
					float32 inv255 = 1.0f/255.0f;

					// todo: lookup face color indices, without that maybe results in wrong color, see above faceEntryIndexes[1]

					// calculate colors
					for (card32 n=0; n<positions.size(); n++)
					{
						card32 tX = (textureCoords[n])[0] * iW;
						card32 tY = (1.0f - (textureCoords[n])[1]) * iH;

						QRgb pixel = myImage->pixel(tX,tY);
						colors.push_back(makeVector3f(qRed(pixel),qGreen(pixel),qBlue(pixel))*inv255);
					};

				} else {
					// texture file invalid, so set white for every vertex
					for (card32 i=0; i<positions.size(); i++) {
						colors.push_back(makeVector3f(1.0f,1.0f,1.0f));
					};
				};

			} else {
				// no texture or an obj-material-file is defined (not supported), so set white for every vertex
				for (card32 i=0; i<positions.size(); i++) {
					colors.push_back(makeVector3f(1.0f,1.0f,1.0f));
				};
			}
		}; // end if colors.empty()
		pAssert(colors.size()>=positions.size());

	} else {
		// one color per face in file, so calculate vertex colors from faces
		Vector3f tempVF;

		pAssert(colors.size()>=faces.size());
		vector<Vector3f> tmpColors;

		// initialize with black first
		for (card32 i=0; i<positions.size(); i++) {
			tmpColors.push_back(makeVector3f(0.0f,0.0f,0.0f));
		};

		// for every face add face color to edge vertices color
		for (card32 i=0; i<faces.size(); i++) {
			tempVF = colors[i];
			tmpColors[(faces[i])[0]] += tempVF;
			tmpColors[(faces[i])[1]] += tempVF;
			tmpColors[(faces[i])[2]] += tempVF;
		}

		// finally add "normalized" colors
		colors.clear();
		for (card32 i=0; i<positions.size(); i++) {
			colors.push_back(tmpColors[i]/3.0f);
		};
		tmpColors.clear();

	};	// end if bindColorToVertex

	return true;
}

void MeshImporterSMFOBJ::getPositions( vector<Vector3f> &positions )
{
   positions = this->positions;
}

void MeshImporterSMFOBJ::getColors( vector<Vector3f> &colors )
{
   colors = this->colors;
}

void MeshImporterSMFOBJ::getNormals( vector<Vector3f> &normals )
{
   normals = this->normals;
}

void MeshImporterSMFOBJ::getFaceIndices( vector<Vector3i> &faces )
{
   faces = this->faces;
}

void MeshImporterSMFOBJ::getTextureCoordinates( vector<Vector2f> &textureCoords )
{
   textureCoords = this->textureCoords;
}


TriangleMesh * MeshImporterSMFOBJ::createTriangleMesh()
{
	StructureDescriptor sdVertex;
	sdVertex.pushAttrib(mSAD("position",3,   SAD::DATA_FORMAT_FLOAT32));
	sdVertex.pushAttrib(mSAD("normal",3,     SAD::DATA_FORMAT_FLOAT32));
	sdVertex.pushAttrib(mSAD("color",3,      SAD::DATA_FORMAT_FLOAT32));
	sdVertex.pushAttrib(mSAD("texcoords", 2, SAD::DATA_FORMAT_FLOAT32));
	StructureDescriptor sdFace;
	sdFace.pushAttrib(mSAD("index", 3, SAD::DATA_FORMAT_INT32));
	
	mpcard numVertices = positions.size();
	mpcard numTriangles = faces.size();

	TriangleMesh *result = new TriangleMesh;
	result->clearAndSetup(&sdVertex, numVertices, &sdFace, numTriangles);
	
	DynamicArrayOfStructures *vertices = result->getVertices();
	AAT positionAAT = vertices->getAAT("position");
	AAT normalAAT = vertices->getAAT("normal");
	AAT colorAAT = vertices->getAAT("color");
	AAT texcoordsAAT = vertices->getAAT("texcoords");
	for (size_t i=0; i<positions.size(); i++) {
		vertices->set3f(i, positionAAT, positions[i]);
		if (i<normals.size()) vertices->set3f(i, normalAAT, normals[i]);
		if (i<colors.size())vertices->set3f(i, colorAAT, colors[i]);
		if (i<textureCoords.size()) vertices->set2f(i, texcoordsAAT, textureCoords[i]);
	}

	DynamicArrayOfStructures *triangles = result->getTriangles();
	AAT indexAAT = triangles->getAAT("index");
	for (size_t i=0; i<faces.size(); i++) {
		triangles->set3i(i, indexAAT, faces[i]);
	}
	return result;
}

