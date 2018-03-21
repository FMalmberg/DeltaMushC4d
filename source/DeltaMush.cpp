#include "c4d.h"
#include "odeltamush.h"
#include <set>
#include <vector>
using namespace std;

class DeltaMush : public ObjectData
{
public:
	virtual Bool Init(GeListNode *node);
	virtual Bool ModifyObject   (BaseObject *mod, BaseDocument *doc, BaseObject *op, const Matrix &op_mg, const Matrix &mod_mg, Real lod, LONG flags, BaseThread *thread);
	static NodeData *Alloc(void) { return gNew DeltaMush; }
};

Bool DeltaMush::Init(GeListNode *node){
	BaseObject		*op   = (BaseObject*)node;
	BaseContainer *data = op->GetDataInstance();

	data->SetLong(ITERATIONS, 5);

	return true;
}

Bool DeltaMush::ModifyObject(BaseObject *mod, BaseDocument *doc, BaseObject *op, const Matrix &op_mg, const Matrix &mod_mg, Real lod, LONG flags, BaseThread *thread){
	if (!(op->IsInstanceOf(Opolygon))) {
		return TRUE;
	}

	Real		w;
	SReal		*weight=NULL;
	weight = ToPoint(op)->CalcVertexMap(mod);

	BaseContainer *data = mod->GetDataInstance();

	long iterations=data->GetLong(ITERATIONS);


	
	PolygonObject* obj=ToPoly(op);
	Vector* vertices=obj->GetPointW();
	long nverts=obj->GetPointCount();
	
	PointObject* target_obj=NULL;

	Bool has_reference=false;
	BaseList2D* reference=data->GetLink(REFERENCE_OBJECT, doc);
	if(reference){
		if( reference->IsInstanceOf(Opoint)){
			target_obj=ToPoint(reference);
			if(nverts==target_obj->GetPointCount()){
				has_reference=true;  
				//Reference object exists, is a point object, 
				//and has the same number of vertices as the deformed object.
			}
		}
	}


	const CPolygon* polygons = obj->GetPolygonR();
	long npolys=obj->GetPolygonCount();


	vector<set<long> > neighbors(nverts);

	//Build adjacency map
	CPolygon face;
	for(int i=0; i<npolys; i++){
		face=polygons[i];
		neighbors[face.a].insert(face.b);
		neighbors[face.b].insert(face.a);
		neighbors[face.b].insert(face.c);
		neighbors[face.c].insert(face.b);
		if(face.d!=face.c){ //Quad
			neighbors[face.c].insert(face.d);
			neighbors[face.d].insert(face.a);
			neighbors[face.a].insert(face.d);
		}
		else{ //Triangle
			neighbors[face.c].insert(face.a);
			neighbors[face.a].insert(face.c);
		}
	}

	vector<Vector> tmp_verts(nverts);

	if(has_reference){
		const Vector* target_vertices=target_obj->GetPointR();
		for(int i=0; i<nverts; i++){
			vertices[i]=vertices[i]-target_vertices[i];
		}
	}

	set<long>::iterator it;
	Vector p;
	for(int n=0; n<iterations; n++){
		for(int i=0; i<nverts; i++){
			p=Vector(0,0,0);
			for(it=neighbors[i].begin(); it!=neighbors[i].end(); it++){
				p+=vertices[*it];
			}
			tmp_verts[i]=p/double(neighbors[i].size());
		}

		for(int i=0; i<nverts; i++){
			w=1;
			if(weight){w*=weight[i];}
			vertices[i]=(1-w)*vertices[i]+w*tmp_verts[i];
		}
	}

	if(has_reference){
		const Vector* target_vertices=target_obj->GetPointR();
		for(int i=0; i<nverts; i++){
			vertices[i]=vertices[i]+target_vertices[i];
		}
	}

	op->Message(MSG_UPDATE);
	return true;
}

#define ID_DELTAMUSH 1040828 //ID acquired from plugincafe.com

Bool RegisterDeltaMush(void)
{
	return RegisterObjectPlugin(ID_DELTAMUSH,"DeltaMush",OBJECT_MODIFIER,DeltaMush::Alloc,"Odeltamush",AutoBitmap("DeltaMushIcon.tiff"),0);
}