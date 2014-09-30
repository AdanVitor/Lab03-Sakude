#ifndef _POLYCLIP_
#define _POLYCLIP_

#define MAX_POLY 50
#define Max(a,b) ((a>b)?a:b)
#define Min(a,b) ((a<b)?a:b)
typedef struct {
			float x,y;
			} float_point_type;
typedef struct {
    int n; 
	float_point_type vertex[MAX_POLY];
} float_polygon_type;

void ClipPolygon(float_polygon_type poly, float_polygon_type &poly_out);
void DrawPolygon(float_polygon_type poly);

#endif
