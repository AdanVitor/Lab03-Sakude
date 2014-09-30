typedef enum {LEFT,TOP,RIGHT,BOTTOM,LAST} win_edge_type;

bool LineIntersectiom(float_point_type P1,float_point_type P2,win_edge_type edge)
{
  switch (edge)
  {
  case LEFT: return ((P1.x-wxs)*(P2.x-wxs)<0);
  case RIGHT: return ((P1.x-wxh)*(P2.x-wxh)<0);
  case TOP: return ((P1.y-wyh)*(P2.y-wyh)<0);
  case BOTTOM: return ((P1.y-wys)*(P2.y-wys)<0);
  }
  return false;
}
bool Visible(float_point_type P, win_edge_type edge)
{
	 switch (edge)
  {
 
 case LEFT: return (P.x>=wxs);
  case RIGHT: return (P.x<=wxh);
  case TOP: return (P.y<=wyh);
  case BOTTOM: return (P.y>=wys);
	 }
	 return false;
}
float_point_type Intersection(float_point_type P1,float_point_type P2,
						win_edge_type edge)
{ float_point_type p;
  switch (edge)
  {
  case LEFT:  
	        p.x=wxs;
			p.y = (P1.y+((float)(P2.y-P1.y))/(P2.x-P1.x)*(p.x-P1.x));

	  break;
  case RIGHT:p.x=wxh;
			TO DO same above
	  break;
  case TOP: p.y=wyh;
			TO DO CHANGE x by y and vice versa 
	  break;
  case BOTTOM: p.y=wys;
			TO DO copy above equation here
 }
  return p;

}
void ClipEdge(float_point_type P1, float_point_type P2, win_edge_type edge,
float_polygon_type &poly_out)
{  float_point_type Pi;
  	if (Visible(P1,edge)) // P is at the same side of window
	     InsertVertex(poly_out,P1.x,P1.y)	;
	 if (LineIntersectiom(P1,P2,edge))
	{ Pi=Intersection(P1,P2, edge);
		   InsertVertex(poly_out,Pi.x,Pi.y);
			}

}
void ClipPolygon(float_polygon_type poly, float_polygon_type &poly_out)
{  win_edge_type edge;
	for (int edg=0; edg<4; edg++)
	{  	poly_out.n=0; // Reset poly_out	
	edge=(win_edge_type)edg;
	  poly.vertex[poly.n]=poly.vertex[0];
		for (int i=0; i<poly.n; i++) 
			ClipEdge(poly.vertex[i],poly.vertex[i+1],  edge,poly_out);
    
		poly=poly_out;// Copy poly_out to poly
	
		
	}

}

void DrawPolygon(float_polygon_type poly)
{  float_polygon_type poly_out;
   for (i=0; i<poly.n; i++) // Transformação do usuário
   	 <Matriz>.DoTransformatio(poly.vertex[i].x, poly.vertex[i].y)
   Call Polygon Clipping // recorte do poligono
   if (poly_out.n>0) // resultou em poligono dentro
   {   for (i=0; i<poly_out.n; i++) // transformacao de janelamento
	  {  ViewingTransform (<poly _out.vertex[i].x>, <pol _out.vertex[i].y>); 
	      Normalizado Para Dispositivo (<poly_out.vertex[i].x>, <poly_out.vertex[i].y>);
	  }
	if (fill_polygon)
	    FillPolygon(poly_out); // preenche poligono
	else DrawPolyg(poly_out); // desenha
   }
}
