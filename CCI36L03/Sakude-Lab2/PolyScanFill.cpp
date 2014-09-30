void InsertVertex(polygon_type &poly, int x, int y)
{	// insert x,y as the last element
	 if (poly.n<MAX_POLY)
	 {
        poly.vertex[poly.n].x=x;
        poly.vertex[poly.n].y=y;
		poly.n++;
	 }

}



void GetPoint(polygon_type polygon, int k , int &x , int &y ) 
{
	x = polygon.vertex[k].x;
	y = polygon.vertex[k].y;

}

void PolyInsert(edge_list_type &list, int x1, int y1, int x2, int y2)
{     // insert line segment in edge struct, if not horizontal
     if (y1!=y2)
	 {  
		 int YM=Max(y1,y2),J1=list.n+1;
	      
	     while (J1!=1 &&  list.edge[J1-1].Ymax<YM)
		 {
			 list.edge[J1]=list.edge[J1-1];
			 J1--;
		  }

		 list.edge[J1].Ymax=YM;
		 list.edge[J1].dx = -1*(float)(x2-x1)/(y2-y1);
		 if (y1>y2)
		 {
	       list.edge[J1].Ymin=y2;
		   list.edge[J1].Xinter=(float)x1;
		 }
		 else {
		   list.edge[J1].Ymin=y1;
		   list.edge[J1].Xinter=(float)x2;
		 }

		 list.n++;
	 }

}

void LoadPolygon(polygon_type &polygon, edge_list_type &list, int &num_Edges)
{
	int x1,x2,y1,y2,k=1;
	
	list.n = 0;
	GetPoint(polygon, k,x1,y1);
	num_Edges = 0;

	for ( ; k <= polygon.n ; k++ ) 
	{
		GetPoint(polygon, k%polygon.n+1,x2,y2);
		if (y1==y2) x1 = x2 ;
		else 
		{
			PolyInsert(list, x1,y1,x2,y2);
			num_Edges+=1;
			x1=x2;
			y1=y2;
		}
	}

} 


void Include(edge_list_type &list, int &end_Edge, int final_Edge, int scan)
{	// include all edges that intersept y_scan
   while (end_Edge < final_Edge && list.edge[end_Edge+1].Ymax >= scan)
   {
	end_Edge++;
   }
}

void XSort(edge_list_type &list, int start_Edge, int last_Edge)
{  
	int L,k ; bool sorted=false;
	edge_type temp;
	
	// Use bubble sort
	
    for ( k = start_Edge ; k < last_Edge; k++ )
	{
	  
	   L = k + 1; 
	  
	   while ( L > start_Edge && 
		   list.edge[L].Xinter < list.edge[L-1].Xinter )
	   {
		   temp=list.edge[L];
		   list.edge[L]=list.edge[L-1];
		   list.edge[L-1]=temp;
		   L--;
		 
	   }

   }
	
} 

void FillIn ( int x1 , int x2 , int y )
{
	int px,py;

	py = y  % 8 ;
	
	if (x1!=x2)
	{
		for (int x = x1 ; x <= x2 ; x++ ) 
		{
			px=x%8;

			DrawPixel(x,y);
		
		}
	} 
}

void FillScan (edge_list_type &list, int end_Edge , int start_Edge , int scan )
{
	int NX , J , K;
	
	
		NX = (end_Edge - start_Edge + 1)/2;
	
	J = start_Edge;
	
	
	for ( K = 1 ; K <= NX ; K++ )
	{
		FillIn ( (int)list.edge[J].Xinter, 
			(int)list.edge[J+1].Xinter, scan);
			J += 2;
	}
	
}

void UpdateXValues(edge_list_type &list, int last_Edge , int &start_Edge , int scan)
{
	int K1;

	for ( K1 = start_Edge ; K1 <= last_Edge ; K1++)
	{
		if ( list.edge[K1].Ymin < scan )
		{
			list.edge[K1].Xinter += list.edge[K1].dx ;
		}
		else 
		{  // remove edge
			start_Edge++;
			if ( start_Edge <= K1 ) 
				for ( int i = K1 ; i >= start_Edge ; i--)
					list.edge[i]=list.edge[i-1]; 
		}
   }
}

	
void FillPolygon (polygon_type &polygon, edge_list_type &list )
{
	int Edges , start_Edge , end_Edge , scan;
	
	LoadPolygon(polygon,list, Edges);
	if (Edges==2) return;
	scan = list.edge[1].Ymax ;
	start_Edge = 1 ;
	end_Edge = 1 ;

	Include(list, end_Edge, Edges, scan);
	while ( end_Edge != start_Edge - 1 ) 
	{
		XSort(list, start_Edge, end_Edge);
		FillScan(list, end_Edge, start_Edge, scan);
		scan--;
		UpdateXValues(list, end_Edge, start_Edge, scan);
		Include(list, end_Edge, Edges, scan);
	}

}
