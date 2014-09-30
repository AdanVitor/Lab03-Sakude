#ifndef _SEGMENT_
#define _SEGMENT_
#include "ListClass.h"

typedef enum { BLACK,BLUE,GREEN,CYAN,RED,MAGENTA,
BROWN,LIGHTGRAY,DARKGRAY,LIGHTBLUE,LIGHTGREEN,
LIGHTCYAN,LIGHTRED,LIGHTMAGENTA,YELLOW,WHITE} color;

void Set_Color(int new_color, int width);
/////////////////////////////////
#define PICK_TOL 8
#define COLOR_DEFAULT BLACK

typedef struct { float x, y;
} point_type;

typedef struct {
  int x,y;
}point_2d;

typedef struct {

       BOOL type,pattern;
	   CPoint pc; 
	   CRect rect;
	   point_2d p[2];
        int color;
} entity_type;


class ENTITY_CLASS
{ public:
 int type, color, width;
 ENTITY_CLASS(int tip, int cor, int wid)
 { type = tip; color=cor; width=wid; }
  ENTITY_CLASS(int tip, int cor)
 { type = tip; color=cor; width=1; }
 ENTITY_CLASS(int tip)
 { type = tip; color=COLOR_DEFAULT; width=1; };
 ENTITY_CLASS()
 {type=0; color=COLOR_DEFAULT;}
  void SetType(int tip)
  { type = tip;
  }
  void SetColor(int cor)
  { color=cor;}
  void SetWidth(int wid)
  {width=wid;}
int GetColor()
{  return color;}
  virtual bool PickI(int x, int y,float d)
  {return true;};
  virtual bool Pick(float x, float y, float d)
	{return true;};
  virtual void Draw()
  {};
  virtual void Draw(int cor)
  {};
  virtual void Save(FILE *file )
  {};
   virtual void Load(FILE *file )
  {};
};


class LINE_CLASS: public ENTITY_CLASS
{  public:
    point_type p1,p2;
	LINE_CLASS(point_type pp1, point_type pp2):ENTITY_CLASS(1)
	{  p1=pp1; p2=pp2;
	}
	LINE_CLASS():ENTITY_CLASS(1)
	{ }
	void SetP1(float xx, float yy)
	{p1.x=xx; p1.y=yy;}
	void SetP2(float xx, float yy)
	{p2.x=xx; p2.y=yy;}
	void SetP1P2(float x1, float y1, float x2, float y2)
	{ SetP1(x1,y1); SetP2(x2,y2);}
	void GetP1P2(point_type *pp1, point_type *pp2)
	{ *pp1=p1; *pp2=p2;}
	point_type GetP1()
	{return p1;}
	point_type GetP2()
	{return p1;}
	void SetP1P2I(int xi1, int yi1, int xi2, int yi2);
	void Draw_();
	void Draw()
	{  Set_Color(color,width);
	   Draw_();
	};
	bool PickI(int x, int y, float d);
    bool Pick(float x, float y, float d);
	void Draw(int cor)
	{Set_Color(cor,width);
	   Draw_();
	}
	void Save(FILE *file)
	{
		fprintf_s(file,"%d %d %d %f %f %f %f\n",
		type, color, width,p1.x,p1.y,p2.x,p2.y);
	}
	void Load(FILE *file)
	{
		fscanf_s(file,"%d %d %d %f %f %f %f",
		&type, &color, &width,&p1.x,&p1.y,&p2.x,&p2.y);
	}
};

class CIRCLE_CLASS: public ENTITY_CLASS
{  public:
    point_type c; float r;
	CIRCLE_CLASS():ENTITY_CLASS(2)
	{ c.x=0; c.y=0; r=0.0; }
	CIRCLE_CLASS(point_type cc, float rr):ENTITY_CLASS(2)
	{ c=cc; r=rr; }
	void SetC(float xx, float yy)
	{c.x=xx; c.y=yy;}
	void SetR(float rr)
	{ r=rr;}
	point_type GetC()
	{ return c;}
	float GetR()
	{return r; }
	void SetCR(float xc, float yc, float r)
	{ SetC(xc,yc); SetR(r);}
	void SetCRI(int xci, int yci, int ri);
	bool PickI(int x, int y, float d);
    bool Pick(float x, float y, float d);
	void Draw_();
  void Draw()
	{  Set_Color(color,width);
	   Draw_();
	};
	void Draw(int cor)
	{Set_Color(cor,width);
	   Draw_();
	}
	void Save(FILE *file)
	{
		fprintf_s(file,"%d %d %d %f %f %f\n",
		type, color, width,c.x,c.y,r);
	}
	void Load(FILE *file)
	{
		fscanf_s(file,"%d %d %d %f %f %f",
		&type, &color, &width,&c.x,&c.y,&r);
	}
};
#define DIM_DEFAULT 100
typedef ENTITY_CLASS *PTR_ENTITY_CLASS;

class SEGMENT_CLASS
{public:
    char name[100];
	 LIST_CLASS<PTR_ENTITY_CLASS> *list;
	 LIST_CLASS<PTR_ENTITY_CLASS> *pick_list; 
	 SEGMENT_CLASS()
	{name[0]=0x00;list=new LIST_CLASS<PTR_ENTITY_CLASS> (DIM_DEFAULT,DIM_DEFAULT/2);
	 pick_list=new LIST_CLASS<PTR_ENTITY_CLASS> (DIM_DEFAULT/4,DIM_DEFAULT/8);
	 };
	 SEGMENT_CLASS(char *nam)
	{SetName(nam);list=new LIST_CLASS<PTR_ENTITY_CLASS> (DIM_DEFAULT,DIM_DEFAULT/2);
	 pick_list=new LIST_CLASS<PTR_ENTITY_CLASS> (DIM_DEFAULT/4,DIM_DEFAULT/8);};
	SEGMENT_CLASS(int DIM)
	{ list=new LIST_CLASS<PTR_ENTITY_CLASS> (DIM,DIM/2);
	pick_list=new LIST_CLASS<PTR_ENTITY_CLASS> (DIM/2,DIM/4);
	}
	 ~SEGMENT_CLASS()
	{ delete list;};
	 
	 void SetName(char *new_name)
	 {strcpy(name,new_name);};
   
void AddLine(point_type p1, point_type p2, int cor)
	{ LINE_CLASS *ptr_line=new LINE_CLASS(p1,p2); 
		ptr_line->SetColor(cor);
		list->Add((PTR_ENTITY_CLASS) ptr_line);
	}
	void AddLine(PTR_ENTITY_CLASS ptr_line)
	{ list->Add((PTR_ENTITY_CLASS) ptr_line); }
 void AddCircle(point_type c, float r, int cor)
	{ CIRCLE_CLASS *ptr_circle=new CIRCLE_CLASS(c,r);
 ptr_circle->SetColor(cor);
		list->Add((PTR_ENTITY_CLASS)ptr_circle);

	}

 void AddCircle(PTR_ENTITY_CLASS ptr_line)
	{ list->Add((PTR_ENTITY_CLASS) ptr_line); }
void Draw()
	{ int n=list->GetNElem();
		for (int i=0; i<n; i++)
		   list->GetElem(i)->Draw();		
	}

bool Pick(int x, int y, float d);
LIST_CLASS<PTR_ENTITY_CLASS> *PickList()
	 {return pick_list;}

void Save (char *filename);
void Load(char *filename);

};

typedef   SEGMENT_CLASS *PTR_SEGMENT_CLASS;

class SCENE_CLASS    // it can be SCENE GRAPH CLASS
{public:
     LIST_CLASS<PTR_SEGMENT_CLASS> *list;
	 SCENE_CLASS()
	{  list =  new LIST_CLASS<PTR_SEGMENT_CLASS>(DIM_DEFAULT,DIM_DEFAULT/2);
}
void AddNode (PTR_SEGMENT_CLASS node) // Add node to list
	{ 	list->Add((PTR_SEGMENT_CLASS) node);
	}
PTR_SEGMENT_CLASS GetNode(char *nodename); // Get node by name
PTR_SEGMENT_CLASS PickNode(int x, int y, float d); // Get node by pointing
void DeleteNode(PTR_NODE_CLASS node); // Delete node
void Save (char *filename);
void Load(char *filename);
};



#endif

