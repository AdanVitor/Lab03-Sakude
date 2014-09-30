int p0_x, p0_y, p1_x,p1_y, X_1,y_1, x_2,y_2; 

if (mouse_action==L_MOUSE_DOWN)
		{  // Pick first point up 

		 
		if (polygon.n==0)
		{   p0_x=p1_x=mouse_x;
			p0_y=p1_y=mouse_y;
			InsertVertex(polygon,p0_x,p0_y);
		}
		}
     if (mouse_action==L_MOUSE_MOVE_DOWN)
	 {  // Example of elastic line
		 if (p1_x!=mouse_x || p1_y!=mouse_y)
		 {  // Erase previous line. NOTE: using XOR line
	  
		DrawLineXor(p0_x,p0_y,p1_x,p1_y);
		p1_x=mouse_x;
		p1_y=mouse_y;  // Draw new line
		
		DrawLineXor(p0_x,p0_y,p1_x,p1_y);
		   
			
		x_1=p0_x;
		y_1=p0_y;
		x_2=p1_x;
		y_2=p1_y;
		
		 }	 
	 }
      else  if(mouse_action==L_MOUSE_UP)
		 {	
		//  SetGraphicsColor((int)MY_LIGHTGREEN);
		  DrawLine(p0_x,p0_y,p1_x,p1_y);
			p0_x=p1_x=mouse_x;
			p0_y=p1_y=mouse_y;
	
			if (polygon.n>0 && 
				( polygon.vertex[polygon.n-1].x!=p0_x
				||polygon.vertex[polygon.n-1].y!=p0_y))
			InsertVertex(polygon,p0_x,p0_y);
		
		
      		mouse_action=NO_ACTION;
		 }
	else  if(mouse_action==R_MOUSE_DOWN)
		 {
			TO DO 	
			//	DrawPoly(polygon);
  			// if
			 
			//	  FillPolygon(polygon, list);
			  // else 
			//	  FloodFillRecursive(polygon);
			     

			  
  				mouse_action=NO_ACTION;
				polygon.n=0;
		 }