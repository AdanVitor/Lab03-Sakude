#define PICK_TOL 8
#define sqr(x) (x)*(x)



void LINE_CLASS:: SetP1P2(int xi1, int yi1, int xi2, int yi2)
{  float x1,y1,x2,y2;
	DeviceToNormalized(  xi1, yi1, &x1, &y1);
	TO DO: the same for point 2
	InverseViewingTransformation(&x1, &y1);
	TO DO: the same for point 2
	SetP1P2(x1,y1, x2,y2);
}
void LINE_CLASS::Draw()
{
	SetGraphicsColor(color,width);
	MoveAbs2D(p1.x,p1.y);
	LineAbs2D(p2.x,p2.y);
}

 bool LINE_CLASS:: Pick(int x, int y, float d1)
{  float x1,y1;
	TO DO : transform x, y to world coordinate
	return Pick(x1,y1, d1);
}
bool  LINE_CLASS::Pick(float x, float y, float d);
{  float dist2,xmin,ymin,xmax,ymax;
	xmin=Min(p1.x,p1.x); ymin=Min(p1.y,p2.y); xmax=Max(p1.x,p1.x); ymax=Max(p1.y,p2.y); 
 	dist2 = sqr((x-p1.x)*(p2.y-p1.y)-(y-p1.y)*(p2.x-p1.x))/(sqr(p2.x-p1.x)+sqr(p2.y-p1.y))
	return (dist2<=d*d)&&((xmin-d<=x)&&(x<=xmax+d)&&(ymin-d<=y)&&(y<=ymax+d);
}

void CIRCLE_CLASS:: SetCR(int xci, int yci, int ri)
{  float x1,y1,r1,r2; int ri2=ri; 
	DeviceToNormalized(  xci, yci, &x1, &y1);
	DeviceToNormalized(  ri, ri2, &r1, &r2);
	TO DO transform x1 y1 to worl coordinate
	InverseViewingTransformation(&r1, &r2);
	SetRC(x1,y1, r1);
}


void CIRCLE_CLASS::Draw()
{ int ri, xi,yi;
   float x,y,r0,r1;
   x=c.x; y=c.y;
   r0=r1=r;
	SetGraphicsColor(color,width);
	ViewingTransformation(&x,&y);
	NormalizedToDevice(x,y,&xi,&yi);
	ViewingTransformation(&r0,&r1);
	NormalizedToDevice(r0,r1,&ri,&ri);
	DrawEllipse(xi,yi,ri,ri,0);
}
bool CIRCLE_CLASS:: Pick(int (int x, int y, float d)
{  float x1,y1;
	TO DO Transform x y to world coordnate
	return Pick(x1,y1, d);
}
bool  CIRCLE_CLASS::Pick(float x, float y, float d);
{  float dist2;
 	dist2 = sqr((x-c.x)+(sqr(y-c.y);
     if ((r-d)<=0)
     return (dist2<=sqr(d+r);
	else return  ((sqr(r-d)<=dist2)&&(dist2<=sqr(d+r)));
}

bool SEGMENT_CLASS :: Pick(int x, int y, float d)
{  ENTITY_CLASS *ent;
	float x1,y1; PTR_ENTITY_CLASS ptr;
	TO DO transform x y to world coord x1 y1
	for (int i=0; i<list->n; i++)
	{ ent=list->GetElem(i);
	
		if (ent->Pick(x1,y1,d))
			pick_list->Add(ent);
	}
	return pick_list->n>0;
}

void SEGMENT_CLASS ::Save (char *filename)
{FILE *file;
	fopen_s(&file, filename,"w");
    if (file)
	{  fprintf_s(file,"%d\n",list->n);	
        
   	for (int i=0; i<list->n; i++)
	list->GetElem(i)->Save(file);
	}
	fclose(file);
}

void SEGMENT_CLASS ::Load(char *filename)
{FILE *file; int tipo, cor, esp,m; float x1,  x2,  y1,  y2;
	fopen_s(&file, filename,"r");
    if (file)
	{fscanf_s(file,"%d",&m);	
	list->Reset();
   	for (int i=0; i<m; i++)
	{ fscanf_s(file,"%d %d %d",&tipo, &cor, &esp);
	   if (tipo==1)
	   {   LINE_CLASS *line = new LINE_CLASS;
		   fscanf_s(file,"%f %f %f %f",&x1, &y1, &x2, &y2);
		   line->SetP1P2(x1, y1, x2, y2); 
		   line->SetColor(cor);
		   line->SetWidth(esp);
		   list->Add(line);
	   }
	   else  {
		   CIRCLE_CLASS *circle = new CIRCLE_CLASS;
		   fscanf_s(file,"%f %f %f",&x1, &y1, &x2);
		   circle->SetCR(x1, y1, x2); 
		   circle->SetColor(cor);
		   circle->SetWidth(esp);
		   list->Add(circle);
	   }

	  
	}
	}
	fclose(file);
}

