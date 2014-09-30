



float x_start,x_end,y_start,y_end,heigth,width; /* screem bitmap size */

float wxs=0.0,wxh=1.0,wys=0.0,wyh=1.0;  /* windows corners variables */

float vxs=0.0,vxh=1.0,vys=0.0,vyh=1.0; /* viewport corners variables */

float vwsx,vwsy; /* viewing transformation scale */



int inside=0, botton=1, top=2, right=4, left=8;
float x_current,y_current;


void SetWindow(float x1, float x2, float y1, float y2)


/* window specification and scale transformation computation */
{
   wxs=x1;
   wxh=x2;
   wys=y1;
   wyh=y2;
   vwsx=(vxh-vxs)/(wxh-wxs);
   vwsy=(vyh-vys)/(wyh-wys);
}

void SetViewport( float x1,float x2,float y1, float y2)

/* Viewport specification and scale transformation computation */
{
   vxs=x1;
   vxh=x2;
   vys=y1;
   vyh=y2;
   vwsx=(vxh-vxs)/(wxh-wxs);
   vwsy=(vyh-vys)/(wyh-wys);
}
void ViewingTransformation(float *x,float *y)

/* transformation from object point to normalized point */
{
   *x=(*x - wxs)*vwsx + vxs;
   *y=(*y - wys)*vwsy + vys;
}

void NormalizedToDevice( float xn, float yn, int *xd, int *yd)

/* transformation from normalized point to device coordenate (its rounds the
float number) */
{
   *xd=(int)(x_start +width*xn);
   *yd=(int)(y_end -(y_start+heigth*yn));
}


void XYEdgeIntersection(float  *x1,float *x2,float *y1,float *y2,
						float wy,float *x,float *y )

{
     *x=*x1 + (*x2-*x1)*(wy-*y1)/(*y2-*y1);
     *y=wy;
}

void LineAbs2(float x2,float y2 )

{ float x1,y1;
  int xi1,yi1,xi2,yi2;

   x1=x_current;
   y1=y_current;
   	 x_current=x2;
	y_current=y2;

       ViewingTransformation( &x1,&y1 );
       ViewingTransformation( &x2,&y2 );
       NormalizedToDevice( x1,y1, &xi1,&yi1);
       NormalizedToDevice( x2,y2, &xi2, &yi2);
       DrawLine(xi1,yi1, xi2,yi2);
    
}
void MoveAbs2D(float x,float y)

{
    x_current=x;
    y_current=y;
}


void MoveRel2D(float dx,float dy)

{
    x_current += dx;
    y_current += dy;
}
void DrawLine2(float x1, float y1, float x2, float y2, int color)
{
	SetGraphicsColor(color,2);
	 MoveAbs2D(x1,y1);
	LineAbs2(x2,y2);

}
void SetCode2D(float *x,float *y, int *c)

/* This procedure sets the point code */
 {
      *c = inside;
      if ( *x < wxs)
	 *c |= left ;
	 else if ( *x > wxh)
	     *c |= right;
      if (*y < wys)
	 *c |=  botton;
	 else if ( *y > wyh)
	   *c |= top ;
}

bool Clip2D(float *x1,float *y1,float *x2,float *y2)


{
 int c , c1 , c2;
 float x, y;


      SetCode2D (x1,y1, &c1);
      SetCode2D (x2,y2, &c2);
      while (((c1 != inside ) || (c2 != inside)) )
         {
	 if ( (c1&c2) != inside )
	 {
		
	    return(false);
	 }
	  else
           {
	      c = c1;
	      if (c == inside)
			c = c2;

	      if  (left & c)
		XYEdgeIntersection( y1,y2,x1,x2, wxs,&y,&x);
	      else if  (right & c)
		XYEdgeIntersection( y1,y2,x1,x2, wxh,&y,&x );
	      else if  (botton & c)
		TO DO // call XYEdgeIntersection, change x by y 
	      else if (top & c)
		TO DO // call XYEdgeIntersection, change x by y  
	      if (c == c1)
              {
			  
		 *x1= x;
		 *y1= y;

		 SetCode2D (x1,y1, &c1);
              }
              else
              {
		TO DO: same above, replace 1 by 2
		 
              }
            }
	 }
      return(true);
}


void DrawLine2D(float x2,float y2 )

{ float x1,y1;
  int xi1,yi1,xi2,yi2;

   x1=x_current;
   y1=y_current;

	TO DO <MATRIZ>.DoTransformation(&x1,&y1 );
	TO DO  Faça Transformacao x2 y2 

   if  (Clip2D(&x1,&y1, &x2, &y2))
     {
       ViewingTransformation( &x1,&y1 );
      TO DO ViewingTransformation for point 2
       NormalizedToDevice( x1,y1, &xi1,&yi1);
       TO DO NormalizedToDevice for point 2
       DrawLine(xi1,yi1, xi2,yi2);
     }
}

void LineAbs2D(float x,float y)

{
    DrawLine2D( x,y );
    x_current=x;
    y_current=y;
}


void LineRel2D(float dx,float dy)

{
    dx += x_current;
    dy += y_current;
    DrawLine2D( dx, dy );
    x_current = dx;
    y_current = dy;
}




void InitGraf()
{
	y_end=(float)GetMaxY();
	x_end=(float)GetMaxX();
	
	x_start = 0.0f;
	y_start = 0.0f;
	width = (float) (x_end - x_start);
	heigth = (float) (y_end - y_start);
	MoveAbs2D(0.0f,0.0f);
	SetViewport (0.0f , 1.0f, 0.0f , 1.0f);
	SetWindow(0.0f,(float)x_end,0.0f,(float)y_end);
}